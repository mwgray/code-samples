// CORE - Copyright 2007 HumaNature Studios Inc.

#include "ui/navbar.h"

#include "ui\uimanager.h"
#include "debug/convar.h"
#include "wytrenderer.h"
#include "wyttypes.h"
#include "memory\frameallocator.h"
#include "audio\nitrosfxdriver.h"
#include "audio\WYT_music.sadl"
#include "consolecommand.h"

namespace core{


void UINavbar::Initialize(XmlElement* element)
{
    UILayer::Initialize(element);

    std::string path = "ui/all/controls/nav_buttons_@"; // @ gets overwritten
    std::string::size_type ovrwrtPos = path.length() - 1;

    XmlElement* pElem;

    for (unsigned char i = 0; i < UILayer::NavbarComponent_Total; ++i)
    {
        pElem = i == 0 ? element->FirstChildElement("Button") : pElem->NextSiblingElement("Button");

        std::string name = pElem->GetAttribute("name");
        std::string component = NavbarComponentToString(i);

        hdAssert(component == name);

        GetElementValue(pElem, "x", m_xPositions[i]);
        GetElementValue(pElem, "y", m_yPositions[i]);
        GetElementValue(pElem, "w", m_Widths[i]);
        GetElementValue(pElem, "h", m_Heights[i]);

        if (!GetElementValue(pElem, "SFX", m_SoundIDs[i]))
            m_SoundIDs[i] = SFX_MenuSelect01;
    }

    for (unsigned char i = 0; i < NB_NUMWIGGLEFRAMES; ++i)
    {
        NitroImage2D& image = m_Image[i];

        path[ovrwrtPos] = i + '0';

        image.Setup(path, NitroSystemMain);
 
        u32 offset = wyt::Renderer::Instance().Allocate(NitroSystemMain, image.GetVRAMSize());
        image.SetOffset(offset);
        image.Cache();
    }

    NitroPalette2D& palette = m_Palette;

    m_Animation.Load(path.c_str(), &m_Image[0], NULL);

    // Since the navbar and check circles use the same palette,
    //  shouldn't have to worry about offset here.
    palette.Setup("ui/all/controls/circles", MainObjectPalette);
    palette.Cache();

    // All three anims should be the same, just use this one.
    //m_AnimInstance.InitAnim(&m_Animation, 0);

    for (int i = 0; i < UILayer::NavbarComponent_Total; ++i)
        m_Flags[i] = 0;
}

void UINavbar::Begin()
{
    for (int i = 0; i < UILayer::NavbarComponent_Total; ++i)
    {
        char& flag = m_Flags[i];
        
        if (flag & NB_ACTIVE)
            flag |= NB_VISIBLE;
        
        flag &= ~NB_ENABLED;
    }

    UncacheResources();

    UILayer::Begin();
}

void UINavbar::Enter()
{
    for (int i = 0; i < UILayer::NavbarComponent_Total; ++i)
    {
        char& flag = m_Flags[i];
        
        if (flag & NB_ACTIVE)
        {
            flag |= NB_VISIBLE;
            flag |= NB_ENABLED;
        }
    }

    UILayer::Enter();
}

void UINavbar::Render()
{
    UIComponent::Render();

    int wiggleFrame = WiggleFrame::Instance().GetWiggleFrame();
    m_Animation.SetNitroImage2D(&m_Image[wiggleFrame]);

    static NNSG2dRendererInstance* pRenderer = NitroRenderer::Instance().GetRendererInstance();

    m_Palette.Cache();
    for(int i = 0; i < NB_NUMWIGGLEFRAMES; ++i)
    {
        m_Image[i].Cache();
    }

    NNS_G2dBeginRendering(pRenderer);

    for (int i = 0; i < UILayer::NavbarComponent_Total; ++i)
    {        
        if (!(m_Flags[i] & NB_VISIBLE))
            continue;

        m_AnimInstance.InitAnim(&m_Animation, i != 1 ? i : 3);
        m_AnimInstance.Render(FX32i(m_xPositions[i]), FX32i(m_yPositions[i]));
    }

    NNS_G2dEndRendering();
}

void UINavbar::Leave()
{
    for(int i = 0; i < UILayer::NavbarComponent_Total; ++i)
    {
        char& flag = m_Flags[i];
        flag &= ~NB_ENABLED;
        flag &= ~NB_ACTIVE;
    }

    UILayer::Leave();
}

void UINavbar::End()
{
    for(int i = 0; i < UILayer::NavbarComponent_Total; ++i)
        m_Flags[i] = 0;

    UILayer::End();
}

bool UINavbar::OnTouch(int x, int y)
{
    for (int i = 0; i < UILayer::NavbarComponent_Total; ++i)
    {
        char flag = m_Flags[i];

        if (!(flag & NB_ENABLED))
            continue;

        UIManager& manager = UIManager::Instance();
        NitroSfxDriver& sfx = NitroSfxDriver::Instance();

        int leftX   = m_xPositions[i];
        int rightX  = leftX + m_Widths[i];
        int topY    = m_yPositions[i];
        int bottomY = topY + m_Heights[i];

        if (x < leftX || x > rightX || y < topY || y > bottomY)
            continue;

        hdLog(("UI", "Navbar button %s playing sound %i\n", UILayer::NavbarComponentToString(i), m_SoundIDs[i]));

        sfx.PlaySound(&manager, m_SoundIDs[i]);
        manager.OnUIEvent(NavbarComponentToString(i));
        return true;
    }

    return false;
}

void UINavbar::SetEnabled(UILayer::NavbarComponent component, bool bEnabled)
{
    if (bEnabled)
        m_Flags[component] |= NB_ENABLED;
    else
        m_Flags[component] &= ~NB_ENABLED;
}

void UINavbar::SetVisible(UILayer::NavbarComponent component, bool bVisible)
{
    if (bVisible)
        m_Flags[component] |= NB_VISIBLE;
    else
        m_Flags[component] &= ~NB_VISIBLE;
}

void UINavbar::SetActive(UILayer::NavbarComponent component, bool bActive)
{
    if (bActive)
        m_Flags[component] |= NB_ACTIVE;
    else
        m_Flags[component] &= ~NB_ACTIVE;
}

void UINavbar::SetPosition(UILayer::NavbarComponent component, int x, int y)
{
    m_xPositions[component] = x;
    m_yPositions[component] = y;
}

void UINavbar::GetPosition(UILayer::NavbarComponent component, int& x, int& y)
{
    x = m_xPositions[component];
    y = m_yPositions[component];
}

void UINavbar::TurnOn(UILayer::NavbarComponent component, bool turnOn /* = true */)
{
    m_Flags[component] = turnOn ? 0x7 : 0;
}

void UINavbar::UncacheResources()
{
    m_Palette.Uncache();
    for(int i = 0; i < NB_NUMWIGGLEFRAMES; ++i)
    {
        m_Image[i].Uncache();
    }
}
// Replace TurnOn, MakeActive, GetImage

void UINavbar::DeactivateAll()
{
    memset(m_Flags, 0, UILayer::NavbarComponent_Total * sizeof(char));
}

CONSOLE_COMMAND(UncacheNavBar, "", "")
{
    (void)pParameters;
    GetUILayer(UINavbar, "navbar")->UncacheResources();
}

#pragma force_active on
REGISTER_UI_TYPE(UINavbar)
#pragma force_active off

}   // core