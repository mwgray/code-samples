// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __BOUNDMEMBER_H__
#define __BOUNDMEMBER_H__

namespace core {

class Property;

class BoundMember
{
public:
    BoundMember();
    virtual ~BoundMember();

    BoundMember& name(const char* name);
    const char* getName() const {return mName.c_str();}

    BoundMember& comment(const char* comment);
    const char* getComment() const {return mComment.c_str();}

    BoundMember& isBootOnly(bool isBootOnly);

    virtual void updateValue(Property* property) = 0;

protected:
    std::string mName;
    std::string mComment;
    bool mIsBootOnly;
};

} // namespace core

#endif // __BOUNDMEMBER_H__