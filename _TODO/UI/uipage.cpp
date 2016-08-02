// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui\uipage.h"

#include "ui\uimanager.h"
#include "ui\navbar.h"
#include "renderer\nitrorenderer.h"
#include "debug\convar.h"
#include "TimingPoolInstance.h"

namespace core {

ConVar<int> g_TicksBetweenFade("UIPage::g_TicksBetweenFade", 1, "number of ticks to pause before continuing the fade between pages");
ConVar<int> g_FadeStep("UIPage::g_FadeStep", 3, "amount to fade by when it is time (fade range is between -16 and 16, so don't pick anything too big)");
ConVar<bool> g_ReportMemoryEachPage("g_ReportMemoryEachPage", false);

ConVar<std::string> g_PageToTrack("UIPage::g_PageToTrack", "", "the timing pools will be cleared before the begin of this page");

class UILayerReference:
    public UIComponent
{
    std::string m_LayerName;
    UILayer* m_pLayer;
public:
    UILayerReference()
    {
        m_Name = "UILayerReference";
    }

    virtual void Initialize(XmlElement* pXmlElement)
    {
        UIComponent::Initialize(pXmlElement);

        FromXMLAttribute(pXmlElement, "target", m_LayerName);
    }
    
    virtual void Begin()
    {
        UIComponent::Begin();

        m_pLayer = UILayer::FindLayer(m_LayerName.c_str(), false);
        if(m_pLayer)
        {
            m_pLayer->AddReference(1);            
        }
        else
        {
            // load the layer file and create a new layer
            m_pLayer = UILayer::LoadLayer(m_LayerName.c_str());
        }

        m_pLayer->SetReference(this);                
    }

    virtual void Enter()
    {
        UIComponent::Enter();

        m_pParent->SwapChild(this, m_pLayer);
        m_pLayer->SpecialPostInitialize();
        m_pLayer->Begin();
        m_pLayer->Enter();
    }

    virtual void End()
    {
        UIComponent::End();

        m_pParent->SwapChild(m_pLayer, this);

        m_pLayer->AddReference(-1);
    }
};

void UIPage::Initialize(XmlElement* element)
{
    TIMING_POOL("UIPage::Initialize");
    UIContainer::Initialize(element);

    m_FadingTimer = 0;
    m_pNavbar = NULL;
}

void UIPage::PostInitialize(XmlElement* element)
{
    TIMING_POOL("UIPage::PostInitialize");

    InitializeEdges(element);

    UIContainer::PostInitialize(element);

    // find the navbar
    m_pNavbar = HNS_RELEASE_CAST(UINavbar*, UILayer::FindLayer("navbar", false));    
}

void UIPage::Shutdown()
{
    // NOTE dj: Do NOT call UIContainer::Shutdown here, because our children can have
    //  multiple parents. Layers get handled in DestroyAllLayers.

    Uncache();

    for (EdgeVec::iterator iter = m_EdgeVec.begin(); iter != m_EdgeVec.end(); ++iter)
        delete *iter;

    std::vector<UIComponent*>::iterator i;
    for(i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        UIComponent* pComponent = *i;

        if(pComponent != NULL)
        {
            if(pComponent->GetCRCName() == "UILayerReference")
            {
                pComponent->Shutdown();
                delete pComponent;
            }
        }
    }


    m_EdgeVec.clear();
    m_Children.clear();

    UIComponent::Shutdown();
}

void UIPage::Tick(Float deltaSeconds)
{
    UIContainer::Tick(deltaSeconds);
}

void UIPage::Uncache()
{
    UIContainer::Uncache();
}

bool UIPage::OnTouch( int x, int y )
{
    hdLog(("UI", "\nOnTouch %s page\n", m_Name.c_str()));
    return UIContainer::OnTouch(x, y);
}

void UIPage::OnUIEvent(UIEvent event)
{
    // go through current pages edge events and execute code based on the event type
    for(EdgeVec::iterator iter = m_EdgeVec.begin(); iter != m_EdgeVec.end(); iter++)
    {
        // todo - should we have a bool return so it doesn't get accepted?
        (*iter)->OnUIEvent(event);
    }

    UIContainer::OnUIEvent(event);
}

void UIPage::Begin()
{
    hdLog(("UI", "\nBegin %s page\n", m_Name.c_str()));

    if(g_ReportMemoryEachPage)
    {
        hdLog(("Page Mem Report", "\nBegin %s page\n", m_Name.c_str()));
        hdLog(("Page Mem Report", "%s\n", PlatformGetMemorySummary()));
    }

    if (g_PageToTrack != "" && g_PageToTrack == m_Name.c_str())
    {
        TimingPoolInstance::ClearPools();
    }

    // clear all timing pools
    UIContainer::Begin();
}

void UIPage::InitializeEdges(XmlElement* element)
{
    hdLog(("UI", "UIPage::InitializeEdges() : %s\n", m_Name.c_str()));

    // Get the number of edges to instantiate
    EdgeVec::size_type numEdges = 0;
    XmlElement* childElement;
    for(childElement = element->FirstChildElement("Edge"); childElement != NULL; childElement = childElement->NextSiblingElement("Edge"))
    {
        numEdges++;
    }

    m_EdgeVec.reserve(numEdges);

    for(int i = 0; i < numEdges; ++i)
    {
        childElement = i ? childElement->NextSiblingElement("Edge") : element->FirstChildElement("Edge");

        std::string type;
        FromXMLAttribute(childElement, "type", type);

        hdLog(("UI", "Edge : %s\n", type.c_str()));

        UIEdge* pEdge = g_EdgeFactory.Create(type);
        pEdge->SetPageGraph(m_pUIPageGraph);
        pEdge->Init(childElement);

        m_EdgeVec.push_back(pEdge);
    }

    // HACK (sorta): Give everyone an error mode edge.
    UIEdge* pEdge = g_EdgeFactory.Create("UIGameModeEdge");
    pEdge->SetPageGraph(m_pUIPageGraph);
    pEdge->ErrorInit();
    m_EdgeVec.push_back(pEdge);
}


#pragma force_active on
REGISTER_UI_TYPE(UIPage)
REGISTER_UI_TYPE(UILayerReference)
#pragma force_active off

} // namespace core
