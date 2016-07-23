// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui\uicontainer.h"
#include "ui\uimanager.h"
#include "ui\uilayer.h"

namespace core {

void UIContainer::Initialize(XmlElement* element)
{
    UIComponent::Initialize(element);

    // Get the number of children to instantiate
    std::vector<UIComponent*>::size_type numChildren = 0;
    XmlElement* childElement;
    for(childElement = element->FirstChildElement("UIComponent"); childElement != NULL; childElement = childElement->NextSiblingElement("UIComponent"))
    {
        numChildren++;
    }

    if(numChildren)
    {
        MEMORY_POOL("UIContainer");
        m_Children.reserve(numChildren);

        // instantiate and initialize the children    
        UIComponent* child;
        for(int i = 0; i < numChildren; ++i)
        {
            childElement = i ? childElement->NextSiblingElement("UIComponent") : element->FirstChildElement("UIComponent");
            child = UIManager::Instance().CreateUIComponent(childElement);

            if(child)
            {
                child->SetParent(this);

                if(child->m_InitState == NeedsInitializing)
                {
                    child->Initialize(childElement);
                }

                m_Children.push_back(child);
            }
        }
    }
}

void UIContainer::PostInitialize(XmlElement* element)
{
    UIComponent::PostInitialize(element);

    XmlElement* childElement = element->FirstChildElement("UIComponent");

    if(childElement)
    {
        std::vector<UIComponent*>::iterator i;
        for(i = m_Children.begin(); i != m_Children.end(); ++i)
        {
            if((*i)->m_InitState == NeedsPostInitializing)
            {
                (*i)->PostInitialize(childElement);
            }

            childElement = childElement->NextSiblingElement("UIComponent");
        }
    }
}

void UIContainer::Shutdown()
{
    Uncache();
    
    std::vector<UIComponent*>::iterator i;
    for(i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        UIComponent* pComponent = *i;

        if (pComponent != NULL)
        {
            if (pComponent->IsLayer())
            {
                if (!HNS_RELEASE_CAST(UILayer*, pComponent)->AddReference(-1))
                {
                    UILayer::RemoveLayer(pComponent->GetCRCName());
                }
            }
            else
            {
                pComponent->Shutdown();
                delete pComponent;
            }
        }
    }

    m_Children.clear();

    UIComponent::Shutdown();
}

void UIContainer::Uncache()
{
    UIComponent::Uncache();

    hdLog(("Caching", "UIContainer::Uncache()  %s\n", m_Name.c_str()));

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->Uncache();
    }
}

void UIContainer::Cache()
{
    UIComponent::Cache();

    hdLog(("Caching", "UIContainer::Cache()  %s\n", m_Name.c_str()));

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->Cache();
    }
}


void UIContainer::Render()
{
    UIComponent::Render();

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        UIComponent* pComponent = *i;

        if(pComponent->Is(Visible))
        {
            hdLog(("UIContainer::Rendering", "%s\n", pComponent->GetName()));
            pComponent->Render();
        }
    }
}

void UIContainer::Tick(Float deltaSeconds)
{
    UIComponent::Tick(deltaSeconds);

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        if((*i)->Is(Enabled))
        {
            (*i)->Tick(deltaSeconds);
        }
    }
}

void UIContainer::GetBounds(int& t, int& l, int& b, int& r)
{
    UIComponent::GetBounds(t, l, b, r);

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        UIComponent* child = *i;

        int ct, cl, cb, cr;
        child->GetBounds(ct, cl, cb, cr);

        t = Min(t, ct);
        l = Min(l, cl);
        b = Max(b, cb);
        r = Max(r, cr);
    }
}

UIComponent* UIContainer::FindChild(const char* string)
{
    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        UIComponent* child = *i;
        if(child->GetCRCName() == string)
        {
            return child;
        }
    }

    return NULL;
}

bool UIContainer::OnTouch(int tx, int ty)
{
    bool result = false;

    if(UIComponent::OnTouch(tx, ty))
    {
        return true;
    }

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        hdLog(("UI", "\nOnTouch %s child\n", (*i)->GetName()));

        // get out of here if we found the component that was pressed
        if((*i)->OnTouch(tx, ty))
        {
            hdLog(("UI", "\nTouched %s\n", (*i)->GetName()));
            result = true;
        }
    }

	return result;
}

void UIContainer::OnTouchMoved(int tx, int ty)
{
    UIComponent::OnTouchMoved(tx, ty);

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->OnTouchMoved(tx, ty);
    }
}

void UIContainer::OnTouchPressed(int tx, int ty)
{
    UIComponent::OnTouchPressed(tx, ty);

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->OnTouchPressed(tx, ty);
    }
}

bool UIContainer::OnTouchReleased(int tx, int ty)
{
    bool result = UIComponent::OnTouchReleased(tx, ty);

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        result = (*i)->OnTouchReleased(tx, ty);
    }

    return result;
}

bool UIContainer::OnButtonDown( Joystick* joy, Joystick::Button b )
{
    bool result = UIComponent::OnButtonDown(joy, b);

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        // get out of here if we found the component that was pressed
        if(true == (*i)->OnButtonDown(joy, b))
        {
            return true;
        }
    }

    return result;
}

void UIContainer::OnUIEvent( UIEvent event )
{
    UIComponent::OnUIEvent(event);

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->OnUIEvent(event);
    }
}

void UIContainer::Begin()
{
    UIComponent::Begin();

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->Begin();
    }
}

void UIContainer::End()
{
    UIComponent::End();

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->End();
    }
}

void UIContainer::Enter()
{
    UIComponent::Enter();

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->Enter();
    }
}

void UIContainer::Leave()
{
    UIComponent::Leave();

    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->Leave();
    }
}

void UIContainer::HitTest(int x, int y, std::vector<UIComponent*>& vec)
{
    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        (*i)->HitTest(x, y, vec);
    }
}

void UIContainer::SwapChild(UIComponent* pOld, UIComponent* pNew)
{
    for(std::vector<UIComponent*>::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        if(*i == pOld)
        {
            (*i) = pNew;
        }        
    }
}

} // namespace core
