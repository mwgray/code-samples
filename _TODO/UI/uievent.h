// CORE - Copyright 2007 HumaNature Studios Inc.

#ifndef _CORE_UIEVENT_H_
#define _CORE_UIEVENT_H_

#include "core.h"

#include "xml\xml.h"
#include "crc32.h"
#include "input/input.h"

namespace core {

typedef CompactString UIEvent;

//////////////////////////////////////////////////////////////////////////
class UIEventTrigger
{
public:
    UIEventTrigger();

    virtual void Init(XmlElement* pXmlElement);

    virtual bool Tick(Float deltaSeconds);

    virtual void Render(Float deltaSeconds);

    virtual bool OnButtonDown(Joystick::Button b);
    
    virtual bool OnTouch(int x, int y, int l, int r, int t, int b);

    virtual bool OnTouchMoved(int x, int y, int l, int r, int t, int b);

    virtual bool OnTouchPressed(int x, int y, int l, int r, int t, int b);

    // call to get all the associated variables ready again (UIComponent::Begin is a good place for this)
    virtual void Reset();

    void SetEventName(const char* name){ m_Event = name; }

protected:
    UIEvent m_Event;
    bool m_Activated;
};

typedef Factory<UIEventTrigger, std::string> EventFactory;
extern EventFactory g_EventFactory;

// helper Macro for UI Factory registration
#define REGISTER_UI_EVENT(type)                                                         \
namespace                                                                               \
{                                                                                       \
    UIEventTrigger* create##type() { MEMORY_POOL("UIEvent::"#type); return new type; }  \
    static bool registered##type = g_EventFactory.RegisterType(#type, &create##type);   \
}

//////////////////////////////////////////////////////////////////////////
//<Event type="UITouchDownTrigger" name="Back"/>
class UITouchDownTrigger :
    public UIEventTrigger
{
public:
    virtual bool OnTouch(int x, int y, int l, int r, int t, int b);
};

//////////////////////////////////////////////////////////////////////////
//<Event type="UITouchDownWithoutTransitionTrigger"/>
class UITouchDownWithoutTransitionTrigger :
    public UITouchDownTrigger
{
public:
    virtual void Init(XmlElement* pXmlElement);

    virtual bool Tick(Float deltaSeconds);
};

//////////////////////////////////////////////////////////////////////////
//<Event type="UITouchEventTrigger" name="OK"/>
class UITouchEventTrigger : 
    public UITouchDownTrigger
{
public:
    virtual bool OnTouchMoved(int x, int y, int l, int r, int t, int b);

    virtual bool OnTouchPressed(int x, int y, int l, int r, int t, int b);
};

//////////////////////////////////////////////////////////////////////////
//<Event type="UITouchEventWithoutTransitionTrigger"/>
class UITouchEventWithoutTransitionTrigger : 
    public UITouchEventTrigger
{
public:
    virtual void Init(XmlElement* pXmlElement);

    virtual bool Tick(Float deltaSeconds);
};

//////////////////////////////////////////////////////////////////////////
//<Event type="UITimedEventTrigger" name="OK" time=3.0/>
class UITimedEventTrigger : 
    public UIEventTrigger
{
public:
    UITimedEventTrigger();

    virtual void Init(XmlElement* pXmlElement);

    virtual bool Tick(Float deltaSeconds);

    virtual void Reset();

protected:
    Float m_Timer;
    Float m_EventTime;
};


//////////////////////////////////////////////////////////////////////////
//<Event type="UIButtonEventTrigger" name="OK" button="Start"/>
class UIButtonEventTrigger : 
    public UIEventTrigger
{
public:
    UIButtonEventTrigger();

    virtual void Init(XmlElement* pXmlElement);

    virtual bool OnButtonDown(Joystick::Button b);

protected:
    Joystick::Button m_Button;
};

}   // core

#endif //_CORE_UIEVENT_H_