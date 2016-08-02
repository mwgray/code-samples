// CORE - Copyright 2006 HumaNature Studios Inc.

#ifndef _CORE_UIIMAGE_H_
#define _CORE_UIIMAGE_H_

#include "ui\uicomponent.h"

#include "renderer\animatedtexture.h"

namespace core {

class Material;
class ImageIcon;

class UIImage:
    public UIComponent
{
public:
    UIImage();

    virtual ~UIImage() {}

    virtual void Initialize(XmlElement* element);

    virtual void Uncache();

    virtual void Load();

    virtual void Load(const char* path);

    virtual void Unload();

    virtual void Shutdown();

    virtual void Render();
    
    virtual void Tick(Float deltaSeconds);

    void SetOffsetPosition(Float x, Float y, Float z = 0);
    void GetOffsetPosition(Float &x, Float &y, Float &z);

    void SetIconPosition(int x, int y, int z);
    void GetIconPosition(int &x, int &y, int &z);

    virtual void Begin();   

    virtual void End();

    virtual void SetAlpha(int val);

    virtual void GetBounds(int& t, int& l, int& b, int& r);

    std::string GetTextureName() const { return m_TextureName; }
    void SetTextureName(std::string val) { m_TextureName = val; }

    unsigned int GetNumFrames() const { return m_NumFrames; }
    void SetNumFrames(unsigned int val) { m_NumFrames = val; }

    void SetAnimTexture(AnimatedTexture* animTex) { m_pTexture = animTex; }
    AnimatedTexture* GetAnimTexture() { return m_pTexture; }

    Material* GetMaterial() const { return m_pMaterial; }
    ImageIcon* GetImageIcon() { return m_pImageIcon; }

    virtual void SetRotationInDegrees(int degrees);

    virtual void Cache();

protected:
    //TODO JR: change over to AssetPointer<AnimatedTexture> ?
    AnimatedTexture* m_pTexture;
    Material* m_pMaterial;
    ImageIcon* m_pImageIcon;
    std::string m_TextureName;
    unsigned int m_NumFrames;
    bool m_Loaded;
    
    Float m_OffsetX;
    Float m_OffsetY;
    Float m_OffsetZ;
    bool m_SuperHack;
};

} // namespace core

#endif // _CORE_UIIMAGE_H_
