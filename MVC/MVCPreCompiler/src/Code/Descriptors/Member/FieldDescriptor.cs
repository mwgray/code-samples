using System.Reflection;
using Graymatter.Code.Descriptors.Class;

namespace Graymatter.Code.Descriptors.Member
{
    public class FieldDescriptor: MemberDescriptor
    {
        public readonly FieldInfo fieldInfo;

        public FieldDescriptor(ClassDescriptor owner, FieldInfo fieldInfo)
            : base(owner, fieldInfo)
        {
            this.fieldInfo = fieldInfo;
        }
    }


}