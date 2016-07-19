using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Graymatter.Code.Descriptors.Member;

namespace Graymatter.Code.Descriptors.Class
{
    public class ClassDescriptor
    {
        public readonly Type type;

        public IEnumerable<MemberDescriptor> Members
        {
            get { return fields.Union(collections.Union(methods.OfType<MemberDescriptor>())); }
        }

        public virtual string ClassName
        {
            get { return type.Name.Replace("Declaration", ""); }
        }

        public readonly List<FieldDescriptor> fields;
        public readonly List<CollectionDescriptor> collections;
        public readonly List<MethodDescriptor> methods;

        public ClassDescriptor(Type type)
        {
            this.type = type;

            var fieldInfos = type
                .GetFields()
                .Where(m=>m.MemberType != MemberTypes.Constructor)
                .ToList();

            var methodInfos = type
                .GetMethods(BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance) 
                .Where(m => m.DeclaringType == type && m.MemberType != MemberTypes.Constructor)
                .ToList();

            fields = fieldInfos.Where(f => !IsCollection(f)).Select(f => new FieldDescriptor(this, f)).ToList();
            collections = fieldInfos.Where(f => IsCollection(f)).Select(f => new CollectionDescriptor(this, f)).ToList();
            methods = methodInfos.Select(m => new MethodDescriptor(this, m)).ToList();
        }

        public static bool IsCollection(FieldInfo fieldInfo)
        {
            return fieldInfo.FieldType.Name.Contains("List");
        }
    }
}