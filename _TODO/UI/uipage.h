// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UIPAGE_
#define _CORE_UIPAGE_

#include "ui\uicontainer.h"
#include "ui\uiedge.h"

namespace core {

class UIPageGraph;
class UINavbar;

class UIPage:
    public UIContainer
{
public:
    typedef std::vector<UIEdge*> EdgeVec;

    virtual ~UIPage() {}

    virtual void Initialize(XmlElement* element);

    virtual void PostInitialize(XmlElement* element);

    virtual void Shutdown(void);

    virtual void Tick(Float deltaSeconds);

    virtual void Uncache();

    virtual bool OnTouch(int x, int y);

    virtual void OnUIEvent( UIEvent event );

    // load up the components we need for this page
    virtual void Begin();

    EdgeVec& GetEdges(){ return m_EdgeVec; }

protected:

    void InitializeEdges(XmlElement* element);

    int m_ID;

    UIPageGraph* m_pUIPageGraph;
    
    EdgeVec m_EdgeVec;

    friend class UIPageGraph;

    unsigned int m_FadingTimer;

    UINavbar* m_pNavbar;
};

} // namespace core

#endif // _CORE_UIPAGE_