// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UIEDGE_H_
#define _CORE_UIEDGE_H_

#include "core.h"

#include "xml/xml.h"
#include "crc32.h"

namespace core {

class UIPageGraph;
class UIPage;
class UITransition;
typedef CompactString UIEvent;

class UIEdge
{
public:    

    UIEdge();

    virtual ~UIEdge();

    virtual void Init(XmlElement* pXmlElement);    

    virtual void ErrorInit();

    void OnUIEvent(UIEvent eventID);

    void SetPageGraph(UIPageGraph* pPageGraph);

    virtual void Trigger() = 0;

    void AcknowledgeTransition();

protected:

    UIEvent m_Event;
    UITransition* m_pTransitionIn;
    UITransition* m_pTransitionOut;
    UIPageGraph* m_pPageGraph;
};

typedef Factory<UIEdge, std::string> EdgeFactory;
extern EdgeFactory g_EdgeFactory;

// helper Macro for UI Factory registration
#define REGISTER_UI_EDGE(type)                                                       \
namespace                                                                            \
{                                                                                    \
    UIEdge* create##type() { MEMORY_POOL("UIEdge::"#type); return new type; }        \
    static bool registered##type = g_EdgeFactory.RegisterType(#type, &create##type); \
}

//////////////////////////////////////////////////////////////////////////

class UIPageEdge:
    public UIEdge
{
public:
    virtual void Init(XmlElement* pXmlElement);

    virtual void Trigger();

    virtual void SetPageName(CompactString name) { m_PageName = name; }

protected:
    CompactString m_PageName;
};

//////////////////////////////////////////////////////////////////////////

class UIGraphEdge:
    public UIEdge
{
public:
    virtual void Init(XmlElement* pXmlElement);

    virtual void Trigger();

protected:
    // FIX - this should just be the fileID of the graph file
    //       no need to store the strings in UI manager
    std::string* m_pGraph;
};

} // namespace core

#endif // _CORE_UIEDGE_H_