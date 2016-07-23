using System;
using System.Reflection;
using Graymatter.Code.Descriptors;
using Graymatter.Code.Generators;
using Graymatter.Common.Logging;

namespace Graymatter
{
    internal static class MainPlatformerDataModel
    {
        [STAThread]
        private static void Main(string[] args)
        {
            string sourceDirectory;
            if(args.Length < 1)
            {
                sourceDirectory = @"C:\_projects\gm\depot\unity\platformer\Assets\_code";

                Log.Warning("No source directory supplied, using {0}", sourceDirectory);
            }
            else
            {
                sourceDirectory = args[0];
            }

            new AssemblyGenerator().Generate(new AssemblyDescriptor(Assembly.GetExecutingAssembly()), sourceDirectory);
        }
    }
}