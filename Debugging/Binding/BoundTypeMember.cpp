// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "BoundTypeMember.h"


namespace core {

    // TODO- move this back to the h file when the Property hierarchy has cooled off
    template <>
    void BoundTypeMember<bool>::updateValue(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    // TODO- move this back to the h file when the Property hierarchy has cooled off
    template <>
    void BoundTypeMember<int>::updateValue(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

    // TODO- move this back to the h file when the Property hierarchy has cooled off
    template <>
    void BoundTypeMember<float>::updateValue(Property* property)
    {
        property->read(mName.c_str(), mRef);
    }

} // namespace core