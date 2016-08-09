using System;
using System.Linq;
using Graymatter.Code.Descriptors;
using Graymatter.Code.Generators;
using Graymatter.Common.Extensions;
using Graymatter.Common.Logging;

namespace Graymatter
{
    // ReSharper disable once InconsistentNaming
    internal static class MVCPreCompilerMain
    {
        [STAThread]
        private static void Main(string[] args)
        {
            var assemblyDescriptor = new AssemblyDescriptor();
            
            foreach(var propertyDescriptor in assemblyDescriptor.modelDeclarations.Values.SelectMany(classDescriptor => classDescriptor.properties))
            {
                var dependencies = propertyDescriptor.dependencies.Count == 0 ? "" : "Depends on " + propertyDescriptor.dependencies.Select(d => d.owner.type.Name + "." + d.fieldInfo.Name).Join();
                var collectionDependencies = propertyDescriptor.collectionDependencies.Count == 0 ? "" : "Depends on Collection " + propertyDescriptor.collectionDependencies.Select(d => d.collection.fieldInfo.Name + "." + d.subField.Name).Join();
            
                Log.Info("{0}.{1} {2} {3}", propertyDescriptor.owner.type.Name, propertyDescriptor.fieldInfo.Name, dependencies, collectionDependencies);
            }

            new AssemblyGenerator().Generate(assemblyDescriptor);
        }
    }
}