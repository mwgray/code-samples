// CORE - Copyright 2007 HumaNature Studios Inc.

#ifndef _CORE_UITRIPLESCREEN_H_
#define _CORE_UITRIPLESCREEN_H_

#include "ui\uiscreen.h"

namespace core {

class UITripleScreen:
    public UIScreenTmpl<3, 1>
{
public:
    void Tick(Float deltaSeconds);
};

class UINewTripleScreen:
    public UIScreenTmpl<3, 3>
{
public:
    void Tick(Float deltaSeconds);
};

} // namespace core

#endif // _CORE_UITRIPLESCREEN_H_