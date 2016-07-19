using Graymatter.Code.Descriptors.Member;

namespace Graymatter.Code.Descriptors.Dependencies
{
    public class CollectionOperationDependency: DependencyDescriptor
    {
        public CollectionDescriptor Collection
        {
            get { return dependency as CollectionDescriptor; }
        }

        public enum Operation
        {
            Add,
            Remove,
            Replace,

            Any,
        }

        public Operation operation;

        public CollectionOperationDependency(MemberDescriptor dependent, CollectionDescriptor collection, Operation operation)
            : base(dependent, collection)
        {
            this.operation = operation;
        }
    }
}