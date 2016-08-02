// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UILAYER_
#define _CORE_UILAYER_

#include "ui\uicontainer.h"
#include "ui\uitext.h"

#include "StringTable.h"

namespace core {

class UILayer:
    public UIContainer
{    
protected:
    typedef std::map<CompactString, UILayer*> LayerMap;

    static LayerMap m_LayerMap;
    static StringTableCollection m_StringMap;

public:
    // Don't change the order, these match anim indices for the buttons!
    ENUM4(NavbarComponent,
        Left,
        Right,
        Back,
        OK
        );

    UILayer();

    virtual void Initialize(XmlElement* element);

    virtual void Shutdown();

    static void AddLayer(const char* name, UILayer* pUILayer);

    static UILayer* FindLayer(const char* name, bool assertOnFail = true);

    template <typename t_type> 
    static t_type* GetLayer(const char* name, bool assertOnFail = true)
    {
        UILayer* pLayer = FindLayer(name, assertOnFail);
        if (pLayer)
        {
            t_type* pType = HNS_RELEASE_CAST(t_type*,pLayer);

            if (pType)
            {
                return pType;
            }
            else
            {
                hdHaltMsg(("Bad dynamic cast of child %s to class '%s'!\n", name, HNS_TYPEID_NAME(t_type*)));
            }
        }

        return NULL;
    }

    static UILayer* LoadLayer(const char* name);

    static void PostInitializeLayers();

    void SpecialPostInitialize();

    static void LoadStringTable(const char* pName, bool clearTableBeforeLoading = true);

    static void DestroyLayers(std::list<CompactString>& layerList);

    static void DestroyAllLayers();
    
    static void RemoveLayer(CompactString layerName);

    static LayerMap::iterator RemoveLayer(LayerMap::iterator layerIter);

    virtual void Begin();

    virtual void Tick(Float deltaSeconds);

    virtual bool IsLayer();

    bool AddReference(int ref);

    static std::string GetLocalizedString(const CompactString& key);

    void SetReference(UIComponent* pComponent)
    {
        m_pReference = pComponent;        
    }

    virtual void End();

    static void CleanupLayers();

    UIComponent* m_pReference;

protected:
    CompactString m_Playlist;
    FileID m_FileID;

    Float   m_Timer;
    UIEvent m_OnTimerUIEvent;
    Float   m_TimedEventTime;
    int     m_RefCount;
    bool    m_TimerEventFired;  // move to layer!
    bool    m_UseStringTable;
    bool    m_NeedPostInit;

    bool    m_NavbarComponents[NavbarComponent_Total];
};

} // namespace core

#endif // _CORE_UILAYER_
