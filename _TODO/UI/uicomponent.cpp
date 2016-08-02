// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui\uimanager.h"

#include "ui\uilayer.h"

#include "ui\uipage.h"

#include "audio\nitrosfxdriver.h"
#include "audio\WYT_music.sadl"
#include "camera\camera.h"
#include "message.h"
#include "debug/debugboxdrawer.h"

namespace core {

#pragma force_active on
REGISTER_UI_TYPE(UIComponent)
#pragma force_active off

UIComponent::UIComponent(): 
m_InitState(NeedsInitializing),
m_pParent(NULL),
m_Hover(false),
m_Mobile(false),
x(0),
y(0),
m_Width(0),
m_Height(0),
m_SFXID(SFX_MenuSelect01)
{
    m_Name = "";
}

void UIComponent::Initialize(XmlElement* element)
{
    // Save path here?
    FromXMLAttribute(element,"name", m_Name, false);    

    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));

    GetElementValue(element, "x", x);
    GetElementValue(element, "y", y);
    GetElementValue(element, "w", m_Width);
    GetElementValue(element, "h", m_Height);

    GetElementValue(element, "Mobile", m_Mobile);

    GetElementValue(element, "SFX", m_SFXID);

    for(int i = 0; i < UIFlag_Total; ++i)
    {
        m_Flags[i] = true;
        GetElementValue(element, UIFlagToString(i), m_Flags[i]);
    }

    m_Hover = false;
    m_InitState = NeedsPostInitializing;
}

void UIComponent::PostInitialize(XmlElement* element)
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));

    InitializeEvents(element);

    m_InitState = CompletedInitialization;
}

void UIComponent::Shutdown()
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));

    for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
    {
        UIEventTrigger* pEvent = *i;
        delete pEvent;
    }

    m_Events.clear();
}

ConVar<bool> g_RenderUIBoxes("g_RenderUIBoxes", false);
void UIComponent::Render()
{
    if(g_RenderUIBoxes)
    {
        Float3 tl, br;
        Vector<Float, 3> tlBB, brBB;
        int t,l,b,r;
        GetBounds(t,l,b,r);
        tl = Camera::Instance().TouchScreenToWorld(l, t); tlBB.x = tl.x; tlBB.y = tl.y; tlBB.z = tl.z;
        br = Camera::Instance().TouchScreenToWorld(r, b); brBB.x = br.x; brBB.y = br.y; brBB.z = br.z;

        DebugBoxDrawer::Instance().DrawBox(tl, br - tl, Red);
    }
}

void UIComponent::Uncache()
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));
}


void UIComponent::Cache()
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));
}


void UIComponent::Tick(Float deltaSeconds)
{
    for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
        (*i)->Tick(deltaSeconds);
}

void UIComponent::GetBounds(int& t, int& l, int& b, int& r)
{
    t = GetAbsoluteY();
    l = GetAbsoluteX();
    b = t + static_cast<int>(m_Height);
    r = l + static_cast<int>(m_Width);
}

bool UIComponent::OnTouch(int tx, int ty)
{
    if(Is(Enabled))
    {
        int t, l, b, r;
        GetBounds(t, l, b, r);

        hdLog(("UI", "Checking collision %s (%i, %i) to (t=%i b=%i l=%i r=%i )\n", m_Name.c_str(), tx, ty - 192, t, b, l, r));

        for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
        {
            UIEventTrigger* pEvent = *i;
            m_Hover |= pEvent->OnTouch(tx, ty - 192, l, r, t, b);
        }

        if (m_Hover && m_SFXID != -1)
        {
            hdLog(("UI", "UIComponent %s playing sound %i\n", m_Name.c_str(), m_SFXID));
            NitroSfxDriver::Instance().PlaySound(&UIManager::Instance(), m_SFXID);
        }

        return m_Hover;   
    }

    return false;
}

void UIComponent::OnTouchMoved(int tx, int ty)
{
    if(Is(Enabled))
    {
        int t, l, b, r;
        GetBounds(t, l, b, r);

        for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
        {
            UIEventTrigger* pEvent = *i;
            m_Hover |= pEvent->OnTouchMoved(tx, ty - 192, l, r, t, b);
        }   
    }

    return;
}

