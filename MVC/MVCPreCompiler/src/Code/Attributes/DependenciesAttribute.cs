using System;

namespace Graymatter.Code.Attributes
{
    public class DependenciesAttribute: Attribute
    {
        public string[] dependencies;

        public DependenciesAttribute(params string[] dependencies)
        {
            this.dependencies = dependencies;
        }
    }
}