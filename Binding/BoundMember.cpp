// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "BoundMember.h"

namespace core {

    BoundMember::BoundMember()
        : mIsBootOnly(false)
    {

    }

    BoundMember::~BoundMember()        
    {

    }

    BoundMember& BoundMember::name(const char* name)
    {
        mName = name;

        return *this;
    }

    BoundMember& BoundMember::comment(const char* comment)
    {
        mComment = comment;

        return *this;
    }

    BoundMember& BoundMember::isBootOnly(bool isBootOnly)
    {
        mIsBootOnly = isBootOnly;

        return *this;
    }

} // namespace core