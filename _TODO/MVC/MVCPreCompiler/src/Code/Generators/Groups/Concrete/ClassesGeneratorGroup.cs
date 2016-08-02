using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Generators.Groups.Concrete;
using Graymatter.Common.Extensions;
using Graymatter.Common.Logging;
using Graymatter.Properties;

namespace Graymatter.Code.Generators.Groups
{
    public class ClassesGeneratorGroup: GeneratorGroup
    {
        public string Inject(string value, List<ClassDescriptor> classDescriptors)
        {
            var classes = classDescriptors.OrderBy(m => m.type.Name).Select(GenerateClass).Join("\n");

            return ReplaceKeyword(value, classes);
        }

        private string GenerateClass(ClassDescriptor classDescriptor)
        {
            var dataDescriptor = classDescriptor as DataDescriptor;
            if(dataDescriptor != null)
            {
                return GenerateData(dataDescriptor);
            }

            var viewDescriptor = classDescriptor as ViewDescriptor;
            if(viewDescriptor != null)
            {
                return GenerateView(viewDescriptor);
            }

            var controllerDescriptor = classDescriptor as ControllerDescriptor;
            if(controllerDescriptor != null)
            {
                return GenerateController(controllerDescriptor);
            }

            Log.Warning("Unhandled class type {0}", classDescriptor.GetType().Name);

            return "";
        }

        private string GenerateData(DataDescriptor dataDescriptor)
        {
            var baseClass = dataDescriptor.type.BaseType.Name;

            var classString = Resources.DataBaseClassTemplate
                                       .Replace("$ClassName$", dataDescriptor.type.Name)
                                       .Replace("$BaseClass$", baseClass);

            classString = DataGeneratorGroup.InjectAll(classString, dataDescriptor);

            classString = new FieldSubBindingsGroup().Inject(classString, dataDescriptor);

            // remove empty functions
            classString = Regex.Replace(classString, " +protected.*()\n +{\n +}\n\n", "");

            return dataDescriptor.collections.Aggregate(classString, CollectionGeneratorGroup.InjectAll);
        }

        private string GenerateView(ViewDescriptor viewDescriptor)
        {
            var baseType = viewDescriptor.type.BaseType;
            var baseClass = baseType.Name;
            var dataName = viewDescriptor.dataDescriptor.type.Name;

            var isBase = baseType.GenericTypeArguments.Length > 0;
            if(isBase)
            {
                baseClass = baseClass.Replace("`1", "<" + dataName + ">");
            }


            var classString = Resources.ViewBaseClassTemplate
                                       .Replace("$ClassName$", viewDescriptor.type.Name)
                                       .Replace("$BaseClass$", baseClass)
                                       .Replace("$DataName$", dataName)
                                       .EvaluateConditional("IsBase", isBase);

            return DataGeneratorGroup.InjectAll(classString, viewDescriptor);
        }

        private string GenerateController(ControllerDescriptor controllerDescriptor)
        {
            var baseType = controllerDescriptor.type.BaseType;
            var baseClass = baseType.Name;
            var dataName = controllerDescriptor.dataDescriptor.type.Name;

            var isBase = baseType.GenericTypeArguments.Length > 0;
            if(isBase)
            {
                baseClass = baseClass.Replace("`1", "<" + dataName + ">");
            }

            var classString = Resources.ControllerBaseClassTemplate
                                       .Replace("$ClassName$", controllerDescriptor.ClassName)
                                       .Replace("$BaseClass$", baseClass)
                                       .Replace("$DataName$", dataName);

            classString = DataGeneratorGroup.InjectAll(classString, controllerDescriptor);

            classString = classString.Replace("(", string.Format("({0} {1}, ", controllerDescriptor.dataDescriptor.type.Name, controllerDescriptor.dataDescriptor.type.Name.ToLowerFirstCharacter()))
                                     .Replace(", )", ")");

            return classString;
        }
    }
}