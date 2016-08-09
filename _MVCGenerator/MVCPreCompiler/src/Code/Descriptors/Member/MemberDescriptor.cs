using System;
using System.Collections.Generic;
using System.Reflection;
using Graymatter.Code.Attributes;
using Graymatter.Code.Descriptors.Class;
using Graymatter.Code.Descriptors.Dependencies;

namespace Graymatter.Code.Descriptors.Member
{
    public class MemberDescriptor
    {
        public readonly MemberInfo memberInfo;

        public readonly ClassDescriptor owner;

        public readonly List<DependencyDescriptor> dependencies = new List<DependencyDescriptor>();

        public readonly List<MemberDescriptor> dependents = new List<MemberDescriptor>();

        public MemberDescriptor(ClassDescriptor owner, MemberInfo memberInfo)
        {
            this.owner = owner;

            this.memberInfo = memberInfo;
        }

        public bool HasDependents
        {
            get { return dependents.Count > 0; }
        }

        public bool HasAttribute<T>() where T : Attribute
        {
            return memberInfo.GetCustomAttribute<T>() != null;
        }
    }
}