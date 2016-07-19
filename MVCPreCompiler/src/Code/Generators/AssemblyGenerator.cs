using System;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using Graymatter.Code.Descriptors;
using Graymatter.Code.Generators.Groups;
using Graymatter.Common.Extensions;
using Graymatter.Common.Logging;
using Graymatter.Properties;

namespace Graymatter.Code.Generators
{
    public class AssemblyGenerator
    {
        public void Generate(AssemblyDescriptor assemblyDescriptor, string sourceDirectory)
        {
            var namespaceGroups = assemblyDescriptor.classDescriptors.Values.GroupBy(d => d.type.Namespace, d => d);

            foreach(var namespaceGroup in namespaceGroups)
            {
                var classDescriptors = namespaceGroup.ToList();

                if(classDescriptors.Count == 0)
                {
                    continue;
                }

                var Namespace = classDescriptors[0].type.Namespace;

                var generatedClassesFile = Resources.GeneratedClassesTemplate
                                                    .Replace("$Namespace$", Namespace);

                generatedClassesFile = new ClassesGeneratorGroup().Inject(generatedClassesFile, classDescriptors);

                generatedClassesFile = generatedClassesFile
                    .Replace("Declaration", "")
                    .Replace("Int32", "int")
                    .Replace("Void", "void")
                    .Replace("String", "string")
                    .Replace("Boolean", "bool");

                generatedClassesFile = Regex.Replace(generatedClassesFile, @" +\n", "\n");

                var namespaceStrings = Namespace.Split('.');
                var domain = namespaceStrings[namespaceStrings.Length - 2];
                var type = namespaceStrings[namespaceStrings.Length - 1];

                var outputDirectory = string.Format(@"{0}\{1}", sourceDirectory, Namespace.Replace(".", "\\"));
                var generatedClassesFilePath = string.Format("{0}\\$Generated.{1}.{2}.cs", outputDirectory, domain, type);

                Log.Info("Generating {0}", Namespace);

                Directory.CreateDirectory(outputDirectory);

                if(File.Exists(generatedClassesFilePath))
                {
                    var existingText = File.ReadAllText(generatedClassesFilePath);
                    existingText = existingText.Substr(0, existingText.IndexOf("namespace", StringComparison.Ordinal));
                    generatedClassesFile = existingText + generatedClassesFile.Substring(generatedClassesFile.IndexOf("namespace", StringComparison.Ordinal));
                }

                WriteFile(generatedClassesFilePath, generatedClassesFile);

                // need to create stub classes
                foreach(var classDescriptor in classDescriptors)
                {
                    var className = classDescriptor.ClassName;

                    var classFile = Resources.ClassTemplate
                                             .Replace("$Namespace$", Namespace)
                                             .Replace("$ClassName$", className);

                    var classFilePath = string.Format("{0}\\{1}.cs", outputDirectory, className);
                    if(!File.Exists(classFilePath))
                    {
                        WriteFile(classFilePath, classFile);
                    }
                }
            }
        }

        private void WriteFile(string path, string contents)
        {
            if(!File.Exists(path) || File.ReadAllText(path) != contents)
            {
                File.WriteAllText(path, contents);
            }
        }
    }
}