// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui\uipagegraph.h"
#include "ui\uieditor.h"
#include "ui\uiedge.h"
#include "ui\uimanager.h"
#include "ui\navbar.h"

#include "ui\uipage.h"
#include "crc32.h"
#include "checkcircle.h"

#include "test/TestResponse.h"

namespace core {

UIPageGraph::UIPageGraph() :
    m_pCurrentPage(NULL),
    m_pPrevPage(NULL),
    m_pCurrentTransition(NULL),
    m_StartPageName(""),
    m_ClearStringTable(true)
{

}

void UIPageGraph::Initialize( XmlElement* element )
{
    TIMING_POOL("UIPageGraph::Initialize");

    UIContainer::Initialize(element);

    InitializeTransitions(element);

    // load any string tables
    XmlElement* childElement;
    int count = 0;
    for(childElement = element->FirstChildElement("StringTable"); childElement != NULL; childElement = childElement->NextSiblingElement("StringTable"))
    {
        string tableName;
        tableName = childElement->GetText();
        UILayer::LoadStringTable(tableName.c_str(), (count++ == 0) && m_ClearStringTable);
    }
}

void UIPageGraph::PostInitialize(XmlElement* element)
{
    TIMING_POOL("UIPageGraph::PostInitialize");

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        UIPage* page = (UIPage*)*i;

        page->m_pUIPageGraph = this;
        AddPage(page);
    }

    XmlElement* pStart = element->FirstChildElement("StartPage");

    if (pStart == NULL)
    {
        hdWarning(("UI", "No start page defined, using first page found."));
        m_StartPageName = (*m_PageMap.begin()).second->GetCRCName();
    }
    else
    {
        m_StartPageName = pStart->GetText();
    }

    m_pCurrentTransition = NULL;

    // do this after, so all the pages know the graph they are in
    UIContainer::PostInitialize(element);

    //GotoPage(m_StartPageName);
}

void UIPageGraph::Shutdown(void)
{
    m_pCurrentPage = NULL;
    m_pPrevPage = NULL;
    m_PageMap.clear();

    DestroyAllTransition();

    UIContainer::Shutdown();

    UILayer::DestroyAllLayers();
    UIText::FreeFontCanvas();
    wyt::CheckCircle::s_LoadedShared = false;
}

void UIPageGraph::AddPage(UIPage* page)
{
    hdAssertMsg(m_PageMap.find(page->m_Name) == m_PageMap.end(), ("ID already associated with a UIPage!"));
    m_PageMap[page->m_Name] = page;
}

UIPage* UIPageGraph::GetPage(CompactString pageName, bool assertOnFail /*= false*/)
{
    if(m_PageMap.find(pageName) == m_PageMap.end())
    {       
        hdAssertMsg(!assertOnFail, ("Could not find page! crc: %d!", pageName));
        return NULL;
    }

    return m_PageMap[pageName];
}

UIPage* UIPageGraph::GetCurrentPage(void) const
{
    return m_pCurrentPage;
}

void UIPageGraph::Render()
{
    if(m_pCurrentTransition)
    {
        m_pCurrentTransition->Render();
    }

    if(m_pPrevPage)
    {
        m_pPrevPage->Render();
    }

    else if(m_pCurrentPage)
    {
        m_pCurrentPage->Render();
    }
}

void UIPageGraph::Tick(Float deltaSeconds)
{
    static const int waitFrames = 1;
    static int frame = 0;

    if(m_pCurrentTransition)
    {
        m_pCurrentTransition->Tick(deltaSeconds);

        switch(m_pCurrentTransition->GetTransitionState())
        {
        case UITransition::eTransitionOut:
            m_pPrevPage->Tick(deltaSeconds);
            break;

        case UITransition::eTransitionOutDone:
            
            if (frame++ >= waitFrames)
            {
                frame = 0;

                hdAssert((m_pCurrentPage != NULL));                
                hdAssert((m_pPrevPage != NULL));                

                m_pPrevPage->End();
                m_pPrevPage = NULL;


                m_pCurrentPage->Begin();
                m_pCurrentPage->Enter();

                UILayer::CleanupLayers();

                m_pCurrentTransition->AcknowledgeTransition();

                hdLog(("UIPageGraph", "***************************\nEntered %s\n", m_pCurrentPage->GetName()));
                //DumpPools();
            }
            break;

        case UITransition::eTransitionInDone:
            m_pCurrentTransition = NULL;

            // send the test event that the page has been reached
            hdLog(("TestXML", "<Test type='WaitTillPageTest' page='%s'/>\n", m_pCurrentPage->m_Name.c_str()));
            TEST_MESSAGE("PageReadyForInput", CRC32::Calculate(m_pCurrentPage->m_Name.c_str()));
            TEST_MESSAGE("ReadyForInput", 0);        

            break;

        default:
            m_pCurrentPage->Tick(deltaSeconds);
        }
    }
    else
    {
        m_pCurrentPage->Tick(deltaSeconds);
    }
}

