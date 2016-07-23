using System;

namespace Graymatter.Code.Descriptors.Class
{
    public class ControllerDescriptor: ClassDescriptor
    {
        public readonly DataDescriptor dataDescriptor;

        public ControllerDescriptor(Type type, DataDescriptor dataDescriptor)
            : base(type)
        {
            this.dataDescriptor = dataDescriptor;

            dataDescriptor.controllerDescriptor = this;
        }
    }
}