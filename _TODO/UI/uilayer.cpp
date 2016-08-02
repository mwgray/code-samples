// CORE - Copyright 2006 HumaNature Studios Inc.

#include "ui\uilayer.h"

#include "ui\uimanager.h"
#include "ui\navbar.h"
#include "audio\nitromusicdriver.h"
#include "audio\WYT_music.sadl"

#include "file/filesystem.h"
#include "xml/XmlDocument.h"

#define UI_STOP_MUSIC (-1)

namespace core {

UILayer::LayerMap  UILayer::m_LayerMap;
StringTableCollection UILayer::m_StringMap;

UILayer::UILayer() :
m_OnTimerUIEvent(""),
m_Playlist("none"),
m_TimedEventTime(0),
m_RefCount(0),
m_TimerEventFired(false),
m_Timer(0),
m_UseStringTable(false),
m_NeedPostInit(true),
m_pReference(NULL)
{
    MEMORY_POOL("UILayer");

    memset(m_NavbarComponents, 0, sizeof(bool) * NavbarComponent_Total);
}

void UILayer::Initialize(XmlElement* element)
{
    TIMING_POOL("UILayer::Initialize");

    MEMORY_POOL("UILayer");

    int nElemValue;
    float fElemValue;
    std::string strElemValue;

    UIContainer::Initialize(element);    

    if (GetElementValue(element, "UseStringTable", nElemValue) && nElemValue != 0)
    {
        m_UseStringTable = true;
    }
/*
    s32 musicIDs[10];
    s32 bankIDs[10];
    int curID = 0;
    int volume = 127;

    XmlElement* pElem = element->FirstChildElement("Playlist");

    if (pElem)
    {
        FromXMLAttribute(pElem, "name",   m_Playlist, true);    
        FromXMLAttribute(pElem, "volume", volume,     false);

        // Get all song ID's for the playlist.
        while (true)
        {
            pElem = curID == 0 ? pElem->FirstChildElement("MusicID") : pElem->NextSiblingElement("MusicID");
            
            if (!pElem) break;
            
            const char* pValue = pElem->GetText();
            if (pValue)
                FromString(pValue, nElemValue);

            if (nElemValue == UI_STOP_MUSIC)
            {
                m_Playlist = "UI_STOP_MUSIC";
                break;
            }

            musicIDs[curID] = nElemValue;
            bankIDs[curID++] = BANK_002_11k; // Need to move this into .layer files eventually.
        }

        // Add the playlist.
        if (m_Playlist != "none" && m_Playlist != "UI_STOP_MUSIC" && curID > 0)
            NitroMusicDriver::Instance().AddPlaylist(m_Playlist, true, curID, musicIDs, bankIDs, (unsigned char)volume);
    }
    else
    {
*/        m_Playlist = "none";
//    }

    m_OnTimerUIEvent = GetElementValue(element, "OnTimer", strElemValue) ? strElemValue.c_str() : "";

    if(GetElementValue(element, "TimedEventTime", fElemValue))
        m_TimedEventTime = FX32f(fElemValue);

    // look for navbar goodies
    for(int i = 0; i < NavbarComponent_Total; ++i)
    {
        string name = NavbarComponentToString(i);
        name += "_Enable";
        int enabled = 0;

        hdLog(("UI", "Looking for %s\n", name.c_str()));

        if(GetElementValue(element, name.c_str(), enabled))
        {
            m_NavbarComponents[i] = (enabled != 0);
            hdLog(("UI", "Setting %s to %s\n", NavbarComponentToString(i), m_NavbarComponents[i] ? "true" : "false"));
        }
    }

    m_NeedPostInit = true;
}

void UILayer::Shutdown()
{
    MEMORY_POOL("UILayer");

    NitroMusicDriver::Instance().RemovePlaylist(m_Playlist);

    UIContainer::Shutdown();
}

void UILayer::DestroyLayers(std::list<CompactString>& layerList)
{
    MEMORY_POOL("UILayer");

    for (std::list<CompactString>::iterator layerIter = layerList.begin(); layerIter != layerList.end(); ++layerIter)
    {
        CompactString& layerName = *layerIter;
        LayerMap::iterator mapIter = m_LayerMap.find(layerName);

        if (mapIter != m_LayerMap.end())
        {
            UILayer& layer = *(*mapIter).second;
            
            if (!layer.AddReference(-1))
            {
                layer.Shutdown();
                delete &layer;
                m_LayerMap.erase(mapIter);
            }
        }
    }
}

void UILayer::DestroyAllLayers()
{
    MEMORY_POOL("UILayer");

    for (LayerMap::iterator iter = m_LayerMap.begin(); iter != m_LayerMap.end(); ++iter)
    {
        UILayer* pLayer = (*iter).second;

        pLayer->Shutdown();
        delete pLayer;
    }

    m_LayerMap.clear();
}

void UILayer::RemoveLayer(CompactString layerName)
{
    MEMORY_POOL("UILayer");

    LayerMap::iterator mapIter = m_LayerMap.find(layerName);
    RemoveLayer(mapIter);
}

UILayer::LayerMap::iterator  UILayer::RemoveLayer(UILayer::LayerMap::iterator layerIter)
{
    MEMORY_POOL("UILayer");

    LayerMap::iterator retIter = m_LayerMap.end();

    if(layerIter != m_LayerMap.end())
    {
        UILayer& layer = *(*layerIter).second;

        hdLog(("UILayer", "RemoveLayer(%s)\n", layer.GetName()));

        layer.Shutdown();
        delete &layer;
        retIter = m_LayerMap.erase(layerIter);
    }

    return retIter;
}

void UILayer::AddLayer(const char* name, UILayer* pUILayer)
{
    MEMORY_POOL("UI Layer Map");
    hdAssertMsg(m_LayerMap.find(name) == m_LayerMap.end(), ("Layer %s is already registered!", name));

    m_LayerMap[name] = pUILayer;
}

UILayer* UILayer::LoadLayer(const char* name)
{
    AUTO_MEMORY_POOL();

    hdLog(("UILayer", "LoadLayer(%s)\n", name));

    std::string filename = "ui/layer/";
    filename += name;
    filename = filename + ".layer";    
    FileID fileID = FileSystem::Instance().GetFileID(filename.c_str());

    xml::Document document;
    document.LoadFinal(filename);
    
    UILayer* pUILayer;
    {
        MEMORY_POOL("UI Layer Instances");
        pUILayer = (UILayer*)UIManager::Instance().CreateUIComponent(document);
        pUILayer->m_Name = name;
        pUILayer->m_FileID = fileID;
    }

    AddLayer(name, pUILayer);

    {
        MEMORY_POOL("UI Layer Inits");
        // just initialize; post will get hit by the UIPage PostInit
        pUILayer->Initialize(document);
    }

    pUILayer->AddReference(1);

    return pUILayer;
}

void UILayer::PostInitializeLayers()
{
    MEMORY_POOL("UILayer");

    for(LayerMap::iterator i = m_LayerMap.begin(); i != m_LayerMap.end(); ++i)
    {
        UILayer& layer = *(*i).second;        
        layer.SpecialPostInitialize();
    }
}

void UILayer::SpecialPostInitialize()
{
    if(m_NeedPostInit)
    {
        TIMING_POOL("UILayer::SpecialPostInitialize()");

        AUTO_MEMORY_POOL();
        // do not use the element; it is passed in from the page
        // instead, open the file and do a proper post init
        xml::Document document;
        document.LoadFinal(m_FileID);

        // do post
        PostInitialize(document);
        m_NeedPostInit = false;
    }
}

void UILayer::LoadStringTable(const char* pName, bool clearTableBeforeLoading/* = true*/)
{
    MEMORY_POOL("UILayer::StringTable");

    if(clearTableBeforeLoading)
    {
        m_StringMap.clear();
    }
    
    std::string filename = "$strings\\";
    filename += pName;
    filename += ".StringTable";

    m_StringMap.AddStringTableFinal(filename.c_str());
}

void UILayer::Begin()
{
    MEMORY_POOL("UILayer");

    hdLog(("UI", "\nEnter %s layer\n", m_Name.c_str()));

    UIContainer::Begin();

    NitroMusicDriver& music = NitroMusicDriver::Instance();

    // reset timer goodies
    m_Timer = 0;
    m_TimerEventFired = false;

    if (m_Playlist == "UI_STOP_MUSIC")
        music.Stop();
    else if (m_Playlist != "none" && !music.PlaylistsAreSame(m_Playlist, music.GetCurPlaylistName()))
    {
        music.SetCurrentPlaylist(m_Playlist);
        music.Play();
    }

    if(m_UseStringTable)
    {
        for (std::vector<UIComponent*>::iterator iter = m_Children.begin(); iter != m_Children.end(); ++iter)
        {
            UIComponent* pComponent = *iter;
            
            if (!pComponent->IsText())
                continue;
            
            string text = GetLocalizedString(pComponent->GetCRCName());

            if(text != "")
            {
            	hdLog(("UI", "Setting string: %s\n", text.c_str()));
            	
                UIText* pUIText = HNS_RELEASE_CAST(UIText*,pComponent);
                hdNullAssert(pUIText);
                pUIText->SetText(text.c_str());
            }
        }
    }

    // set up navbar
    UINavbar* pNavbar = UIManager::Instance().GetNavbar();
    hdLog(("UI", "Navbar %X\n", pNavbar));
    if(pNavbar)
    {
        for(int i = 0; i < NavbarComponent_Total; ++i)
        {
            if(m_NavbarComponents[i])
            {
                pNavbar->SetActive(UILayer::NavbarComponent(i), true);
                hdLog(("UI", "Turning %s %s\n", m_NavbarComponents[i] ? "on" : "off", NavbarComponentToString(i)));
            }
        }
    }
}

void UILayer::Tick(Float deltaSeconds)
{
    MEMORY_POOL("UILayer");

    UIContainer::Tick(deltaSeconds);

    if(m_OnTimerUIEvent != "" && (m_TimerEventFired == false))
    {
        m_Timer += deltaSeconds;

        if(m_Timer >= m_TimedEventTime)
        {
            UIManager::Instance().OnUIEvent(m_OnTimerUIEvent);
            m_TimerEventFired = true;
        }
    }
}

bool UILayer::IsLayer()
{
    MEMORY_POOL("UILayer");

    return true;
}

bool UILayer::AddReference(int ref)
{
    MEMORY_POOL("UILayer");

    m_RefCount += ref;

    hdLog(("UILayerReference", "AddReference(%d) on %s(%X); m_RefCount = %d\n", ref, m_Name.c_str(), this, m_RefCount));

    return m_RefCount > 0;
}

std::string UILayer::GetLocalizedString(const CompactString& key)
{
    MEMORY_POOL("UILayer");

    const char* pString = m_StringMap[key];

    hdLog(("StringTable", "%s 0x%08X:", key.c_str(), pString));

    if(pString)
    {
        hdLog(("StringTable", " %s\n", pString));
        return pString;
    }
    else
    {
        hdLog(("StringTable", " null\n"));
        return "";
    }    
}

void UILayer::End()
{
    UIContainer::End();

    if(m_pReference)
    {
        m_pReference->End();
    }
}

void UILayer::CleanupLayers()
{
    hdLog(("UILayer", "%d layer(s) in the map.\n", m_LayerMap.size()));

    for(LayerMap::iterator i = m_LayerMap.begin(); i != m_LayerMap.end();)
    {
        UILayer& layer = *(*i).second;

        hdLog(("UILayerReference", "Layer %s has %d references, %s!\n", 
            layer.m_Name.c_str(),
            layer.m_RefCount,
            layer.m_RefCount <= 0 ? "removing" : "keeping"
            ));

        if(layer.m_RefCount <= 0)
        {
            i = RemoveLayer(i);
        }
        else
        {
            ++i;
        }
    }    
}

UILayer* UILayer::FindLayer(const char* name, bool assertOnFail)
{
    if(m_LayerMap.find(name) == m_LayerMap.end())
    {   
        if(assertOnFail)
        {
            hdWarning(("UILayer", "Could not find Layer %s!\nAvailableLayers:\n", name));

            for (LayerMap::iterator iter = m_LayerMap.begin(); iter != m_LayerMap.end(); ++iter)
            {
                UILayer* pLayer = (*iter).second;
                hdWarning(("UILayer", "%s, %d, %s, %s, %d\n", 
                    iter->first.c_str(), 
                    iter->first.m_CRC, 
                    pLayer->GetName(),
                    CompactString(name).c_str(),
                    CompactString(name).m_CRC
                    ));
            }
            hdHalt();

        }
        return NULL;
    }
    else
    {
        return m_LayerMap[name];
    }
}

#pragma force_active on
REGISTER_UI_TYPE(UILayer)
#pragma force_active off

} // namespace core