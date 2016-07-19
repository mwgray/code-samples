using System;
using System.Reflection;
using Graymatter.Code.Descriptors.Class;

namespace Graymatter.Code.Descriptors.Member
{
    public class CollectionDescriptor: FieldDescriptor
    {
        public readonly Type subType;

        public CollectionDescriptor(ClassDescriptor owner, FieldInfo fieldInfo)
            : base(owner, fieldInfo)
        {
            subType = fieldInfo.FieldType.GetGenericArguments()[0];
        }
    }
}