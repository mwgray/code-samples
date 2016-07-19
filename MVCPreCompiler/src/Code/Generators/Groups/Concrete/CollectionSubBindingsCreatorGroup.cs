using System.Linq;
using Graymatter.Code.Descriptors;
using Graymatter.Code.Descriptors.Dependencies;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class CollectionSubBindingsCreatorGroup: CollectionGeneratorGroup
    {
        public override string Inject(string value, CollectionDescriptor collectionDescriptor)
        {
            keyword = "{" + collectionDescriptor.fieldInfo.Name + "SubBindings}";

            const string propertyTemplate = "value._{0}Property.Subscribe(_ => Update{1}()),";
            const string methodTemplate = "value._{0}Property.Subscribe(_ => {1}()),";

            var classDescriptor = collectionDescriptor.owner;
            var collectionDependencies = classDescriptor.Members.Where(m => m.memberInfo.DeclaringType == classDescriptor.type).SelectMany(p => p.dependencies.OfType<CollectionFieldDependency>()).Where(c => c.collection == collectionDescriptor).ToList();

            var dependentGrouping = collectionDependencies.GroupBy(d => d.dependent, d => d);

            var subscribeCalls = dependentGrouping
                .Select(g => g.Select(d => string.Format(d.dependent is FieldDescriptor ? propertyTemplate : methodTemplate, d.dependency.fieldInfo.Name, d.dependent.memberInfo.Name)).Join("\n") + "\n")
                .Join("\n").Trim();

            return ReplaceKeyword(value, subscribeCalls);
        }
    }
}