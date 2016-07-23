#include "ui/uiscenepage.h"

#include "introtextlayer.h"
#include "nitrorenderer.h"
#include "wytcharacter.h"

#include "wytworld.h"

BEGIN_NAMESPACE_CORE

/******************************************************************************
**   
*/ UIScenePage::UIScenePage() 
/*
*******************************************************************************/
{
    m_bDoPallette = false;    
}

/******************************************************************************
**   
*/ void UIScenePage::PostInitialize(XmlElement* element) 
/*
*******************************************************************************/
{
    UIPage::PostInitialize(element);
    
    FromString(element->GetAttribute("scene"), m_SceneName);
    FromXMLAttribute(element, "name", m_Name);

    hdLog(("ScenePage", "Name: '%s' Scene: '%s'\n", m_Name.c_str(), m_SceneName.c_str()));
    
    m_TopFade = m_BottomFade = NitroRenderer::Instance().s_FadeDefault;    

    GetElementValue(element, "TopFade", m_TopFade);
    GetElementValue(element, "BottomFade", m_BottomFade);

    m_ZoomCharacter = "";

    XmlElement *pElem = element->FirstChildElement("ZoomCharacter");
    if(pElem) {
        m_ZoomCharacter = pElem->GetText();
        FromXMLAttribute(pElem, "Pallete", m_bDoPallette);
    }
}


/******************************************************************************
**   
*/ void UIScenePage::Begin() 
/*
*******************************************************************************/
{
    UIPage::Begin();

    UIComponent *txtlayer = FindChild("introtextlayer");
    if(!txtlayer)
        txtlayer = FindChild("introtextlayerbottom");
    
    if(txtlayer) {        
        wyt::UIIntroTextLayer *pITLayer = HNS_RELEASE_CAST(wyt::UIIntroTextLayer*,txtlayer);
        hdAssert((pITLayer != NULL));
        pITLayer->SetScene(m_Name.c_str());
    }

    const int fdef = NitroRenderer::Instance().s_FadeDefault;

    if(m_TopFade != fdef)
        NitroRenderer::Instance().SetTopFade(m_TopFade);
    if(m_BottomFade != fdef) {
        NitroRenderer::Instance().SetBottomFade(m_BottomFade);

    }

    if(m_SceneName != "") {        
        hdLog(("ScenePage", "Playing scene: '%s'\n", m_SceneName.c_str()));    
        wyt::ScenePlayer::Instance().PlayScene(m_SceneName);    
    }    
}

/******************************************************************************
**   
*/ void UIScenePage::End() 
/*
*******************************************************************************/
{
    UIPage::End();    
    wyt::ScenePlayer::Instance().End();    
    //World::Instance().UncacheAll();
}


/******************************************************************************
**   
*/ void UIScenePage::Shutdown() 
/*
*******************************************************************************/
{
    UIPage::Shutdown();
}

#pragma force_active on
REGISTER_UI_TYPE(UIScenePage)
#pragma force_active off


END_NAMESPACE_CORE
