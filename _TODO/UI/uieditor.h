
#ifndef _UIEDITOR_H_
#define _UIEDITOR_H_


#include "ui\uicomponent.h"
#include "input\input.h"
#include "convar.h"


namespace core {
namespace wyt{
namespace uiedit {


class UIEditor :
    public Singleton<UIEditor>,
    public ExecutionComponent
{
public:

    UIEditor(void);

    virtual bool Init(void);
    virtual bool Tick(Float deltaSeconds);
    virtual bool Render(void);

    UIComponent* GetCurrentComponent(void);
    void SetCurrentComponent(UIComponent* pComponent);

    void OnTouchScreenDown(TouchScreen* pScreen, int x, int y);
    void OnTouchScreenMoved(TouchScreen* pScreen, int x, int y);
    void OnTouchScreenUp(TouchScreen* pScreen, int x, int y);
    void OnTouchScreenPressed(TouchScreen* pScreen, int x, int y);
    void OnButtonDown(Joystick* pJoystick, Joystick::Button b);

    static ConVar<bool> m_bUIEditMode;

protected:

    UIComponent* m_pCurComponent;

    // Last known position of the stylus.
    int m_TouchX;
    int m_TouchY;
};


} // namespace uiedit
} // namespace wyt
} // namespace core


#endif // _UIEDITOR_H_