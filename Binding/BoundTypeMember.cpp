// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "BoundTypeMember.h"


namespace core {

    // where most of the magic happens, the value is updated from the property.  each one of these just references the
    // template read from the Property class

    template <>
    void BoundTypeMember<bool>::updateValueFromProperty(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    template <>
    void BoundTypeMember<int>::updateValueFromProperty(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    template <>
    void BoundTypeMember<float>::updateValueFromProperty(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    template <>
    void BoundTypeMember<const char>::updateValueFromProperty(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

} // namespace core