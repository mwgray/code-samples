using System.Linq;
using Graymatter.Code.Descriptors.Member;

namespace Graymatter.Code.Generators.Groups
{
    public abstract class CollectionGeneratorGroup: GeneratorGroup
    {
        public static string InjectAll(string value, CollectionDescriptor collectionDescriptor)
        {
            return GetGeneratorGroups<CollectionGeneratorGroup>().Aggregate(value, (v, g) => g.Inject(v, collectionDescriptor));
        }

        public abstract string Inject(string value, CollectionDescriptor collectionDescriptor);
    }
}