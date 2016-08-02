// CORE - Copyright 2007 HumaNature Studios Inc.

#include "ui\uitriplescreen.h"
#include "ui\uimanager.h"
#include "wyttypes.h"

namespace core {

void UITripleScreen::Tick(Float deltaSeconds)
{
    UIScreenTmpl<3, 1>::Tick(deltaSeconds);    

    int wiggleFrame = WiggleFrame::Instance().GetWiggleFrame();
    
    if(VRAMIsMain(m_NitroImage2D[0].GetVramLocation()))
    {
        G2_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_512x512, GX_BG_COLORMODE_256, 
            GXBGScrBase(GX_BG_SCRBASE_0x1000 + 2 * wiggleFrame), 
            GX_BG_CHARBASE_0x10000);
    }
    else
    {
        G2S_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_512x512, GX_BG_COLORMODE_256, 
            GXBGScrBase(GX_BG_SCRBASE_0x1000 + 2 * wiggleFrame), 
            GX_BG_CHARBASE_0x10000);
    }
}

void UINewTripleScreen::Tick(Float deltaSeconds)
{
    UIScreenTmpl<3, 3>::Tick(deltaSeconds);

    int wiggleFrame = WiggleFrame::Instance().GetWiggleFrame();    

    if(VRAMIsMain(m_NitroImage2D[0].GetVramLocation()))
    {
        G2_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_512x512, GX_BG_COLORMODE_256, 
            GXBGScrBase(GX_BG_SCRBASE_0x1000 + 2 * wiggleFrame), 
            GXBGCharBase(GX_BG_CHARBASE_0x10000 + 3 * wiggleFrame));
    }
    else
    {
        G2S_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_512x512, GX_BG_COLORMODE_256, 
            GXBGScrBase(GX_BG_SCRBASE_0x1000 + 2 * wiggleFrame), 
            GXBGCharBase(GX_BG_CHARBASE_0x10000 + 3 * wiggleFrame));
    }
}

#pragma force_active on
REGISTER_UI_TYPE(UITripleScreen)
#pragma force_active off

#pragma force_active on
REGISTER_UI_TYPE(UINewTripleScreen)
#pragma force_active off

} // namespace core