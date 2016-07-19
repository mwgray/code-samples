using Graymatter.Code.Descriptors.Member;

namespace Graymatter.Code.Descriptors.Dependencies
{
    public abstract class DependencyDescriptor
    {
        public MemberDescriptor dependent;

        public FieldDescriptor dependency;

        protected DependencyDescriptor(MemberDescriptor dependent, FieldDescriptor dependency)
        {
            this.dependent = dependent;

            this.dependency = dependency;

            this.dependency.dependents.Add(dependent);
        }
    }
}