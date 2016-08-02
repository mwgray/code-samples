// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui\uiimage.h"

#include "renderer\material.h"
#include "icon\nitroimageicon.h"
#include "ui\uimanager.h"
#include "debug\convar.h"
#include "wyttypes.h"
#include "icon\overlayimageicon.h"

namespace core {

#pragma force_active on
REGISTER_UI_TYPE(UIImage)
#pragma force_active off

UIImage::UIImage() :
m_pTexture(NULL),
m_pMaterial(NULL),
m_pImageIcon(NULL),
m_NumFrames(0),
m_Loaded(false),
m_OffsetX(0),
m_OffsetY(0),
m_OffsetZ(0),
m_SuperHack(false)
{

}

void UIImage::Initialize(XmlElement* element)
{
    MEMORY_POOL("UIImage");
    UIComponent::Initialize(element);

    if(!GetElementValue(element, "Texture", m_TextureName))
    {
        hdWarning(("UI", "NO TEXTURE SPECIFIED in UIImage!"));
    }

    if(!GetElementValue(element, "frames", m_NumFrames))
    {
        hdWarning(("UI", "Could not find number of frames in %s", m_TextureName.c_str()));
    }

    GetElementValue(element, "SuperHack", m_SuperHack);

    m_Loaded = false;
}

void UIImage::Uncache()
{
    if (m_pImageIcon != NULL)
    {
        hdLog(("Caching", "Uncache()->UIIMage %s\n", m_Name.c_str()));
        m_pImageIcon->Uncache();
    }
}

void UIImage::Load()
{
    MEMORY_POOL("UIImage");
    
    m_pTexture = AnimatedTexture::Create(m_NumFrames);
    m_pTexture->SetCompressed(true);
    m_pTexture->SetFileName(m_TextureName.c_str());
    m_pTexture->Load();

    m_pMaterial = Material::Create();
    m_pMaterial->SetTexture(m_pTexture);

    m_pImageIcon = ImageIcon::Create(true);
    m_pImageIcon->SetMaterial(m_pMaterial);

    Uncache();
   // m_pImageIcon->SetTextureExtents(x, y, m_Width, m_Height);    // TODO JR MG: possibly using this in the future

    m_Loaded = true;
}

void UIImage::Load( const char* path )
{
    if(m_Loaded)
    {
        Unload();
    }

    m_TextureName = path;

    Load();
}

void UIImage::Unload()
{
    MEMORY_POOL("UIImage");

    if (m_pTexture != NULL)
        m_pTexture->Unload();

    HNS_DELETE(m_pTexture);
    HNS_DELETE(m_pMaterial);
    HNS_DELETE(m_pImageIcon);

    m_Loaded = false;
}

void UIImage::Shutdown()
{    
    Uncache();
    Unload();

    m_Loaded = false;

    UIComponent::Shutdown();
}

static int g_sZRenderValue = 100;
void UIImage::Render()
{
    if(Is(Visible))
    {
        G3_MtxMode(GX_MTXMODE_TEXTURE);
        G3_PushMtx();
        G3_Identity();
        G3_MtxMode(GX_MTXMODE_PROJECTION);
        G3_PushMtx();
        G3_Identity();
        G3_Ortho(FX32_ONE * 0,     FX32_ONE * 192,
            FX32_ONE * 0,     FX32_ONE * 256,
            FX32_ONE * -1024, FX32_ONE * 1024,
            NULL);
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
        G3_PushMtx();
        G3_Identity();

        ((NitroImageIcon*)m_pImageIcon)->m_SuperHack = m_SuperHack;
        m_pImageIcon->SetPosition( FX32i(GetAbsoluteX()) + m_OffsetX, FX32i(GetAbsoluteY()) + m_OffsetY, FX32i(g_sZRenderValue) + m_OffsetZ);
        m_pImageIcon->Render();

        G3_MtxMode(GX_MTXMODE_TEXTURE);
        G3_PopMtx(1);
        G3_MtxMode(GX_MTXMODE_PROJECTION);
        G3_PopMtx(1);
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
        G3_PopMtx(1);       
    }

    UIComponent::Render();
}

void UIImage::Tick(Float deltaSeconds)
{
    if(m_NumFrames > 1)
    {
        m_pTexture->SetFrame(WiggleFrame::Instance().GetWiggleFrame() % m_NumFrames);
    }
    else
    {
        m_pTexture->SetFrame(0);
    }

    UIComponent::Tick(deltaSeconds);
}

void UIImage::SetOffsetPosition( Float x, Float y, Float z /*= 0*/ )
{
    m_OffsetX = x;
    m_OffsetY = y;
    m_OffsetZ = z;
}

void UIImage::GetOffsetPosition( Float &x, Float &y, Float &z )
{
    x = m_OffsetX;
    y = m_OffsetY;
    z = m_OffsetZ;
}

void UIImage::Begin()
{
    if (!m_Loaded)
    {
        Load();
    }
    else
    {
        Uncache();
    }

    UIComponent::Begin();
}

void UIImage::End()
{
    UIComponent::End();

    Uncache();
}

void UIImage::SetAlpha( int val )
{
    m_pImageIcon->SetAlpha(val);
}

void UIImage::SetIconPosition( int x, int y, int z )
{
    m_pImageIcon->SetPosition(x, y, z);
}

void UIImage::GetIconPosition( int &x, int &y, int &z )
{
    m_pImageIcon->GetPosition(x, y, z);
}

void UIImage::GetBounds( int& t, int& l, int& b, int& r )
{
    t = GetAbsoluteY() + FX_Whole(m_OffsetY);
    l = GetAbsoluteX() + FX_Whole(m_OffsetX);
    b = t + static_cast<int>(m_Height);
    r = l + static_cast<int>(m_Width);
}

void UIImage::SetRotationInDegrees( int degrees )
{
    m_pImageIcon->SetRotationInDegrees(degrees);
}

void UIImage::Cache()
{
    if (m_pTexture)
    {
        hdLog(("Caching", "Cache()->UIIMage %s\n", m_Name.c_str()));
        m_pTexture->Cache();
    }
}

} // namespace core
