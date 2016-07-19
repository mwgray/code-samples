using System.Linq;
using System.Text;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class MethodsGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            // dirty is in view already
            var lines = classDescriptor.methods.Where(m => m.methodInfo.Name != "Dirty").Select(GenerateMethod).Join("\n");

            return ReplaceKeyword(value, lines).Trim(' ', '\n', '\r');
        }

        private string GenerateMethod(MethodDescriptor methodDescriptor)
        {
            var methodInfo = methodDescriptor.methodInfo;

            return string.Format("{0} abstract {1} {2}({3});",
                methodInfo.IsPublic ? "public" : "protected",
                methodInfo.ReturnType.Name,
                methodInfo.Name,
                methodInfo.GetParameters().Select(p => p.ParameterType.Name + " " + p.Name + (p.HasDefaultValue ? "=" + (p.DefaultValue ?? "null") : "")).Join());
        }
    }

    public class ControllerMethodsGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            // dirty is in view already
            var dataDescriptor = classDescriptor as DataDescriptor;
            _controllerDescriptor = dataDescriptor == null ? null : dataDescriptor.controllerDescriptor;

            var lines = _controllerDescriptor == null ? "" : _controllerDescriptor.methods.Where(m => m.methodInfo.IsPublic).Select(GenerateMethod).Join("\n");

            return ReplaceKeyword(value, lines).Trim(' ', '\n', '\r');
        }
        
        private ControllerDescriptor _controllerDescriptor;

        private string GenerateMethod(MethodDescriptor methodDescriptor)
        {
            var methodInfo = methodDescriptor.methodInfo;

            var stringBuilder = new StringBuilder();
            
            stringBuilder.Append("public ");
            stringBuilder.Append(methodInfo.ReturnType.Name + " ");
            stringBuilder.Append(methodInfo.Name);
            stringBuilder.Append("(");
            stringBuilder.Append(methodInfo.GetParameters().Select(p => p.ParameterType.Name + " " + p.Name + (p.HasDefaultValue ? "=" + (p.DefaultValue ?? "null") : "")).Join());
            stringBuilder.Append(")");
            stringBuilder.Append(" { ");

            if(methodInfo.ReturnType != typeof(void))
            {
                stringBuilder.Append("return ");
            }

            var dataName = _controllerDescriptor.dataDescriptor.type.Name;
            var passedParameters = new[] {string.Format("({0})this", dataName)}.Concat(methodInfo.GetParameters().Select(p=>p.Name)).Join();

            stringBuilder.Append(string.Format("new {0}Controller().{1}({2});", dataName, methodInfo.Name, passedParameters));

            stringBuilder.Append(" }");

            return stringBuilder.ToString();
        }

        // public bool AddHalfHeart(HeartType heartType) { return new AvatarHealthController().AddHalfHeart((AvatarHealth)this, heartType); }
    }
}