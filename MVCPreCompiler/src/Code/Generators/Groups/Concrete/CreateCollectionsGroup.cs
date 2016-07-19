using System.Linq;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class CreateCollectionsGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            var lines = classDescriptor.collections.Where(m => m.memberInfo.DeclaringType == classDescriptor.type).Select(InjectCall).Join("\n");

            return ReplaceKeyword(value, lines);
        }

        private string InjectCall(CollectionDescriptor collectionDescriptor)
        {
            return string.Format("{0} = new DataCollection<{1}>();\n", collectionDescriptor.memberInfo.Name, collectionDescriptor.subType.Name);
        }
    }
}