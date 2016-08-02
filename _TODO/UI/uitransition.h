// CORE - Copyright 2007 HumaNature Studios Inc.

#ifndef _CORE_UITRANSITION_H_
#define _CORE_UITRANSITION_H_

#include "core.h"

#include "xml\xml.h"
#include "superenum.h"

namespace core {

class UIEdge;

//////////////////////////////////////////////////////////////////////////
class UITransition
//////////////////////////////////////////////////////////////////////////
{
public:
    enum TransitionState { eTransitionIn = 0, eTransitionInDone, eTransitionOut, eTransitionOutDone, TransitionState_Total };
    typedef Signal<> TransitionDoneSignal;
    static TransitionDoneSignal s_TransitionOutDoneSignal;
    static TransitionDoneSignal s_TransitionInDoneSignal;

    ENUM3(TransitionScreen,
        Top,
        Bottom,
        Dual);

    virtual bool Initialize(XmlElement* pXmlElement);

    virtual bool Tick(Float deltaSeconds);

    virtual void Render(){}

    virtual void Enter();

    virtual void Leave();

    TransitionState GetTransitionState() const { return m_TransitionState; }
    void SetTransitionState(TransitionState state) { m_TransitionState = state; }

    UIEdge* GetParent() const { return m_pParent; }
    void SetParent(UIEdge* val) { m_pParent = val; }

    void AcknowledgeTransition();

protected:
    virtual bool TransitionIn(){ return true; }

    virtual bool TransitionOut(){ return true; }

    TransitionState m_TransitionState;
    TransitionScreen m_Screen;

    Float m_FadingOffset;   // time after to told to transition to actually fire the transition
    Float m_OffsetTimer;

    Float m_FadeTime;
    Float m_Timer;
    
    UIEdge* m_pParent;
};

typedef Factory<UITransition, std::string> TransitionFactory;
extern TransitionFactory g_TransitionFactory;

// helper Macro for UI Factory registration
#define REGISTER_UI_TRANSITION(type)                                                        \
    namespace                                                                               \
{                                                                                           \
    UITransition* create##type() { return new type; }                                       \
    static bool registered##type = g_TransitionFactory.RegisterType(#type, &create##type);  \
}

//////////////////////////////////////////////////////////////////////////
class UIFadeTransition :
//////////////////////////////////////////////////////////////////////////
    public UITransition
{
protected:
    virtual bool TransitionIn();

    virtual bool TransitionOut();
};

}

#endif