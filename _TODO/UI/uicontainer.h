// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UICONTAINER_
#define _CORE_UICONTAINER_

#include "ui\uicomponent.h"

namespace core {

class UIContainer:
    public UIComponent
{
public:
    virtual ~UIContainer() {}
    
    virtual void Initialize(XmlElement* element);

    virtual void PostInitialize(XmlElement* element);

    virtual void Shutdown(void);

    virtual void Uncache();

    virtual void Cache();

    virtual void Render();

    virtual void Tick(Float deltaSeconds);

    virtual void GetBounds(int& t, int& l, int& b, int& r);

    UIComponent* FindChild(const char* string);

    template <typename t_type> 
    t_type* GetChild(const char* string, bool assertOnMiss = true)
    {
        UIComponent* pUIComponent = FindChild(string);
        if(pUIComponent)            
        {
            t_type* pType = HNS_RELEASE_CAST(t_type*,pUIComponent);
            if(pType)
            {
                return pType;
            }
            else
            {
                hdHaltMsg(("Bad dynamic cast of child '%s' to class '%s'!\n", string, HNS_TYPEID_NAME(t_type*)));
            }
        }
        else if(assertOnMiss)
        {
            hdHaltMsg(("Could not find child '%s'!\n", string));
        }

        return NULL;
    }

    virtual bool OnTouch(int x, int y);

    virtual void OnTouchMoved(int x, int y);

    virtual void OnTouchPressed(int x, int y);

    virtual bool OnTouchReleased(int x, int y);

    virtual bool OnButtonDown(Joystick*, Joystick::Button b);
    
    virtual void OnUIEvent(UIEvent event);

    virtual void Begin();

    virtual void End();

    virtual void Enter();

    virtual void Leave();

    virtual void HitTest(int x, int y, std::vector<UIComponent*>& vec);

    void SwapChild(UIComponent* pOld, UIComponent* pNew);

protected:
    std::vector<UIComponent*> m_Children;
};

} // namespace core

#endif // _CORE_UICONTAINER_
