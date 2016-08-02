// CORE - Copyright 2007 HumaNature Studios Inc.

#ifndef _WYT_NAVBAR_H_
#define _WYT_NAVBAR_H_

#include "core.h"

#include "ui\uilayer.h"
#include "ui\uiimage.h" // TODO KILL ME KILL ME PLEASE!
#include "superenum.h"

#include "renderer/nitroimage2d.h"
#include "renderer/nitroanimation2d.h"

namespace core {

#define NB_NUMWIGGLEFRAMES (3)
#define NB_ENABLED (0x1)
#define NB_VISIBLE (0x2)
#define NB_ACTIVE  (0x4)

class UINavbar:
    public UILayer
{
public:

    virtual void Initialize(XmlElement* element);

    virtual void Begin();

    virtual void Enter();

    virtual void Render();

    virtual void Leave();

    virtual void End();

    virtual bool OnTouch(int x, int y);

    void SetEnabled(UILayer::NavbarComponent component, bool bEnabled);
    void SetVisible(UILayer::NavbarComponent component, bool bVisible);
    void SetActive( UILayer::NavbarComponent component, bool bActive);

    void SetPosition(UILayer::NavbarComponent component, int x, int y);
    void GetPosition(UILayer::NavbarComponent component, int& x, int& y);

    // same as calling SetEnabled, SetVisible, SetActive all in one.
    void TurnOn(UILayer::NavbarComponent component, bool turnOn = true);

    void UncacheResources();

    void DeactivateAll();

protected:    
    int m_xPositions[UILayer::NavbarComponent_Total];
    int m_yPositions[UILayer::NavbarComponent_Total];
    int m_Widths[UILayer::NavbarComponent_Total];
    int m_Heights[UILayer::NavbarComponent_Total];
    int m_SoundIDs[UILayer::NavbarComponent_Total];

    NitroAnimation2D::Instance m_AnimInstance;
    NitroAnimation2D m_Animation;
    NitroImage2D m_Image[NB_NUMWIGGLEFRAMES];    
    NitroPalette2D m_Palette;

    char m_Flags[UILayer::NavbarComponent_Total];
};

} // namespace core

#endif // _WYT_NAVBAR_H_