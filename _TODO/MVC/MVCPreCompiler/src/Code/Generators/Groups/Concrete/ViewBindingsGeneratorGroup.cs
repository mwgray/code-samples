using System.Collections.Generic;
using System.Linq;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Dependencies;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class ViewBindingsGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            var dependentMethods = classDescriptor.methods.Where(p => p.dependencies.Count > 0);
            var dependencies = dependentMethods.SelectMany(d => d.dependencies).ToList();

            var bindings = BindFields(dependencies.OfType<FieldDependency>())
                           + BindCollections(dependencies.OfType<CollectionOperationDependency>())
                           + BindCollectionFields(dependencies.OfType<CollectionFieldDependency>()).Trim();

            var hasBindings = !string.IsNullOrEmpty(bindings);
            value = value.EvaluateConditional("HasBindings", hasBindings);

            return hasBindings ? ReplaceKeyword(value, bindings) : value;
        }

        private string BindFields(IEnumerable<FieldDependency> dependencies)
        {
            return dependencies.Select(d => string.Format("Data._{0}Property.Subscribe(_=>{1}());\n", d.dependency.fieldInfo.Name, d.dependent.memberInfo.Name)).Join("");
        }

        private string BindCollections(IEnumerable<CollectionOperationDependency> dependencies)
        {
            return dependencies.Select(d =>
            {
                var parameters = d.operation == CollectionOperationDependency.Operation.Replace ? "_.NewValue, _.OldValue" : "_.Value";
                return string.Format("Data.{0}.Observe{1}().Subscribe(_=> {2}({3}));\n", d.dependency.fieldInfo.Name, d.operation, d.dependent.memberInfo.Name, parameters);
            }).Join("");
        }

        private string BindCollectionFields(IEnumerable<CollectionFieldDependency> dependencies)
        {
            return dependencies.Select(d => string.Format("Data.NOTIMPL_{0}Property.Subscribe(_=>{1}());\n", d.dependency.fieldInfo.Name, d.dependent.memberInfo.Name)).Join("");
        }
    }
}