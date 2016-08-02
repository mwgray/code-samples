using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Dependencies;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;
using Graymatter.Common.Logging;

namespace Graymatter.Code.Descriptors
{
    public class AssemblyDescriptor
    {
        public readonly Dictionary<Type, ClassDescriptor> classDescriptors = new Dictionary<Type, ClassDescriptor>();

        public AssemblyDescriptor(Assembly assembly)
        {
            _assembly = assembly;

            FindTypes();

            FindDependencies();
        }

        private readonly Assembly _assembly;

        private void FindTypes()
        {
            var assemblyTypes = _assembly.GetTypes();

            foreach(var type in assemblyTypes.Where(t => t.IsSubclassOf(typeof(DataDeclaration))))
            {
                classDescriptors[type] = new DataDescriptor(type);
            }

            foreach(var type in assemblyTypes.Where(t => t.IsSubclassOf(typeof(ViewDeclaration))))
            {
                var viewDeclarationType = type;

                while(viewDeclarationType.BaseType != typeof(ViewDeclaration))
                {
                    viewDeclarationType = viewDeclarationType.BaseType;
                }

                var dataType = viewDeclarationType.GetGenericArguments()[0];
                var viewDescriptor = new ViewDescriptor(type, (DataDescriptor)classDescriptors[dataType]);

                classDescriptors[type] = viewDescriptor;
            }

            foreach(var type in assemblyTypes.Where(t => t.IsSubclassOf(typeof(ControllerDeclaration))))
            {
                var controllerDeclarationType = type;

                while(controllerDeclarationType.BaseType != typeof(ControllerDeclaration))
                {
                    controllerDeclarationType = controllerDeclarationType.BaseType;
                }

                var dataType = controllerDeclarationType.GetGenericArguments()[0];
                var controllerDescriptor = new ControllerDescriptor(type, (DataDescriptor)classDescriptors[dataType]);

                classDescriptors[type] = controllerDescriptor;
            }
        }

        private void FindDependencies()
        {
            var dependents = classDescriptors.Values.SelectMany(c => c.fields).Cast<MemberDescriptor>()
                                             .Concat(classDescriptors.Values.SelectMany(c => c.methods))
                                             .Concat(classDescriptors.Values.SelectMany(c => c.collections));

            foreach(var dependent in dependents)
            {
                var dependencyNames = dependent.memberInfo.GetCustomAttributes<DependenciesAttribute>().SelectMany(a => a.dependencies);

                var scope = dependent.owner;
                var viewDescriptor = scope as ViewDescriptor;

                if(viewDescriptor != null)
                {
                    scope = viewDescriptor.dataDescriptor;
                }

                foreach(var dependencyName in dependencyNames)
                {
                    if(TryResolveFieldDependency(scope, dependent, dependencyName)
                       || TryResolveCollectionDependency(scope, dependent, dependencyName))
                    {
                        continue;
                    }

                    Log.Warning("error: Could not resolve dependency {0}.{1}", scope.type.Name, dependencyName);
                }
            }
        }

        private bool TryResolveFieldDependency(ClassDescriptor classDescriptor, MemberDescriptor dependent, string dependencyName)
        {
            if(dependencyName.Contains("_"))
            {
                var strings = dependencyName.Split('_');
                var scope = strings[0];
                var name = strings[1];

                var dependency = classDescriptor.fields.FirstOrDefault(p => p.fieldInfo.Name == scope);

                if(dependency != null)
                {
                    var subFieldInfo = dependency.fieldInfo.FieldType.GetField(name);

                    if(subFieldInfo != null)
                    {
                        dependent.dependencies.Add(new SubFieldDependency(dependent, dependency, subFieldInfo));
                    }

                    return true;
                }
            }
            else
            {
                var dependency = classDescriptor.fields.FirstOrDefault(p => p.fieldInfo.Name == dependencyName);
                if(dependency != null)
                {
                    dependent.dependencies.Add(new FieldDependency(dependent, dependency));

                    return true;
                }
            }

            return false;
        }

        private bool TryResolveCollectionDependency(ClassDescriptor classDescriptor, MemberDescriptor dependent, string dependencyName)
        {
            if(dependencyName.Contains("."))
            {
                var strings = dependencyName.Split('.');
                var scope = strings[0];
                var name = strings[1];

                var collectionDescriptor = classDescriptor.collections.FirstOrDefault(c => c.fieldInfo.Name == scope);

                if(collectionDescriptor != null)
                {
                    var subFieldInfo = collectionDescriptor.subType.GetField(name);

                    var subClassDescriptor = classDescriptors[collectionDescriptor.subType];
                    var dependency = subClassDescriptor.fields.FirstOrDefault(p => p.fieldInfo == subFieldInfo);

                    if(dependency != null)
                    {
                        dependent.dependencies.Add(new CollectionFieldDependency(dependent, dependency, collectionDescriptor));

                        return true;
                    }
                }
            }
            else
            {
                var operation = CollectionOperationDependency.Operation.Any;

                // check for sepecific commands
                if(dependencyName.Contains("#"))
                {
                    var strings = dependencyName.Split('#');
                    dependencyName = strings[0];
                    operation = strings[1].ToEnum<CollectionOperationDependency.Operation>();
                }

                // is directly dependent on the collection
                var dependency = classDescriptor.collections.FirstOrDefault(c => c.fieldInfo.Name == dependencyName);

                if(dependency != null)
                {
                    if(operation == CollectionOperationDependency.Operation.Any)
                    {
                        dependent.dependencies.Add(new CollectionOperationDependency(dependent, dependency, CollectionOperationDependency.Operation.Replace));
                        dependent.dependencies.Add(new CollectionOperationDependency(dependent, dependency, CollectionOperationDependency.Operation.Add));
                        dependent.dependencies.Add(new CollectionOperationDependency(dependent, dependency, CollectionOperationDependency.Operation.Remove));
                    }
                    else
                    {
                        dependent.dependencies.Add(new CollectionOperationDependency(dependent, dependency, operation));
                    }

                    return true;
                }
            }

            return false;
        }
    }
}