#pragma once
#include "utils/CSingleton.h"
#include <RmlUi/Core.h>
#include <chrono>

class CSystemInterfaceImpl : public Rml::SystemInterface, public CSingleton<CSystemInterfaceImpl>
{
    std::chrono::steady_clock::time_point startTime;
    double GetElapsedTime() override
    {
        std::chrono::duration<double> secs = std::chrono::steady_clock::now() - startTime;
        return secs.count();
    }

    void SetClipboardText(const Rml::String& text) override
    {
        if (OpenClipboard(CAppManager::Get()->GetWindow()))
        {
            OpenClipboard(0);
            EmptyClipboard();
            HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size());
            if (!hg) {
                CloseClipboard();
                return;
            }
            memcpy(GlobalLock(hg), text.c_str(), text.size());
            GlobalUnlock(hg);
            SetClipboardData(CF_UNICODETEXT, hg);
            CloseClipboard();
            GlobalFree(hg);
        }
    }

    void GetClipboardText(Rml::String& text) override
    {
        if (OpenClipboard(CAppManager::Get()->GetWindow()))
        {
            HANDLE hData = GetClipboardData(CF_UNICODETEXT);
            if (hData == nullptr)
                return;

            wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
            if (pszText == nullptr)
                return;

            text = Rml::String();
            GlobalUnlock(hData);
            CloseClipboard();
        }
    }

public:
    void SetStartTime()
    {
        startTime = std::chrono::steady_clock::now();
    }
};