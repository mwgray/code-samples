// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BOUNDTYPEMEMBER_H__
#define __BOUNDTYPEMEMBER_H__

#include "BoundMember.h"

namespace core {

template <typename t_type>
class BoundTypeMember:
    public BoundMember
{
public:
    BoundTypeMember(t_type& ref)
        : mRef(ref)
      {

      }

      virtual void updateValue(Property* property);

      t_type* getDataPtr() {return &mRef;}

protected:
    t_type& mRef;

    BoundTypeMember& operator=(BoundTypeMember&);
};

} // namespace core

#endif // __BOUNDTYPEMEMBER_H__