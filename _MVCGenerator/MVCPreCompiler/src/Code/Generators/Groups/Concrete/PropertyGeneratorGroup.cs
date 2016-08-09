using System;
using System.Collections.Generic;
using System.Linq;
using Graymatter.Code.Attributes;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Member;
using Graymatter.Properties;

namespace Graymatter.Code.Generators.Groups.Concrete
{
    public class PropertiesGeneratorGroup: DataGeneratorGroup
    {
        public override string Inject(string value, ClassDescriptor classDescriptor)
        {
            var fields = classDescriptor.fields.Where(p => p.fieldInfo.DeclaringType == classDescriptor.type);

            var lines = fields.GroupBy(CalculatedPropertyType, f => f)
                              .Aggregate("", (current, grouping) => grouping.ToList()
                                                                            .Aggregate(current, (l, f) => l + GenerateProperty(grouping.Key, f)));

            return ReplaceKeyword(value, lines);
        }

        private string GenerateProperty(PropertyType propertyType, FieldDescriptor fieldDescriptor)
        {
            var template = _propertyTemplates[propertyType];

            var fieldType = fieldDescriptor.fieldInfo.FieldType;
            var type = fieldType.Name;
            var name = fieldDescriptor.fieldInfo.Name;

            var isReference = fieldDescriptor.HasAttribute<ReferenceAttribute>();
            var needsConstructor = fieldType == typeof(string) || fieldType.IsPrimitive || fieldType.IsEnum || fieldType.IsValueType || isReference;
            var constructor = needsConstructor ? "" : string.Format("{0}new {1}()", propertyType == PropertyType.Simple ? " = " : "", type);

            if(isReference)
            {
                template = "[InspectorCollapsedFoldout] " + template;
            }

            return template
                .Replace("$Name$", name)
                .Replace("$Type$", type)
                .Replace("$Constructor$", constructor);
        }

        private enum PropertyType
        {
            Simple,
            Calculated,
            Observed,
            ReadOnlyObserved,
        }

        private readonly Dictionary<PropertyType, string> _propertyTemplates = new Dictionary<PropertyType, string>
                                                                               {
                                                                                   { PropertyType.Simple, Resources.SimpleProperty },
                                                                                   { PropertyType.Calculated, Resources.CalculatedProperty },
                                                                                   { PropertyType.Observed, Resources.ObservedProperty },
                                                                                   { PropertyType.ReadOnlyObserved, Resources.ReadOnlyObservedProperty },
                                                                               };

        private PropertyType CalculatedPropertyType(FieldDescriptor fieldDescriptor)
        {
            if(fieldDescriptor.dependencies.Count > 0)
            {
                return PropertyType.Calculated;
            }

            var isObserved = fieldDescriptor.HasDependents || fieldDescriptor.HasAttribute<ObservedAttribute>();

            if(isObserved)
            {
                var isReadOnly = fieldDescriptor.HasAttribute<NonSerializedAttribute>();

                return isReadOnly ? PropertyType.ReadOnlyObserved : PropertyType.Observed;
            }

            return PropertyType.Simple;
        }
    }
}