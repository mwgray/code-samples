using System.Reflection;
using Graymatter.Code.Descriptors.Class;

namespace Graymatter.Code.Descriptors.Member
{
    public class MethodDescriptor: MemberDescriptor
    {
        public readonly MethodInfo methodInfo;

        public MethodDescriptor(ClassDescriptor owner, MethodInfo methodInfo)
            : base(owner, methodInfo)
        {
            this.methodInfo = methodInfo;
        }
    }
}