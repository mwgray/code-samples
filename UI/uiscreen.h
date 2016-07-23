// CORE - Copyright 2007 HumaNature Studios Inc.

#ifndef _CORE_UISCREEN_H_
#define _CORE_UISCREEN_H_

#include "core.h"

#include "uicomponent.h"

#include "renderer/nitrobackground2d.h"
#include "renderer/nitroimage2d.h"
#include "renderer/nitropalette2d.h"

namespace core {

template<int i_NumScreens, int i_NumImages>
class UIScreenTmpl:
    public UIComponent
{
public:
    NitroPalette2D& GetPalette()
    {
        MEMORY_POOL("UIScreen");
        return m_NitroPalette2D;
    }

    ~UIScreenTmpl()
    {
        MEMORY_POOL("UIScreen");
        Unload();
    }
    
    void Initialize(XmlElement* pXmlElement)
    {
        MEMORY_POOL("UIScreen");
        UIComponent::Initialize(pXmlElement);

        std::string path;
        if(FromXMLElement(pXmlElement, "Path", path))        
        {
            std::string vramLocationString;
            if(FromXMLElement(pXmlElement, "VRAMLocation", vramLocationString))
            {
                VRAMLocation vramLocation = VRAMLocationFromString(vramLocationString.c_str());        

                Setup(path.c_str(), vramLocation);
            }
        }
        else
        {
            hdWarning(("XML", "%s is missing path\n", m_Name.c_str()));
        }
    }

    void Begin()
    {
        MEMORY_POOL("UIScreen");
        UIComponent::Begin();

        Load();
        Cache();
        //Unload();
    }

    void End()
    {
        MEMORY_POOL("UIScreen");
        UIComponent::End();

        Uncache();
        Unload();
    }

    void Setup(const char* pPath, VRAMLocation vramLocation)
    {        
        MEMORY_POOL("UIScreen");

        m_NitroPalette2D.Setup(pPath, VRAMPaletteLocation(vramLocation), false);
        m_NitroPalette2D.SetRanged(true);
        m_NitroPalette2D.SetPaletteEnd(192);

        for(int i = 0; i < s_NumScreens; ++i)
        {
            std::string path = pPath;
            if(i)
            {
                path += "_";
                path += ToString(i);
            }
            m_NitroBackground2D[i].Setup(path.c_str(), VRAMScreenLocation(vramLocation), false, i * 0x1000);
        }

        for(int i = 0; i < s_NumImages; ++i)
        {
            std::string path = pPath;
            if(i)
            {
                path += "_";
                path += ToString(i);
            }
            m_NitroImage2D[i].Setup(path.c_str(), VRAMCharacterLocation(vramLocation), false);
        }
    }

    void SetupBlankScreen(VRAMLocation vramLocation)
    {
        MEMORY_POOL("UIScreen");
        Setup("$general\\blank", vramLocation);
    }

    void Load()
    {
        MEMORY_POOL("UIScreen");
        m_NitroPalette2D.Load();

        for(int i = 0; i < s_NumScreens; ++i)
        {
            m_NitroBackground2D[i].Load();
        }

        for(int i = 0; i < s_NumImages; ++i)
        {            
            m_NitroImage2D[i].Load();
        }

        // force color0 to white
        //m_NitroPalette2D.SetColor(0, White);
    }

    void Unload()
    {
        MEMORY_POOL("UIScreen");
        m_NitroPalette2D.Unload();

        for(int i = 0; i < s_NumScreens; ++i)
        {
            m_NitroBackground2D[i].Unload();
        }

        for(int i = 0; i < s_NumImages; ++i)
        {            
            m_NitroImage2D[i].Unload();
        }
    }

    void Cache()
    {
        MEMORY_POOL("UIScreen");
        m_NitroPalette2D.Cache();

        // reset vram locations
        if(VRAMIsMain(m_NitroImage2D[0].GetVramLocation()))
        {
            G2_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_512x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000);
        }
        else
        {
            G2S_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_512x512, GX_BG_COLORMODE_256, GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000);
        }

        for(int i = 0; i < s_NumScreens; ++i)
        {
            m_NitroBackground2D[i].Cache();
        }

        int offset = 0;
        for(int i = 0; i < s_NumImages; ++i)
        {            
            m_NitroImage2D[i].SetOffset(offset);
            m_NitroImage2D[i].Cache();
            offset += m_NitroImage2D[i].GetVRAMSize();
        }
    }

    void Uncache()
    {
        MEMORY_POOL("UIScreen");
        m_NitroPalette2D.Uncache();

        for(int i = 0; i < s_NumScreens; ++i)
        {
            m_NitroBackground2D[i].Uncache();
        }

        for(int i = 0; i < s_NumImages; ++i)
        {            
            m_NitroImage2D[i].Uncache();
        }
    }

protected:
    static const int s_NumScreens = i_NumScreens;
    static const int s_NumImages = i_NumImages;

    NitroBackground2D m_NitroBackground2D[s_NumScreens];
    NitroImage2D      m_NitroImage2D[s_NumImages];
    NitroPalette2D    m_NitroPalette2D;
};

typedef UIScreenTmpl<1, 1> UIScreen;
typedef UIScreenTmpl<1, 3> UINewScreen;

} // namespace core

#endif // _CORE_UISCREEN_H_