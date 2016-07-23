// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UICOMPONENT_H_
#define _CORE_UICOMPONENT_H_

#include "core.h"

#include "xml\xml.h"
#include "signal\signal.h"
#include "input\input.h"
#include "crc32.h"
#include "superenum.h"
#include "ui\uievent.h"

namespace core {

class UIComponent;
class UIContainer;

#ifdef __HNS_UI_CHECK_CASTS__
#define GetUIChild(parent, classname, childName) parent->GetChild<classname>(childName)
#define GetUILayer(classname, layerName) UILayer::GetLayer<classname>(layerName)
#else
#define GetUIChild(parent, classname, childName) static_cast<classname*>(parent->FindChild(childName))
#define GetUILayer(classname, layerName) static_cast<classname*>(UILayer::FindLayer(layerName))
#endif // __HNS_UI_CHECK_CASTS__

template <class t_type>
bool GetElementValue(XmlElement* parent, const char* elementName, t_type& ref)
{
    return FromXMLElement(parent, elementName, ref, false, true);
}

class UIComponent
{
public:
    ENUM2(UIFlag,
        Visible,
        Enabled
        );

    typedef vector<UIEventTrigger*> EventList;

    UIComponent();

    virtual ~UIComponent() {}

    virtual void Initialize(XmlElement* pXmlElement);

    virtual void PostInitialize(XmlElement* pXmlElement);

    virtual void Shutdown();

    virtual void Uncache();

    virtual void Cache();

    virtual void Render();

    virtual void Tick(Float deltaSeconds);

    virtual void GetBounds(int& t, int& l, int& b, int& r);    

    virtual bool OnTouch(int x, int y);

    virtual void OnTouchMoved(int x, int y);

    virtual void OnTouchPressed(int x, int y);

    virtual bool OnTouchReleased(int, int);

    virtual bool OnButtonDown(Joystick*, Joystick::Button b);

    virtual void OnUIEvent(UIEvent event);

    virtual void Begin();

    virtual void End();
    
    virtual void Enter();
    
    virtual void Leave();
    
    virtual void HitTest(int x, int y, std::vector<UIComponent*>& vec);

    virtual bool IsLayer();

    virtual bool IsText();

    inline const char* GetName()
    {
        return m_Name.c_str();
    }

    inline CompactString GetCRCName() const
    {
        return m_Name;
    }

    inline int GetX()
    {
        return x;
    }

    inline int GetY()
    {
        return y;
    }

    inline bool IsMobile()
    {
        return m_Mobile;
    }

    inline void SetX(int nX)
    {
        x = nX;
    }

    inline void SetY(int nY)
    {
        y = nY;
    }

    inline void SetPosition(int nX, int nY)
    {
        SetX(nX);
        SetY(nY);        

        OnPositionChanged();
    }

    virtual void OnPositionChanged() {}

    int GetAbsoluteX();

    int GetAbsoluteY();

    void SetParent(UIContainer* pParent);

    unsigned int GetNumParents();

    inline unsigned int GetWidth()
    {
        return m_Width;
    }

    inline unsigned int GetHeight()
    {
        return m_Height;
    }

    inline void SetWidth(unsigned int w)
    {
        m_Width = w;
    }

    inline void SetHeight(unsigned int h)
    {
        m_Height = h;
    }

    inline void SetMobile(bool bMobile)
    {
        m_Mobile = bMobile;
    }

    inline int GetSFXID()
    {
        return m_SFXID;
    }

    inline void SetSFXID(int id)
    {
        m_SFXID = id;
    }

    void Set(UIFlag flag, bool value);

    void TurnOn(bool bEnabled)
    {
        Set(Enabled, bEnabled);
        Set(Visible, bEnabled);
    }

    inline bool Is(UIFlag flag)
    {
        hdAssertRange(flag, 0, UIFlag_Total);
        return m_Flags[flag];
    }

    /// called when a flag changes
    virtual void OnFlagChanged(UIFlag) {}

    virtual bool IsCollidingWithStylus() { return m_Hover; }

    void AddEvent(UIEventTrigger* pUITrigger);

    void InitializeEvents(XmlElement* element);

    // makes sure all events are reset and our hover bool is turned out, ensures we don't accidentally fire events
    virtual void ClearAllTouchInfo();

protected:

    CompactString m_Name;    

    // todo mg - change to float/int/vec2; do platform dividing??
    int x;
    int y;
    unsigned int m_Width;
    unsigned int m_Height;

    UIContainer* m_pParent;

    bool m_Hover;
    bool m_Mobile;

    EventList m_Events;

    int m_SFXID;            // sound effect played on tap

    bool m_Flags[UIFlag_Total];

    enum InitState
    {
        NeedsInitializing,
        NeedsPostInitializing,
        CompletedInitialization

    } m_InitState;


    friend class UIContainer;
};

} // namespace core

#endif // _CORE_UI_COMPONENT_H_
