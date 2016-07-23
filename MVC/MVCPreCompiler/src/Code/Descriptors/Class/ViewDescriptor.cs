using System;

namespace Graymatter.Code.Descriptors.Class
{
    public class ViewDescriptor: ClassDescriptor
    {
        public readonly DataDescriptor dataDescriptor;

        public ViewDescriptor(Type type, DataDescriptor dataDescriptor)
            : base(type)
        {
            this.dataDescriptor = dataDescriptor;
        }
    }
}