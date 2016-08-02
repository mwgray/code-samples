#include "ui/uitransition.h"

#include "renderer/nitrorenderer.h"
#include "ui/uiedge.h"
#include "ui\errorlayer.h"

namespace core {

TransitionFactory g_TransitionFactory;
UITransition::TransitionDoneSignal UITransition::s_TransitionOutDoneSignal;
UITransition::TransitionDoneSignal UITransition::s_TransitionInDoneSignal;

//////////////////////////////////////////////////////////////////////////
// UITransition
//////////////////////////////////////////////////////////////////////////
bool UITransition::Initialize(XmlElement* pXmlElement)
{
    m_FadingOffset = 0;
    m_FadeTime = 0;
    m_Timer = 0;
    m_OffsetTimer = 0;
    m_pParent = NULL;

    float offset, fadeTime;
    if(FromXMLAttribute(pXmlElement, "offset", offset, false))
    {
        m_FadingOffset = FX32f(offset);
    }

    if(FromXMLAttribute(pXmlElement, "fadeTime", fadeTime, false))
    {
        m_FadeTime = FX32f(fadeTime);
    }

    string screen = "";
    if(FromXMLAttribute(pXmlElement, "screen", screen))
    {
        m_Screen = TransitionScreenFromString(screen);
    }

    return true;
}

bool UITransition::Tick(Float deltaSeconds)
{
    // ensures the transitioning speed won't be effected by framerate hits
    deltaSeconds = Clamp(deltaSeconds, FX32i(0), FX32f(1.0f/20.0f));

    if(m_OffsetTimer > 0)
    {
        m_OffsetTimer -= deltaSeconds;

        if(m_OffsetTimer > 0)
        {
            return true;
        }
    }

    m_Timer += deltaSeconds;

    switch(m_TransitionState)
    {
    case eTransitionIn:
        TransitionIn();
        
        if(m_Timer >= m_FadeTime)
        {
            m_TransitionState = eTransitionInDone;
            s_TransitionInDoneSignal.send();
        }
        break;

    case eTransitionOut:
        TransitionOut();
        
        if(m_Timer >= m_FadeTime)
        {
            m_TransitionState = eTransitionOutDone;
            s_TransitionOutDoneSignal.send();            
        }
        break;
    }

    return true;
}

void UITransition::Enter()
{
    m_Timer = 0;

    m_TransitionState = eTransitionIn;
}   

void UITransition::Leave()
{
    m_OffsetTimer = m_FadingOffset;
    m_Timer = 0;

    m_TransitionState = eTransitionOut;
}

void UITransition::AcknowledgeTransition()
{
    if (m_pParent)
        m_pParent->AcknowledgeTransition();
}

#pragma force_active on
REGISTER_UI_TRANSITION(UITransition)
#pragma force_active off

//////////////////////////////////////////////////////////////////////////
// UIFadeTransition
//////////////////////////////////////////////////////////////////////////
bool UIFadeTransition::TransitionIn()
{
    NitroRenderer* pRenderer = &NitroRenderer::Instance();
    Float interpValue = Clamp(FX_Div(m_Timer, m_FadeTime), FX32i(0), FX32_ONE);
    Float fadingValue = FX_Interpolate(FX32i(pRenderer->s_FadeMax), FX32i(pRenderer->s_FadeDefault), interpValue);//FX32i(pRenderer->s_FadeMax) - FX_Mul(interpValue, FX32i(pRenderer->s_FadeMax - pRenderer->s_FadeDefault));
    int nFadingValue = FX_Whole(fadingValue);

    hdLog(("UITransition", "UIDualFadeTransition::TransitionIn() fadingValue=%.2f or %i currFade=%i\n", FX_FX32_TO_F32(fadingValue), nFadingValue, pRenderer->GetTopFade()));

    if (wyt::UIErrorLayer::GetError() != wyt::ErrorCode_None)
        nFadingValue = pRenderer->s_FadeMax;

    switch(m_Screen)
    {
    case Top:
        if(pRenderer->GetTopFade() == pRenderer->s_FadeDefault) return true;
        pRenderer->SetTopFade(nFadingValue);
    	break;

    case Bottom:
        if(pRenderer->GetBottomFade() == pRenderer->s_FadeDefault) return true;
        pRenderer->SetBottomFade(nFadingValue);
        break;

    case Dual:
        if(pRenderer->GetTopFade() == pRenderer->s_FadeDefault) return true;
        pRenderer->SetFade(nFadingValue);
        break;
    }

    return (nFadingValue == FX32i(pRenderer->s_FadeDefault));
}

bool UIFadeTransition::TransitionOut()
{
    NitroRenderer* pRenderer = &NitroRenderer::Instance();
    Float interpValue = Clamp(FX_Div(m_Timer, m_FadeTime), FX32i(0), FX32_ONE);
    Float fadingValue = FX_Interpolate(FX32i(pRenderer->s_FadeDefault), FX32i(pRenderer->s_FadeMax), interpValue);
    int nFadingValue = FX_Whole(fadingValue);

    hdLog(("UITransition", "UIDualFadeTransition::TransitionOut() fadingValue=%.2f or %i currFade=%i\n", FX_FX32_TO_F32(fadingValue), nFadingValue, pRenderer->GetTopFade()));

    switch(m_Screen)
    {
    case Top:
        if(pRenderer->GetTopFade() == pRenderer->s_FadeMax) return true;
        pRenderer->SetTopFade(nFadingValue);
        break;

    case Bottom:
        if(pRenderer->GetBottomFade() == pRenderer->s_FadeMax) return true;
        pRenderer->SetBottomFade(nFadingValue);
        break;

    case Dual:
        if(pRenderer->GetTopFade() == pRenderer->s_FadeMax) return true;
        pRenderer->SetFade(nFadingValue);
        break;
    }

    return (nFadingValue == FX32i(pRenderer->s_FadeMax));
}


#pragma force_active on
REGISTER_UI_TRANSITION(UIFadeTransition)
#pragma force_active off

}   // core