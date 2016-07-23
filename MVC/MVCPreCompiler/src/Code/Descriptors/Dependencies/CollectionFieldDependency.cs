using Graymatter.Code.Descriptors.Member;

namespace Graymatter.Code.Descriptors.Dependencies
{
    public class CollectionFieldDependency: DependencyDescriptor
    {
        public readonly CollectionDescriptor collection;

        public CollectionFieldDependency(MemberDescriptor dependent, FieldDescriptor dependency, CollectionDescriptor collection)
            : base(dependent, dependency)
        {
            this.collection = collection;
        }
    }
}