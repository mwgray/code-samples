
#include "ui\uieditor.h"
#include "ui\uimanager.h"
#include "bbrenderer.h"


namespace core {
namespace wyt {
namespace uiedit {


ConVar<bool> UIEditor::m_bUIEditMode("UIEditor::m_bUIEditMode", false, "Turns UI editor controls on/off");


UIEditor::UIEditor(void) :
    m_pCurComponent(NULL),
    m_TouchX(0),
    m_TouchY(0)
{

}

bool UIEditor::Init(void)
{
    Input::Instance().m_Down.memConnect(this, &OnTouchScreenDown);
    Input::Instance().m_Moved.memConnect(this, &OnTouchScreenMoved);
    Input::Instance().m_Up.memConnect(this, &OnTouchScreenUp);
    Input::Instance().m_Pressed.memConnect(this, &OnTouchScreenPressed);
    Input::Instance().m_ButtonPressed.memConnect(this, &OnButtonDown);

    return ExecutionComponent::Init();
}

bool UIEditor::Tick(Float deltaSeconds)
{
    return ExecutionComponent::Tick(deltaSeconds);
}

bool UIEditor::Render(void)
{
    if (!m_bUIEditMode || !m_pCurComponent)
        return ExecutionComponent::Render();

    BoundingBoxRenderer& bbRenderer = BoundingBoxRenderer::Instance();    

    int tlX = m_pCurComponent->GetAbsoluteX();
    int tlY = m_pCurComponent->GetAbsoluteY() + 192;
    int brX = tlX + m_pCurComponent->GetWidth();
    int brY = tlY + m_pCurComponent->GetHeight();
    
    Float3 topLeft     = Camera::Instance().TouchScreenToWorld(tlX, tlY);
    Float3 bottomRight = Camera::Instance().TouchScreenToWorld(brX, brY);

    Vector<Float, 3> tl(topLeft.x, topLeft.y);
    Vector<Float, 3> br(bottomRight.x, bottomRight.y);

    bbRenderer.RenderBox(tl, br);

    return ExecutionComponent::Render();
}

UIComponent* UIEditor::GetCurrentComponent(void)
{
    return m_pCurComponent;
}

void UIEditor::SetCurrentComponent(UIComponent* pComponent)
{
    m_pCurComponent = pComponent;
}

void UIEditor::OnTouchScreenDown(TouchScreen* pScreen, int x, int y)
{
    if (!m_bUIEditMode)
    {
        UIManager::Instance().OnTouch(pScreen, x, y);
        return;
    }
    
    std::vector<UIComponent*> vec;
    
    UIContainer* pContainer = (UIContainer*)UIManager::Instance().m_PageGraph.GetCurrentPage();
    
    if (pContainer != NULL)
        pContainer->HitTest(x, y, vec);

    // Normally we'd z-sort here, but UIComponents don't have a z value!
    //  They seem to draw in order, so the last one should be on top.
    m_pCurComponent = vec.empty() ? NULL : vec.back();
    
    m_TouchX = x;
    m_TouchY = y;
}

void UIEditor::OnTouchScreenMoved(TouchScreen* pScreen, int x, int y)
{
    if (!m_bUIEditMode)
    {
        UIManager::Instance().OnTouchMoved(pScreen, x, y);
        return;
    }

    if (m_pCurComponent == NULL) return;

    m_pCurComponent->SetX(m_pCurComponent->GetX() + (x - m_TouchX));
    m_pCurComponent->SetY(m_pCurComponent->GetY() + (y - m_TouchY));

    m_TouchX = x;
    m_TouchY = y;
}

void UIEditor::OnTouchScreenUp(TouchScreen* pScreen, int x, int y)
{
    if (!m_bUIEditMode)
    {
        UIManager::Instance().OnTouchReleased(pScreen, x, y);
        return;
    }
}

void UIEditor::OnTouchScreenPressed( TouchScreen* pScreen, int x, int y )
{
    if (!m_bUIEditMode)
    {
        UIManager::Instance().OnTouchPressed(pScreen, x, y);
    }

    return;
}

void UIEditor::OnButtonDown(Joystick* pJoystick, Joystick::Button b)
{
    if (!m_bUIEditMode)
    {
        UIManager::Instance().OnButtonDown(pJoystick, b);
        return;
    }

    //if (!m_bUIEditMode)
    //{
    //    if (!InFinal() && b == Joystick::Select)
    //        m_bUIEditMode = true;
    //    else
    //    {
    //        UIManager::Instance().OnButtonDown(pJoystick, b);
    //        return;
    //    }
    //}

    //else if (b == Joystick::Select)
    //    m_bUIEditMode = false;
}

} // namespace uiedit
} // namespace wyt
} // namespace core