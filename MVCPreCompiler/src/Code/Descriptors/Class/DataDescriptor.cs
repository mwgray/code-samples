using System;

namespace Graymatter.Code.Descriptors.Class
{
    public class DataDescriptor: ClassDescriptor
    {
        public ControllerDescriptor controllerDescriptor;

        public DataDescriptor(Type type)
            : base(type)
        {
        }
    }
}