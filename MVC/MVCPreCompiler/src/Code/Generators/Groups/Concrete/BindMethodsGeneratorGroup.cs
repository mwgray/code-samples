using System.Linq;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Dependencies;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class BindMethodsGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            var dependents = classDescriptor.methods.Where(p => p.dependencies.Count > 0 && p.methodInfo.DeclaringType == classDescriptor.type);
            var lines = dependents.Select(p => p.dependencies.OfType<FieldDependency>().Select(d => InjectTemplate(p, d)).Join("\n") + "\n");

            return ReplaceKeyword(value, lines.Join("\n"));
        }

        private string InjectTemplate(MethodDescriptor methodDescriptor, FieldDependency fieldDependency)
        {
            const string template = "_{0}Property.Subscribe(_ => {1}());";

            return string.Format(template, fieldDependency.dependency.fieldInfo.Name, methodDescriptor.methodInfo.Name);
        }
    }
}