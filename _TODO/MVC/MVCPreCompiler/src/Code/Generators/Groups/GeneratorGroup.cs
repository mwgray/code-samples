using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using Graymatter.Common.Extensions;

namespace Graymatter.Code.Generators.Groups
{
    public abstract class GeneratorGroup
    {
        protected string keyword;

        protected GeneratorGroup()
        {
            keyword = "{" + GetType().Name.Replace("GeneratorGroup", "") + "}";
        }

        protected static IEnumerable<T> GetGeneratorGroups<T>()
        {
            return Assembly.GetExecutingAssembly().GetTypes().Where(c => !c.IsAbstract && c.IsSubclassOf(typeof(T))).Select(Activator.CreateInstance).Cast<T>();
        }

        protected string ReplaceKeyword(string value, string newValue)
        {
            if(!value.Contains(keyword))
            {
                return value;
            }

            var indentEnd = value.IndexOf(keyword, StringComparison.Ordinal);
            var indentStart = indentEnd;
            while(indentStart > 1)
            {
                if(value[indentStart - 1] == '\n')
                {
                    break;
                }

                indentStart--;
            }

            var indent = value.Substr(indentStart, indentEnd);

            return Regex.Replace(value, indent + keyword + "( *\n)+", string.IsNullOrEmpty(newValue) ? "" : newValue.Tabulate(indent) + "\n");
        }
    }
}