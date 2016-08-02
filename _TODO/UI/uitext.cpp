// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui/uitext.h"
#include "ui/uimanager.h"

#include "font/nitrofont.h"
#include "font/fontcanvas.h"
#include "debug\convar.h"

#include "camera\camera.h"
#include "debug/debugboxdrawer.h"

namespace core {

ConVar<int>  g_sMaxResizeFontAttempts("UIText::g_sMaxResizeFontAttempts", 20, "max number of times we will reduce the size of a font before asserting");
ConVar<int>  g_sResizeDecrement("UIText::g_sResizeDecrement", 2, "the amount we decrement the size by when resizing (currSize - g_sResizeDecrement = newSize)");
ConVar<int>  g_sDisableFontResizing("UIText::g_sDisableFontResizing", false, "if true, fonts will NOT RESIZE AT ALL");
ConVar<bool> g_sDrawTextBoundingBoxes("UIText::g_sDrawTextBoundingBoxes", false, "if true, BBs representing text XWrap and desired height will be drawn");

FontCanvas* UIText::m_spFontCanvas = NULL;

static const int g_skMinimumFontSize = 10;

#ifndef __HNS_FINAL__
bool UIText::m_sMemOverride = false;
#endif    

UIText::UIText() :
m_HJust(Left),
m_VJust(Top),
m_FGColor(Black),
m_BGColor(White),
m_PaletteIndex(0),
m_XWrap(0),
m_DesiredHeight(0),
m_Lines(0),
m_pFont(NULL),
m_IsTextBiggerThanScreenVertically(false),
m_ForceOneLine(false),
m_DisableWrappingAndResizing(false)
{
    MEMORY_POOL("UIText");


}

UIText::~UIText()
{
    MEMORY_POOL("UIText");

    if (m_pFont)
        NitroFont::FreeFont(m_pFont->GetFontPath());
}

void UIText::Initialize(XmlElement* element)
{
    MEMORY_POOL("UIText");

    UIComponent::Initialize(element);

    if(!GetElementValue(element, "Font", m_FontName))
    {
        hdWarning(("UIText", "NO FONT SPECIFIED FOR %s\n", m_Name.c_str()));
    }

    if(!GetElementValue(element, "MinimumFont", m_MinimumFontName))
    {
        char minFontName[256];
        sprintf(minFontName, "a3_%i_16", g_skMinimumFontSize);
        m_MinimumFontName = minFontName;
    }
    
    if (m_FontName == "")
        hdWarning(("UIText", "Font name not acquired in UIText::Initialize!\n\n"));

    std::string hJust = "Left";
    std::string vJust = "Top";
    GetElementValue(element, "HorizontalJustification", hJust);
    GetElementValue(element, "VerticalJustification", vJust);

    m_HJust = JustificationFromString(hJust, true);
    m_VJust = JustificationFromString(vJust, true);

    char r = 0, g = 0, b = 0, a = 0;
    GetElementValue(element, "r", r);
    GetElementValue(element, "g", g);
    GetElementValue(element, "b", b);
    GetElementValue(element, "a", a);
    m_FGColor = makeColor(r, g, b, a);

    r = 31; g = 31; b = 31; a = 1;

    GetElementValue(element, "bg_r", r);
    GetElementValue(element, "bg_g", g);
    GetElementValue(element, "bg_b", b);
    GetElementValue(element, "bg_a", a);

    m_BGColor = makeColor(r, g, b, a);

    GetElementValue(element, "PaletteIndex", m_PaletteIndex);
    GetElementValue(element, "XWrap", m_XWrap);
    GetElementValue(element, "DesiredHeight", m_DesiredHeight);

    GetElementValue(element, "Text", m_Text);
    m_OriginalText = m_Text;

    int nForceOneLine = 0;
    if(GetElementValue(element, "ForceOneLine", nForceOneLine))
    {
        m_ForceOneLine = (nForceOneLine == 1) ? true : false;
    }

    int nDisableWrapping = 0;
    if(GetElementValue(element, "DisableWrappingAndResizing", nDisableWrapping))
    {
        m_DisableWrappingAndResizing = (nDisableWrapping == 1) ? true : false;
    }
    // if not explicitly set, calculate whether we should wrap
    else if(m_XWrap == 0)
    {
        m_DisableWrappingAndResizing = true;
    }
}

void UIText::End()
{
    MEMORY_POOL("UIText");

    UIComponent::End();    
    if (m_pFont)
    {
        NitroFont::FreeFont(m_pFont->GetFontPath());
        m_pFont = NULL;
    }
}


void UIText::Shutdown()
{
    MEMORY_POOL("UIText");

    UIComponent::Shutdown();
}

bool UIText::OnTouch(int x, int y)
{
    MEMORY_POOL("UIText");

    return UIComponent::OnTouch(x, y + 192);
}

void UIText::SetFont( u8 fontSize )
{
    char fontName[64];
    sprintf(fontName, "a3_%i_16", fontSize);
    
    SetFont(fontName);
}

void UIText::SetFont(const char* pFontPath)
{
    MEMORY_POOL("UIText");

    m_CurrentFontName = pFontPath;
    m_FontName = pFontPath;

    // don't set font again if it's the current font
    std::string path = "$fonts/";
    path += pFontPath;
    path += ".nftr";

    // setup font
    Font* pDesiredFont = NitroFont::GetFont(path.c_str());
 
    if (m_pFont)
        NitroFont::FreeFont(m_pFont->GetFontPath());

    m_pFont = pDesiredFont;

    if (m_Text != "")
        m_Text = m_OriginalText; //make sure we're dealing with a clean bit of text
}

void UIText::Render()
{
    MEMORY_POOL("UIText");
    
    UIComponent::Render();

#ifndef __HNS_FINAL__
    {
        static bool was = m_sMemOverride;
        if(was != m_sMemOverride) {
            was = m_sMemOverride;

            char avail[10];
            sprintf(avail, "%d", PlatformGetAvailableMem());
            m_Text = avail;        
            SetText(m_Text.c_str(), true);
        }
    }
#endif    
            

    if(m_PaletteDirty)
    {
        const char* pFont = GetCurrentFontName();
        u8 fontSize = pFont[3] - '0';
        if (pFont[4] != '_')
        {
            fontSize *= 10;
            fontSize += pFont[4] - '0';
        }

        SetupPalette(fontSize <= 16);
    }    

    FontCanvas *pFontCanvas = GetFontCanvas();
    if(pFontCanvas)
    {
        if (!m_pFont)    
            ResizeFont();

        pFontCanvas->Draw(m_pFont, x, y, m_Text.c_str(), m_VJust, m_HJust, m_PaletteIndex * 16 + 1);
    }
    
    if(g_sDrawTextBoundingBoxes)
    {
        RenderWrappingBoundingBox();
    }
}

void UIText::RenderWrappingBoundingBox()
{
    Float3 tl, br;
    Vector<Float, 3> tlBB, brBB;
    int t,l,b,r;
    int localX, localY;

    // we might need the actual text width and height
    if(m_HJust == Center)
    {
        localX = -m_XWrap / 2;
    }
    else if(m_HJust == Right)
    {
        localX = -m_XWrap;
    }    
    else
    {
        localX = 0;
    }

    if(m_VJust == Center)
    {
        localY = -m_DesiredHeight / 2;
    }
    else if(m_VJust == Bottom)
    {
        localY = -m_DesiredHeight;
    }
    else
    {
        localY = 0;
    }

    //GetTopLeftPosition(l, t);
    l = localX + x;
    t = localY + y;
    b = t + m_DesiredHeight;
    r = l + m_XWrap;
    tl = Camera::Instance().TouchScreenToWorld(l, t); tlBB.x = tl.x; tlBB.y = tl.y; tlBB.z = tl.z;
    br = Camera::Instance().TouchScreenToWorld(r, b); brBB.x = br.x; brBB.y = br.y; brBB.z = br.z;

    DebugBoxDrawer::Instance().DrawBox(tl, br - tl, Red);
}

void UIText::SetText(const char* pString, bool settingNewText)
{
    MEMORY_POOL("UIText");

    hdLog(("Font", "SetText(pString=%s, settingNewText=%s) : %s\n", pString, settingNewText ? "true" : "false", m_Name.c_str()));
    hdLog(("Font", "m_pFont = %X\n", m_pFont));
    
    if (m_pFont)
    {
        // ensures the font will start at the desired size
        if(settingNewText)
        {
            m_OriginalText = pString;
            SetFont(m_FontName.c_str());
        }

        m_Text = pString;

        if(!m_ForceOneLine && !m_DisableWrappingAndResizing)
        {
            m_Text = m_pFont->WrapText(m_Text, m_XWrap, &m_Lines);
        }

        if(settingNewText)
        {
            ResizeFont();
        }

        m_IsTextBiggerThanScreenVertically = m_spFontCanvas->GetStringHeight(m_pFont, pString, m_XWrap, &m_Lines) > 192;
    }
    
    else
    {
        m_OriginalText = m_Text = pString;
        m_Lines = 1;
        ResizeFont();
    }
}

const char* UIText::GetText( void )
{
    MEMORY_POOL("UIText");

    return m_Text.c_str();
}

void UIText::SetupPalette(bool darken)
{
    MEMORY_POOL("UIText");

    // setup the palette
    static u16 palette[16];
    for(int i = 0; i < 16; ++i)
    {
        fx32 alpha = FX32f(float(i) / 15.0f);
        if (darken)
            alpha = FX_Mul(alpha, alpha);

        palette[i] = InterpolateColor(m_BGColor, m_FGColor, FX32f(float(i) / 15.0f));        
    }

    DC_FlushRange(palette, sizeof(palette));
    
    GX_LoadBGPltt (palette, m_PaletteIndex * 16 * sizeof(u16), sizeof(palette));    
    GXS_LoadBGPltt(palette, m_PaletteIndex * 16 * sizeof(u16), sizeof(palette));    

    m_PaletteDirty = false;
}


void UIText::Begin()
{
    MEMORY_POOL("UIText");

    SetFont(m_FontName.c_str());
    SetText(m_Text.c_str());

    UIComponent::Begin();
}

void UIText::Enter()
{
    MEMORY_POOL("UIText");

    UIComponent::Enter();

    m_PaletteDirty = true;
}

void UIText::SetFontCanvas(FontCanvas* pFontCanvas) 
{
    MEMORY_POOL("UIText");

    
    if (m_spFontCanvas != NULL)
        FreeFontCanvas();
    
    m_spFontCanvas = pFontCanvas;
}


FontCanvas* UIText::GetFontCanvas()
{
    MEMORY_POOL("UIText");
    
    return m_spFontCanvas;
}

void UIText::FreeFontCanvas()
{
    MEMORY_POOL("UIText");

    if (m_spFontCanvas != NULL)
    {
        delete m_spFontCanvas;
        m_spFontCanvas = NULL;
    }
}

void UIText::FreeFont()
{
    MEMORY_POOL("UIText");

    if (m_pFont)
    {
        NitroFont::FreeFont(m_pFont->GetFontPath());
        m_pFont = NULL;
    }
}

core::Color UIText::GetColor() const
{
    MEMORY_POOL("UIText");

    return m_FGColor;
}

core::Color UIText::GetBGColor() const
{
    MEMORY_POOL("UIText");

    return m_BGColor;
}

void UIText::SetColor(Color val)
{
    MEMORY_POOL("UIText");

    if(m_FGColor != val)
    {
        m_PaletteDirty = true;
        m_FGColor = val;
    }
}

void UIText::SetBGColor(Color val)
{
    MEMORY_POOL("UIText");

    if (m_BGColor != val)
    {
        m_PaletteDirty = true;
        m_BGColor = val;
    }
}

void UIText::SetPaletteDirty(bool dirty)
{
    m_PaletteDirty = dirty;
}

void UIText::GetTextDimenstions(int& w, int& h)
{
    MEMORY_POOL("UIText");

    if(!GetFontCanvas() || !m_pFont) {
        w = h = 0;        
        return;    
    }    
    w = GetWidth();
    h = GetHeight();
}

void UIText::GetTopLeftPosition(int& x, int& y)
{
    MEMORY_POOL("UIText");

    int w,h;
    GetTextDimenstions(w, h);

    switch(m_HJust)
    {
    case Left:
        x = GetAbsoluteX();
        break;
    case Center:
        x = GetAbsoluteX() - (w >> 1);
    	break;
    case Right:
        x = GetAbsoluteX() - w;
        break;

    // TODO jr mg: UNIMPLEMENTED
    case Front:
    case Back:
        break;
    }

    switch(m_VJust)
    {
    case Top:
        y = GetAbsoluteY();
        break;
    case Center:
        y = GetAbsoluteY() - (h >> 1);
        break;
    case Bottom:
        y = GetAbsoluteY() - h;
        break;

        // TODO jr mg: UNIMPLEMENTED
    case Front:
    case Back:
        break;
    }
}

void UIText::GetBounds(int& t, int& l, int& b, int& r)
{
    MEMORY_POOL("UIText");

    int w,h;
    GetTextDimenstions(w, h);
    GetTopLeftPosition(l, t);

    b = t + static_cast<int>(h);
    r = l + static_cast<int>(w);
}

void UIText::OnTouchMoved(int tx, int ty)
{
    MEMORY_POOL("UIText");

    UIComponent::OnTouchMoved(tx, ty + 192);
}

void UIText::Clear( CanvasRange screen /*= BothScreens*/ )
{
    MEMORY_POOL("UIText");

    hdAssert(m_pFont);

    Float start = 0, end = 0;
    switch(screen)
    {
    case UpperScreen:
        start = 0;
        end = FX32f(0.5f);
        break;

    case LowerScreen:
        start = FX32f(0.5f);
        end = FX32f(1.0f);
        break;

    case BothScreens:
        start = 0;
        end = FX32f(1.0f);
        break;
    }

    GetFontCanvas()->ClearSection(start, end);
}

int UIText::GetHeight()
{
    MEMORY_POOL("UIText");

    return GetFontCanvas()->GetStringHeight(m_pFont, m_Text.c_str(), m_XWrap, &m_Lines);
}

int UIText::GetWidth()
{
    MEMORY_POOL("UIText");

    return GetFontCanvas()->GetStringWidth(m_pFont, m_Text.c_str(), 0);
}

bool UIText::ResizeFont()
{
    MEMORY_POOL("UIText");

    if(g_sDisableFontResizing || m_DisableWrappingAndResizing)
    {
        return false;
    }

    std::string fontName = m_FontName;
    int count = 0;

    if (!m_pFont)
        m_pFont = NitroFont::GetFont(m_FontName.c_str());

    bool doHeightWrapping  = (m_DesiredHeight != 0);

    while( (GetWidth() > m_XWrap) || (doHeightWrapping && (GetHeight() > m_DesiredHeight)) )
    {
        if(m_MinimumFontName != "" && m_MinimumFontName == m_CurrentFontName)
        {
            hdLog(("UIText", "Reached minimum font size: %s\n", fontName.c_str()));
            break;
        }

        hdLog(("UIText", "Resizing %s(%i, %i) to fit (%i, %i)\n", m_Text.c_str(), GetWidth(), GetHeight(), m_XWrap, m_DesiredHeight));
        ++count; hdAssert(count != g_sMaxResizeFontAttempts);

        fontName = ReduceFontSize(fontName);
        SetFont(fontName.c_str());
        SetText(m_Text.c_str(), false);
    }

    return (count > 0);
}

std::string UIText::ReduceFontSize(std::string fontName)
{
    MEMORY_POOL("UIText");

    string oldFontName = fontName;
    StringVec fontPathParts = Split(fontName.c_str(), "_");
    hdAssert(fontPathParts.size() > 2);

    hdLog(("UIText", "Font: "));
    for(StringVec::iterator i = fontPathParts.begin(); i != fontPathParts.end(); ++i)
    {
        string str = *i;
        hdLog(("UIText", "%s ", str.c_str()));
    }
    hdLog(("UIText", "\n"));

    int size = 0;
    FromString(fontPathParts[1], size);
    size -= g_sResizeDecrement;

    if(size < g_skMinimumFontSize)    // this is currently our smallest font size
    {
        hdWarning(("UIText", "Trying to set the font to %i which is smaller than our smallest font size!\n"));    
    }

    if(size < g_skMinimumFontSize)
    {
        size = g_skMinimumFontSize;
    }

    fontPathParts[1] = size >= 10 ? ToString(size) : "0" + ToString(size);  // single digit fonts have a leading zero
    fontName = Join(fontPathParts, "_");

    hdLog(("UIText", "Old font: %s  New font: %s\n", oldFontName.c_str(), fontName.c_str()));

    return fontName;
}

bool UIText::IsText()
{
    MEMORY_POOL("UIText");

    return true;
}

#pragma force_active on
REGISTER_UI_TYPE(UIText)
#pragma force_active off

} // namespace core
