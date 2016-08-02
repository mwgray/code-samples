#include "ui\uievent.h"

#include "ui\uimanager.h"

namespace core {

EventFactory g_EventFactory;

//////////////////////////////////////////////////////////////////////////
// UIEventTrigger
//////////////////////////////////////////////////////////////////////////
UIEventTrigger::UIEventTrigger() :
m_Event(""),
m_Activated(false)
{
}

void UIEventTrigger::Init(XmlElement* pXmlElement)
{
    m_Event = pXmlElement->GetAttribute("name");
}

bool UIEventTrigger::Tick(Float /*deltaSeconds*/)
{
    if(m_Activated)
    {
        UIManager::Instance().OnUIEvent(m_Event);
        m_Activated = false;    // so we don't continue to send events
        return true;
    }
    
    return false;
}

void UIEventTrigger::Render(Float /*deltaSeconds*/)
{
}

bool UIEventTrigger::OnButtonDown(Joystick::Button /*b*/)
{
    return false;
}

bool UIEventTrigger::OnTouch(int /*x*/, int /*y*/, int /*l*/, int /*r*/, int /*t*/, int /*b*/)
{
    return false;
}

bool UIEventTrigger::OnTouchMoved(int /*x*/, int /*y*/, int /*l*/, int /*r*/, int /*t*/, int /*b*/)
{
    return false;
}

bool UIEventTrigger::OnTouchPressed( int /*x*/, int /*y*/, int /*l*/, int /*r*/, int /*t*/, int /*b*/ )
{
    return false;
}

void UIEventTrigger::Reset()
{
    hdLog(("UIEventTrigger", "Reset() %s\n", m_Event.c_str()));
    m_Activated = false;
}


//////////////////////////////////////////////////////////////////////////
// UITouchDownTrigger
//////////////////////////////////////////////////////////////////////////
bool UITouchDownTrigger::OnTouch(int x, int y, int l, int r, int t, int b)
{
    // HACK jr mg: this prevents the bool from getting deactivated
    return m_Activated |= RectPointIntersect(x, y, t, l, b, r);
}

#pragma force_active on
REGISTER_UI_EVENT(UITouchDownTrigger)
#pragma force_active off


//////////////////////////////////////////////////////////////////////////
// UITouchDownWithoutTransitionTrigger
//////////////////////////////////////////////////////////////////////////
void UITouchDownWithoutTransitionTrigger::Init(XmlElement*)
{

}

bool UITouchDownWithoutTransitionTrigger::Tick(Float)
{
    if(m_Activated)
    {
        m_Activated = false;    // so we don't continue to send events
        return true;
    }

    return false;
}

#pragma force_active on
REGISTER_UI_EVENT(UITouchDownWithoutTransitionTrigger)
#pragma force_active off


//////////////////////////////////////////////////////////////////////////
// UITouchEventTrigger
//////////////////////////////////////////////////////////////////////////
bool UITouchEventTrigger::OnTouchMoved(int x, int y, int l, int r, int t, int b)
{
    return OnTouch(x, y, l, r, t, b);
}

bool UITouchEventTrigger::OnTouchPressed(int x, int y, int l, int r, int t, int b)
{
    return OnTouch(x, y, l, r, t, b);
}

#pragma force_active on
REGISTER_UI_EVENT(UITouchEventTrigger)
#pragma force_active off


//////////////////////////////////////////////////////////////////////////
// UITouchEventWithouttransitionTrigger
//////////////////////////////////////////////////////////////////////////
void UITouchEventWithoutTransitionTrigger::Init(XmlElement*)
{

}

bool UITouchEventWithoutTransitionTrigger::Tick(Float)
{
    if(m_Activated)
    {
        m_Activated = false;    // so we don't continue to send events
        return true;
    }

    return false;
}

#pragma force_active on
REGISTER_UI_EVENT(UITouchEventWithoutTransitionTrigger)
#pragma force_active off


//////////////////////////////////////////////////////////////////////////
// UITimedEvent
//////////////////////////////////////////////////////////////////////////
UITimedEventTrigger::UITimedEventTrigger() :
m_Timer(0),
m_EventTime(0)
{
}

void UITimedEventTrigger::Init(XmlElement* pXmlElement)
{
    UIEventTrigger::Init(pXmlElement);

    float time;
    FromXMLAttribute(pXmlElement, "time", time);    
    m_EventTime = FX32f(time);
}

bool UITimedEventTrigger::Tick(Float deltaSeconds)
{
    if(m_Timer < m_EventTime)
    {
        m_Timer += deltaSeconds;

        if(m_Timer >= m_EventTime)
        {
            m_Activated = true;
        }
    }

    return UIEventTrigger::Tick(deltaSeconds);
}

void UITimedEventTrigger::Reset()
{
    UIEventTrigger::Reset();

    m_Timer = 0;
}

#pragma force_active on
REGISTER_UI_EVENT(UITimedEventTrigger)
#pragma force_active off


//////////////////////////////////////////////////////////////////////////
// UITimedEvent
//////////////////////////////////////////////////////////////////////////
UIButtonEventTrigger::UIButtonEventTrigger() :
m_Button(Joystick::None)
{
}

void UIButtonEventTrigger::Init(XmlElement* pXmlElement)
{
    UIEventTrigger::Init(pXmlElement);

    m_Button = Joystick::FromString(pXmlElement->GetAttribute("button"));
}

bool UIButtonEventTrigger::OnButtonDown(Joystick::Button b)
{
    if(b == m_Button)
    {
        m_Activated = true;
    }

    return m_Activated;
}

#pragma force_active on
REGISTER_UI_EVENT(UIButtonEventTrigger)
#pragma force_active off
             
}   // core