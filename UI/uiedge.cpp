// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui/uiedge.h"

#include "ui/uimanager.h"
#include "ui/uitransition.h"

namespace core {

EdgeFactory g_EdgeFactory;

UIEdge::UIEdge() :
    m_Event(""),
    m_pTransitionIn(NULL),
    m_pTransitionOut(NULL),
    m_pPageGraph(NULL)
{

}

UIEdge::~UIEdge()
{

}

void UIEdge::Init(XmlElement* pXmlElement)
{
    m_Event = pXmlElement->GetAttribute("event");

    string transitionIn;
    if(!FromXMLAttribute(pXmlElement, "transIn", transitionIn, false))
        transitionIn = "defaultIn";

    m_pTransitionIn = UIManager::Instance().GetTransition(transitionIn.c_str());
    m_pTransitionIn->SetParent(this);

    string transitionOut;
    if(!FromXMLAttribute(pXmlElement, "transOut", transitionOut, false))
        transitionOut = "defaultOut";

    m_pTransitionOut = UIManager::Instance().GetTransition(transitionOut.c_str());
    m_pTransitionOut->SetParent(this);
}

void UIEdge::ErrorInit()
{
    m_Event = "ToErrorMode";
    m_pTransitionIn  = UIManager::Instance().GetTransition("defaultIn");
    m_pTransitionOut = UIManager::Instance().GetTransition("defaultOut");
}

void UIEdge::OnUIEvent(UIEvent eventID)
{
    if(m_Event == eventID)
    {
#if !defined(__HNS_FINAL__) && !defined(__HNS_FINAL_DEBUG__)
        wyt::uiedit::UIEditor::Instance().SetCurrentComponent(NULL);
#endif

        Trigger();
    }
}

void UIEdge::SetPageGraph(UIPageGraph* pPageGraph)
{
    m_pPageGraph = pPageGraph;
}

void UIEdge::AcknowledgeTransition()
{
    if(UIManager::Instance().GetCurrentTransition() == m_pTransitionOut)
    {
        UIManager::Instance().SetCurrentTransition(m_pTransitionIn);
        m_pTransitionIn->Enter();
    }
}

//////////////////////////////////////////////////////////////////////////

void UIPageEdge::Init(XmlElement* pXmlElement)
{
    UIEdge::Init(pXmlElement);

    FromString(pXmlElement->GetAttribute("page"), m_PageName);
}

void UIPageEdge::Trigger()
{
    UIManager& ui = UIManager::Instance();
    
    hdAssertMsg((m_PageName != ""), ("Empty page name in edge\n"));    

    ui.GotoPage(m_PageName);
    ui.SetCurrentTransition(m_pTransitionOut);
    m_pTransitionOut->Leave();
}

#pragma force_active on
REGISTER_UI_EDGE(UIPageEdge)
#pragma force_active off

//////////////////////////////////////////////////////////////////////////

void UIGraphEdge::Init(XmlElement* pXmlElement)
{
    UIEdge::Init(pXmlElement);
}

void UIGraphEdge::Trigger()
{
    UIManager::Instance().OnUITransition(*m_pGraph);
}

#pragma force_active on
REGISTER_UI_EDGE(UIGraphEdge)
#pragma force_active off

} // namespace core
