using System.Linq;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Dependencies;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class InitializeCalculatedValuesGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            var dependentFields = classDescriptor.fields.Where(p => p.dependencies.Count > 0 && p.fieldInfo.DeclaringType == classDescriptor.type);
            var lines = dependentFields.SelectMany(p => p.dependencies).Select(InjectUpdateCall).Distinct();

            return ReplaceKeyword(value, lines.Join("\n"));
        }

        private string InjectUpdateCall(DependencyDescriptor dependencyDescriptor)
        {
            return string.Format("Update{0}();\n", dependencyDescriptor.dependent.memberInfo.Name);
        }
    }
}