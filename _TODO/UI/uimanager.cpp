// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui\uimanager.h"

#include "input\input.h"
#include "renderer\nitrorenderer.h"

#include "ui\uilayer.h"
#include "ui\navbar.h"
#include "ui\uipage.h"
#include "nitroplatform.h"
#include "engine/functimer.h"
#include "crc32.h"
#include "audio\nitrosfxdriver.h"

#include "file/filesystem.h"

#include "fontcanvas.h"

#include "xml/XmlDocument.h"

namespace core {

template<> UIManager* PointerSingleton<UIManager>::s_pInstance = NULL;

UIManager::UIManager() :
    m_ActiveEvent("")
{

}

bool UIManager::Init()
{
    MEMORY_POOL("UIManager");

    m_InGameplay = false;
    m_LastDownTick = 0;

    m_ActiveEvent = "";

    //NitroSfxDriver::Instance().RegisterSpeaker(this);

#if !defined(__HNS_FINAL__) && !defined(__HNS_FINAL_DEBUG__)
    AddExecutionChild(&wyt::uiedit::UIEditor::Instance());
#else
    // hook in
    Input::Instance().m_Down.memConnect(this, &OnTouch);
    Input::Instance().m_Moved.memConnect(this, &OnTouchMoved);
    Input::Instance().m_Up.memConnect(this, &OnTouchReleased);
    Input::Instance().m_Pressed.memConnect(this, &OnTouchPressed);
    Input::Instance().m_ButtonPressed.memConnect(this, &OnButtonDown);
#endif

    return ExecutionModule::Init();
}

bool UIManager::Render()
{
    FUNC_TIMER("UIManager::Render");

    FontCanvas* pCanvas = UIText::GetFontCanvas();

    if(pCanvas && pCanvas->Is(ExecutionModule::Visible))
        pCanvas->Render();

    m_PageGraph.Render();

    return ExecutionModule::Render();
}

bool UIManager::Tick(Float deltaSeconds)
{
    FUNC_TIMER("UIManager::Tick");

    FontCanvas* pCanvas = UIText::GetFontCanvas();

    if(pCanvas && pCanvas->Is(ExecutionModule::Enabled))
    {
        pCanvas->Tick(deltaSeconds);
        pCanvas->Clear();
    }

    if(IsActiveEventWaiting())
        FireUIEvent();

    m_PageGraph.Tick(deltaSeconds);

    return ExecutionModule::Tick(deltaSeconds);
}

void UIManager::OnTouch(TouchScreen*, int x, int y)
{
    Float currentTick = Clock::Instance().TimeElapsed();
    if(m_LastDownTick != currentTick)
    {
        m_PageGraph.OnTouch(x, y);
        m_LastDownTick = currentTick;
    }
}

void UIManager::OnTouchMoved(TouchScreen*, int x, int y)
{
    m_PageGraph.OnTouchMoved(x, y);
}

void UIManager::OnTouchReleased(TouchScreen*, int x, int y)
{
    m_PageGraph.OnTouchReleased(x, y);
}

void UIManager::OnTouchPressed(TouchScreen*, int x, int y)
{
    m_PageGraph.OnTouchPressed(x, y);
}

void UIManager::OnButtonDown(Joystick* joy, Joystick::Button b)
{
    m_PageGraph.OnButtonDown(joy, b);
}

bool UIManager::RegisterType(const std::string s, UIComponent*(*fn)(), const char* pFile, int line)
{
    AddRegistration(s, pFile, line);
    return m_componentFactory.RegisterType(s, fn);
}

bool UIManager::OnUIEvent(UIEvent event)
{
    if(IsTransitioning() == false)
    {
        m_ActiveEvent = event;
        return true;
    }

    return false;
}

void UIManager::FireUIEvent()
{
    hdAssert(m_ActiveEvent != "");

    m_PreviousPageName = m_PageGraph.GetCurrentPageName();
    m_PageGraph.OnUIEvent(m_ActiveEvent);

    m_ActiveEvent = "";
}

void UIManager::OnUITransition(std::string graphName)
{
    UnloadPageGraph();
    LoadPageGraph(graphName);
}

void UIManager::GotoPage(CompactString pageName)
{
    m_PageGraph.GotoPage(pageName);
}

void UIManager::LoadPageGraph(std::string graphName, bool clearStringTable)
{
    TIMING_POOL("UIManager::LoadPageGraph");
    MEMORY_POOL("UI Page Graph");

    std::string filename = "ui/layer/";
    filename += graphName + ".graph";

    hdLog(("UI", "UIManager::LoadPageGraph(%s)\n", graphName.c_str()));

    xml::Document document;
    document.LoadFinal(filename);    
    m_PageGraph.SetClearStringTable(clearStringTable);
    m_PageGraph.Initialize(document);
    UILayer::PostInitializeLayers();
    m_PageGraph.PostInitialize(document);
}

void UIManager::UnloadPageGraph()
{
    MEMORY_POOL("UI Page Graph");
    m_PageGraph.Shutdown();
}

UINavbar* UIManager::GetNavbar()
{
    // Note: Do not use GetUILayer, because we don't want to assert on fail.

#ifdef __HNS_UI_CHECK_CASTS__
    return UILayer::GetLayer<UINavbar>("navbar", false);
#else
    return static_cast<UINavbar*>(UILayer::FindLayer("navbar", false));
#endif // __HNS_UI_CHECK_CASTS__
}

UITransition* UIManager::GetTransition(const char* name)
{
    return m_PageGraph.GetTransition(name);
}

void UIManager::SetCurrentTransition(UITransition* val)
{
    m_PageGraph.SetCurrentTransition(val);
}

UITransition* UIManager::GetCurrentTransition()
{
    return m_PageGraph.GetCurrentTransition();
}

bool UIManager::IsTransitioning()
{
    return (GetCurrentTransition() != NULL);
}

UIComponent* UIManager::CreateUIComponent(XmlElement* pElement)
{
    TIMING_POOL("UIManager::CreateUIComponent");
    std::string type = pElement->GetAttribute("type");
    UIComponent* pUIComponent = NULL;

    if(type == "layerfile")
    {
        // this is a reference to a layer file from the ui page graph definition

        // first, check to see if the layer is already loaded
        std::string name = pElement->GetAttribute("target");

        UILayer* pUILayer = UILayer::FindLayer(name.c_str(), false);
        if(pUILayer)
        {
            pUILayer->AddReference(1);
            return pUILayer;
        }
        else
        {
            // load the layer file and create a new layer
            return UILayer::LoadLayer(name.c_str());
        }
    }
    else
    {
        hdLog(("UI Creation", "Created a '%s', registered from '%s'.\n", type.c_str(), m_Regisitrations[type].c_str()));
        pUIComponent = UIManager::Instance().m_componentFactory.Create(type);
    }

    return pUIComponent;
}

void UIManager::AddRegistration(const std::string type, const char* pFile, int line)
{
    char pDetail[255];
    sprintf(pDetail, "%s, line %d", pFile, line);
    m_Regisitrations[type] = pDetail;
}

void UIManager::UncacheAll()
{
    m_PageGraph.UncacheAll();
}

UIPage* UIManager::GetCurrentPage() const
{
    return m_PageGraph.GetCurrentPage();
}

core::CompactString UIManager::GetCurrentPageName() const
{
    UIPage* pPage = GetCurrentPage();

    if(pPage)
    {
        return pPage->GetCRCName();
    }

    return "";
}
} // namespace core
