// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UITEXT_H_
#define _CORE_UITEXT_H_

#include "ui\uicomponent.h"

#include "justification.h"

#ifndef __HNS_FINAL__
#include "memory/new.h"
#endif

namespace core {

class Font;
class FontCanvas;

class UIText:
    public UIComponent
{
public:
    enum CanvasRange { UpperScreen, LowerScreen, BothScreens, CanvasRange_Total };

    UIText();

    virtual ~UIText();

    virtual void Initialize(XmlElement* element);

    virtual void Shutdown();

    virtual void Begin();
    
    virtual void Enter();
    virtual void End();

    virtual bool OnTouch(int x, int y);

    // if tryOriginalFontSize == true, will try to use the desired font size before resizing
    virtual void SetText(const char* pString, bool settingNewText = true);

    virtual const char* GetText(void);

    virtual void SetFont(const char* pFontPath);

    virtual void SetFont(u8 fontSize);

    virtual void Render();

    static void FreeFontCanvas();

    virtual void FreeFont();

    Color GetColor() const;
    Color GetBGColor() const;

    void SetColor(Color val);
    void SetBGColor(Color val);

    void SetPaletteDirty(bool dirty);

    void GetTextDimenstions(int& w, int& h);
    void GetTopLeftPosition(int& x, int& y);

    inline Justification GetHJust() { return m_HJust; }
    inline Justification GetVJust() { return m_VJust; }

    inline void SetHJust(Justification just) { m_HJust = just; }
    inline void SetVJust(Justification just) { m_VJust = just; }

    virtual void GetBounds(int& t, int& l, int& b, int& r);    

    void OnTouchMoved(int tx, int ty);

    bool IsTextBiggerThanScreenVertically() const { return m_IsTextBiggerThanScreenVertically; }

    // clears text from the specified screen
    // NOTE: only use top/bottom screen IF USING A Nitro8BitDualScreenFontCanvas!!!!!!
    // usage: this will clear the specified screen(s) of text ONLY
    virtual void Clear(CanvasRange screen = BothScreens);

    static void SetFontCanvas(FontCanvas* pFontCanvas);    

    static FontCanvas* GetFontCanvas();

    int GetHeight();
    int GetWidth();

    virtual bool IsText();

    int GetXWrap() const 
    { 
        return m_XWrap; 
    }

    void SetXWrap(int val) 
    { 
        m_XWrap = val; 
    }

    const char* GetCurrentFontName(){ return m_CurrentFontName.c_str(); }

    const char* GetDefaultFontName() const { return m_FontName.c_str(); }
    void SetDefaultFontName(const char* val) { m_FontName = val; }

#ifndef __HNS_FINAL__
    static void SetMemOverride(bool what) { m_sMemOverride = what; }
    static bool GetMemOverride() { return m_sMemOverride; } 
#endif

    bool GetDisableWrappingAndResizing() const 
    { 
        return m_DisableWrappingAndResizing; 
    }

    void SetDisableWrappingAndResizing(bool val) 
    { 
        m_DisableWrappingAndResizing = val; 
    }

protected:
    // reduces font size to fit the text height into m_DesiredHeight, unless m_DesiredHeight == 0
    virtual bool ResizeFont();

    // decrements m_ReducedSizeFontName by one
    std::string ReduceFontSize(std::string fontName);

    Font* m_pFont;

    void RenderWrappingBoundingBox();

#ifndef __HNS_FINAL__
    static bool m_sMemOverride;
#endif    

    std::string m_Text;
    std::string m_OriginalText;
    string m_FontName;
    string m_MinimumFontName;
    string m_CurrentFontName;

    Justification m_HJust;
    Justification m_VJust;
    Color m_FGColor;
    Color m_BGColor;

    int m_PaletteIndex;
    int m_XWrap;
    int m_DesiredHeight;    // this will resize (framerate hit) the text till it fits this
    int m_Lines;
    bool m_ForceOneLine;
    bool m_DisableWrappingAndResizing;
    bool m_PaletteDirty;

    bool m_IsTextBiggerThanScreenVertically;
    
    void SetupPalette(bool darken = false);    

    static FontCanvas* m_spFontCanvas;
};

} // namespace core

#endif // _CORE_UITEXT_H_
