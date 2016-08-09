using System.Linq;
using Graymatter.Code.Descriptors.Dependencies;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class CollectionBindingsCreatorGroup: CollectionGeneratorGroup
    {
        public override string Inject(string value, CollectionDescriptor collectionDescriptor)
        {
            keyword = "{" + collectionDescriptor.fieldInfo.Name + "Bindings}";

            var classDescriptor = collectionDescriptor.owner;
            var collectionDependencies = classDescriptor.Members.Where(m=>m.memberInfo.DeclaringType == classDescriptor.type).SelectMany(p => p.dependencies.OfType<CollectionOperationDependency>()).Where(c => c.Collection == collectionDescriptor).ToList();

            var dependentGrouping = collectionDependencies.GroupBy(d => d.dependent, d => d);

            var subscribeCalls = dependentGrouping
                .Select(g => g.Select(FormatSubscribe).Join("\n") + "\n")
                .Join("\n")
                .Trim();

            return ReplaceKeyword(value, subscribeCalls);
        }

        private string FormatSubscribe(CollectionOperationDependency d)
        {
            if(d.dependent is FieldDescriptor)
            {
                return string.Format("_{0}.Observe{1}().Subscribe(_ => Update{2}());", d.dependency.fieldInfo.Name, d.operation, d.dependent.memberInfo.Name);
            }

            var parameters = "";

            var methodDescriptor = d.dependent as MethodDescriptor;
            var parameterInfos = methodDescriptor.methodInfo.GetParameters();
            if(parameterInfos.Length > 0)
            {
                parameters = d.operation == CollectionOperationDependency.Operation.Replace ? "_.NewValue, _.OldValue" : "_.Value";
            }

            return string.Format("_{0}.Observe{1}().Subscribe(_ => {2}({3}));", d.dependency.fieldInfo.Name, d.operation, d.dependent.memberInfo.Name, parameters);
        }
    }
}