//  Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UISCENEPAGE_
#define _CORE_UISCENEPAGE_


#include "ui/uimanager.h"
#include "ui/uipage.h"
#include "sceneplayer.h"

#include "wytcharacter.h"


BEGIN_NAMESPACE_CORE

class UIScenePage:
    public UIPage
{
public:

    UIScenePage();
    
    virtual void PostInitialize(XmlElement* element);

    virtual void Begin();

    // unload this page's components
    virtual void End();

    virtual void Shutdown();    
    
protected:


    map<wyt::Character*, Float3> m_CharacterPositions;    

    std::string m_ZoomCharacter;
    bool   m_bDoPallette;    
    CompactString m_SceneName;    
    std::string m_Name;    

    int m_BottomFade;
    int m_TopFade;    
};

END_NAMESPACE_CORE

#endif
