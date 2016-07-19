using Graymatter.Code.Descriptors.Member;

namespace Graymatter.Code.Descriptors.Dependencies
{
    public class FieldDependency: DependencyDescriptor
    {
        public FieldDependency(MemberDescriptor dependent, FieldDescriptor dependency)
            : base(dependent, dependency)
        {
        }
    }
}