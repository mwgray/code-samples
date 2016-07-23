using System.Linq;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Dependencies;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class FieldSubBindingsGroup: GeneratorGroup
    {
        public string Inject(string value, ClassDescriptor classDescriptor)
        {
            foreach(var fieldDescriptor in classDescriptor.fields)
            {
                keyword = "{" + fieldDescriptor.fieldInfo.Name + "SubBindings}";

                if(value.Contains(keyword))
                {
                    var subFieldDependencies = fieldDescriptor.dependents
                                                              .SelectMany(d => d.dependencies)
                                                              .OfType<SubFieldDependency>()
                                                              .Distinct()
                                                              .ToList();
                    var lines = subFieldDependencies
                        .Aggregate("", (l, f) => l+ string.Format("{0}._{1}Property.Subscribe(_=>{2}());\n", f.dependency.memberInfo.Name, f.subFieldInfo.Name, f.dependent.memberInfo.Name));

                    value = ReplaceKeyword(value, lines);
                }
            }

            return value;
        }
    }
}