void UIComponent::OnTouchPressed(int tx, int ty)
{
    if(Is(Enabled))
    {
        int t, l, b, r;
        GetBounds(t, l, b, r);

        for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
        {
            UIEventTrigger* pEvent = *i;
            m_Hover |= pEvent->OnTouchPressed(tx, ty - 192, l, r, t, b);
        }   
    }

    return;
}

bool UIComponent::OnTouchReleased(int, int)
{
    m_Hover = false;

    return false;
}

bool UIComponent::OnButtonDown(Joystick*, Joystick::Button b)
{
    for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
    {
        UIEventTrigger* pEvent = *i;
        if(pEvent->OnButtonDown(b))
        {
            return true;
        }
    }

    return false;
}

void UIComponent::OnUIEvent( UIEvent )
{
}

void UIComponent::Begin()
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));

    ClearAllTouchInfo();
}

void UIComponent::End()
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));
}

void UIComponent::Enter()
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));

    for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
    {
        UIEventTrigger* pEvent = *i;
        pEvent->Reset();
    }       
}

void UIComponent::Leave()
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));
}

void UIComponent::HitTest(int x, int y, std::vector<UIComponent*>& vec)
{
    if (!m_Mobile) return;

    int t, l, b, r;
    GetBounds(t, l, b, r);
    
    if (RectPointIntersect(x, y - 192, t, l, b, r))
        vec.push_back(this);
}

bool UIComponent::IsLayer()
{
    return false;
}

bool UIComponent::IsText()
{
    return false;
}

void UIComponent::AddEvent( UIEventTrigger* pUITrigger )
{
    m_Events.push_back(pUITrigger);
}

void UIComponent::InitializeEvents(XmlElement* element)
{
    hdLog(("UI", "%s%s: '%s' of type '%s'\n", 
        IsLayer() ? "\n" : "",
        __FUNCTION__,
        m_Name.c_str(),
        HNS_TYPEID_NAME(this)
        ));

    // Get the number of edges to instantiate
    EventList::size_type numEvents = 0;
    XmlElement* childElement;
    for(childElement = element->FirstChildElement("Event"); childElement != NULL; childElement = childElement->NextSiblingElement("Event"))
    {
        numEvents++;
    }

    m_Events.reserve(numEvents);

    for(int i = 0; i < numEvents; ++i)
    {
        childElement = i ? childElement->NextSiblingElement("Event") : element->FirstChildElement("Event");

        std::string type;
        FromXMLAttribute(childElement, "type", type);

        UIEventTrigger* pEvent = g_EventFactory.Create(type);
        pEvent->Init(childElement);

        m_Events.push_back(pEvent);
    }
}

int UIComponent::GetAbsoluteX()
{
    return (!IsLayer() && m_pParent) ? m_pParent->GetAbsoluteX() + x : x;
}

int UIComponent::GetAbsoluteY()
{
    return (!IsLayer() && m_pParent) ? m_pParent->GetAbsoluteY() + y : y;
}

void UIComponent::SetParent(UIContainer* pParent)
{
    m_pParent = pParent;        
}

unsigned int UIComponent::GetNumParents()
{
    return m_pParent ? 1 + m_pParent->GetNumParents() : 0;
}

void UIComponent::Set( UIFlag flag, bool value )
{
    hdAssertRange(flag, 0, UIFlag_Total);
    if(m_Flags[flag] != value)
    {
        m_Flags[flag] = value;
        OnFlagChanged(flag);

        if(flag == Enabled && value == false)
        {
            ClearAllTouchInfo();
        }
    }
}

void UIComponent::ClearAllTouchInfo()
{
    hdLog(("UI", "%s::ClearAllTouchInfo()\n", m_Name.c_str()));

    m_Hover = false;

    for(EventList::iterator i = m_Events.begin(); i != m_Events.end(); ++i)
    {
        UIEventTrigger* pEvent = *i;
        pEvent->Reset();
    }
}

} // namespace core
