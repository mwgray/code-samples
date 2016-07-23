using System.Linq;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;
using Graymatter.Properties;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class CollectionsGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            var lines = classDescriptor.collections.Where(m=>m.memberInfo.DeclaringType == classDescriptor.type).Select(GenerateCollection).Join("\n");

            return ReplaceKeyword(value, lines);
        }

        private string GenerateCollection(CollectionDescriptor collectionDescriptor)
        {
            var template = collectionDescriptor.dependencies.Count > 0 ? Resources.CalculatedCollectionProperty : Resources.CollectionProperty;

            var type = collectionDescriptor.subType.Name;
            var name = collectionDescriptor.fieldInfo.Name;

            var result = template
                                   .Replace("$Name$", name)
                                   .Replace("$Type$", type);

            return result;
        }
    }
}