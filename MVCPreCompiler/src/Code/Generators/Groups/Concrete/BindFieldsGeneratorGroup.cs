using System.Linq;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Dependencies;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class BindFieldsGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            var dependents = classDescriptor.fields.Where(p => p.dependencies.Count > 0 && p.fieldInfo.DeclaringType == classDescriptor.type);
            var lines = dependents.Select(p => p.dependencies.OfType<FieldDependency>().Select(d => InjectTemplate(p, d)).Join("\n") + "\n");

            return ReplaceKeyword(value, lines.Join("\n"));
        }

        private string InjectTemplate(FieldDescriptor fieldDescriptor, FieldDependency fieldDependency)
        {
            const string template = "_{0}Property.Subscribe(_ => Update{1}());";

            return string.Format(template, fieldDependency.dependency.fieldInfo.Name, fieldDescriptor.fieldInfo.Name);
        }
    }
}