bool UIPageGraph::OnTouch(int x, int y)
{
    // don't do touch events during transitions
    if(m_pCurrentPage && m_pCurrentTransition == NULL)
    { 
        return m_pCurrentPage->OnTouch(x, y);
    }

    return false;
}

void UIPageGraph::OnTouchMoved(int x, int y)
{
    if(m_pCurrentPage)
    {
        m_pCurrentPage->OnTouchMoved(x, y);
    }
}

void UIPageGraph::OnTouchPressed(int x, int y)
{
    if(m_pCurrentPage)
    {
        m_pCurrentPage->OnTouchPressed(x, y);
    }
}

bool UIPageGraph::OnTouchReleased(int x, int y)
{
    if(m_pCurrentPage)
    {
        return m_pCurrentPage->OnTouchReleased(x, y);
    }
    
    return false;
}

bool UIPageGraph::OnButtonDown(Joystick* joy, Joystick::Button b)
{
    if(m_pCurrentPage)
    {
        return m_pCurrentPage->OnButtonDown(joy, b);
    }

    return false;
}

void UIPageGraph::OnUIEvent(UIEvent eventID)
{
    m_pCurrentPage->OnUIEvent(eventID);
}

void UIPageGraph::GotoPage(CompactString pageName)
{
    if (pageName == "")
        pageName = m_StartPageName;

    if (m_pCurrentPage)
    {
        MEMORY_POOL("UIPageGraph::Leaving()");
        m_pPrevPage = m_pCurrentPage;
        m_pPrevPage->Leave();
        m_pCurrentPage = m_PageMap[pageName];

        hdAssertMsg((m_pCurrentPage != NULL), ("Unknown page: '%s'\n", pageName.c_str()));        
    }
    else
    {
        MEMORY_POOL("UIPageGraph::Entering()");
        m_pCurrentPage = m_PageMap[pageName];
        
        hdAssertMsg((m_pCurrentPage != NULL), ("Unknown page: '%s'\n", pageName.c_str()));        

        m_pCurrentPage->Begin();
        m_pCurrentPage->Enter();

        hdLog(("TestXML", "<Test type='WaitTillPageTest' page='%s'/>\n", m_pCurrentPage->m_Name.c_str()));
        TEST_MESSAGE("PageReadyForInput", CRC32::Calculate(m_pCurrentPage->m_Name.c_str()));
        TEST_MESSAGE("ReadyForInput", 0);        
    }
}

UITransition* UIPageGraph::GetTransition(const char* name, bool assertOnFail /*= false*/)
{
    if(m_TransitionMap.find(name) == m_TransitionMap.end())
    {   
        hdAssertMsg(!assertOnFail, ("Could not find Transition %s!", name));
        return NULL;
    }

    return m_TransitionMap[name];
}

void UIPageGraph::DestroyAllTransition()
{
    for (TransitionMap::iterator iter = m_TransitionMap.begin(); iter != m_TransitionMap.end(); ++iter)
    {
        UITransition* pUITransition = (*iter).second;
        delete pUITransition;
    }

    m_TransitionMap.clear();
}

void UIPageGraph::InitializeTransitions(XmlElement* element)
{
    XmlElement* childElement;
    for(childElement = element->FirstChildElement("Transition"); childElement != NULL; childElement = childElement->NextSiblingElement("Transition"))
    {
        std::string type, name;
        FromXMLAttribute(childElement, "type", type);
        FromXMLAttribute(childElement, "name", name);

        UITransition* pTransition = g_TransitionFactory.Create(type);
        pTransition->Initialize(childElement);
        AddTransition(name.c_str(), pTransition);
    }
}

void UIPageGraph::AddTransition(const char* name, UITransition* pUITransition)
{
    MEMORY_POOL("UI Transition Map");
    hdAssertMsg(m_TransitionMap.find(name) == m_TransitionMap.end(), ("Transition %s is already registered!", name));

    m_TransitionMap[name] = pUITransition;
}

void UIPageGraph::UncacheAll()
{
    for(PageMap::iterator i = m_PageMap.begin(); i != m_PageMap.end(); ++i)
    {
        UIPage* pPage = i->second;
        pPage->Uncache();
    }
}

void UIPageGraph::SetClearStringTable(bool clearStringTable)
{
    m_ClearStringTable = clearStringTable;
}

core::CompactString UIPageGraph::GetCurrentPageName()
{
    if(m_pCurrentPage)
    {
        return m_pCurrentPage->GetCRCName();
    }

    return "";
}

} // namespace core
