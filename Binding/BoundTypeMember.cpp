// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "BoundTypeMember.h"


namespace core {

    template <>
    void BoundTypeMember<bool>::updateValue(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    template <>
    void BoundTypeMember<int>::updateValue(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    template <>
    void BoundTypeMember<float>::updateValue(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    template <>
    void BoundTypeMember<const char>::updateValue(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

} // namespace core