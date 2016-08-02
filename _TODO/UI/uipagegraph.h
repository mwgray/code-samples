// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UIPAGEGRAPH_
#define _CORE_UIPAGEGRAPH_

#include "ui\uicontainer.h"

#include "ui\uitransition.h"

namespace core {

class UIPage;
class UINavbar;

class UIPageGraph:
    public UIContainer
{
public:
    UIPageGraph();

    virtual ~UIPageGraph() {}

    virtual void Initialize(XmlElement* element);

    virtual void PostInitialize(XmlElement* element);

    virtual void Shutdown(void);

    void AddPage(UIPage* page);

    UIPage* GetPage(CompactString pageName, bool assertOnFail = false);

    UIPage* GetCurrentPage(void) const;

    virtual void Render();

    virtual void Tick(Float deltaSeconds);

    virtual bool OnTouch(int x, int y);

    virtual void OnTouchMoved(int x, int y);

    virtual void OnTouchPressed(int, int);

    virtual bool OnTouchReleased(int x, int y);

    virtual bool OnButtonDown(Joystick* joy, Joystick::Button b);

    virtual void OnUIEvent( UIEvent event );

    void GotoPage(CompactString pageName);

    UITransition* GetTransition(const char* name, bool assertOnFail = false);

    void DestroyAllTransition();

    UITransition* GetCurrentTransition() const { return m_pCurrentTransition; }
    void SetCurrentTransition(UITransition* val) { m_pCurrentTransition = val; }

    void UncacheAll();

    void SetClearStringTable(bool clearStringTable);

    void SetStartPageName(const CompactString &name) { m_StartPageName = name; }


    CompactString GetCurrentPageName();

protected:
    void InitializeTransitions(XmlElement* element);

    void AddTransition(const char* name, UITransition* pUITransition);

    typedef std::map<CompactString, UIPage*> PageMap;
    PageMap m_PageMap;    

    UIPage* m_pCurrentPage;
    UIPage* m_pPrevPage;

    typedef std::map<CompactString, UITransition*> TransitionMap;
    TransitionMap m_TransitionMap;

    UITransition* m_pCurrentTransition;

    CompactString m_StartPageName;

    bool m_ClearStringTable;
};

} // namespace core

#endif // _CORE_UIPAGEGRAPH_
