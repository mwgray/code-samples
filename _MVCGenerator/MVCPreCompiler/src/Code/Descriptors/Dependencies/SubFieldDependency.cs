using System.Reflection;
using Graymatter.Code.Descriptors.Member;

namespace Graymatter.Code.Descriptors.Dependencies
{
    public class SubFieldDependency: DependencyDescriptor
    {
        public readonly FieldInfo subFieldInfo;

        public SubFieldDependency(MemberDescriptor dependent, FieldDescriptor dependency, FieldInfo subFieldInfo)
            : base(dependent, dependency)
        {
            this.subFieldInfo = subFieldInfo;
        }
    }
}