// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UIMANAGER_H_
#define _CORE_UIMANAGER_H_

#include "core.h"
#include "ui\uieditor.h"
#include "ui\uicomponent.h"
#include "ui\uipagegraph.h"

namespace core {

class TouchScreen;
class UIComponent;
class UINavbar;
class UITransition;

class UIManager:
    public ExecutionModule,
    public PointerSingleton<UIManager>
{
public:
    friend class wyt::uiedit::UIEditor;

    typedef Factory<UIComponent, std::string> ComponentFactory;

    ComponentFactory m_componentFactory;

    UIManager();

    virtual bool Init();        

    virtual bool Render();

    virtual bool Tick(Float deltaSeconds);

    void OnTouch(TouchScreen*, int, int);

    void OnTouchMoved(TouchScreen*, int, int);

    void OnTouchReleased(TouchScreen*, int, int);

    void OnTouchPressed(TouchScreen*, int, int);

    void OnButtonDown(Joystick*, Joystick::Button b);

    bool RegisterType(const std::string s, UIComponent*(*fn)(), const char* pFile, int line);

    // sets the active UI event, which will get called at the start of UIManager::Tick().  ensures new page gets ticked and rendered properly
    virtual bool OnUIEvent(UIEvent event);

    // fires the active UI event
    virtual void FireUIEvent();

    virtual void OnUITransition(std::string graphName);

    void LoadPageGraph(std::string graphName, bool clearStringTable = true);

    void UnloadPageGraph();

    virtual void GotoPage(CompactString pageName);

    UINavbar* GetNavbar();

    bool IsActiveEventWaiting(){ return m_ActiveEvent != ""; }

    bool IsTransitioning();

    bool GetInGameplay() const { return m_InGameplay; }
    void SetInGameplay(bool val) { m_InGameplay = val; }

    UITransition* GetTransition(const char* name);

    void SetCurrentTransition(UITransition* val);
    UITransition* GetCurrentTransition();

    UIComponent* CreateUIComponent(XmlElement* pElement);

    void UncacheAll();

    CompactString GetPreviousPageName() const { return m_PreviousPageName; }
    CompactString GetCurrentPageName() const;

    UIPage* GetCurrentPage() const;

protected:
    void AddRegistration(const std::string type, const char* pFile, int line);
    bool m_InGameplay;  // true if we are currently in the game, false if we are in full screen UI

    std::map<std::string, std::string> m_Regisitrations;

    UIPageGraph m_PageGraph;

    UIEvent m_ActiveEvent;      // when this isn't "", there is an event that needs to be fired

    Float m_LastDownTick;

    CompactString m_PreviousPageName;
};

// helper Macro for UI Factory registration
#define REGISTER_UI_TYPE(type)                                                                                              \
namespace                                                                                                                   \
{                                                                                                                           \
    UIComponent* create##type() { MEMORY_POOL("UI::"#type); return new type; }                                              \
    static bool registered##type = UIManager::Instance().RegisterType(#type, &create##type, __FILE__, __LINE__);            \
}

} // namespace core

#endif // _CORE_UIMANAGER_H_
