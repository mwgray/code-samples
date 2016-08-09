using System.Linq;
using Graymatter.Code.Descriptors.Class;

namespace Graymatter.Code.Generators.Groups
{
    public abstract class DataGeneratorGroup: GeneratorGroup
    {
        public static string InjectAll(string value, ClassDescriptor classDescriptor)
        {
            return GetGeneratorGroups<DataGeneratorGroup>().Aggregate(value, (v, g) => g.Inject(v, classDescriptor));
        }

        public abstract string Inject(string value, ClassDescriptor classDescriptor);
    }
}