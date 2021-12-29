#include "main.h"

double CSystemInterfaceImpl::GetElapsedTime()
{
    std::chrono::duration<double> secs = std::chrono::steady_clock::now() - startTime;
    return secs.count();
}

void CSystemInterfaceImpl::SetClipboardText(const Rml::String& text)
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

void CSystemInterfaceImpl::GetClipboardText(Rml::String& text)
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

bool CSystemInterfaceImpl::LogMessage(Rml::Log::Type type, const Rml::String& message)
{
    std::stringstream str;
    switch (type)
    {
    case Rml::Log::Type::LT_ERROR:
        str << "[ERROR]";
        break;
    case Rml::Log::Type::LT_ASSERT:
        str << "[ASSERT]";
        break;
    case Rml::Log::Type::LT_WARNING:
        str << "[WARNING]";
        break;
    case Rml::Log::Type::LT_INFO:
        str << "[INFO]";
        break;
    case Rml::Log::Type::LT_DEBUG:
        str << "[DEBUG]";
        break;
    default:
        str << "[DEFAULT]";
    }

    str << " " << message.c_str();

    printf("%s\n", str.str().c_str());

    return true;
}

void CSystemInterfaceImpl::SetStartTime()
{
    startTime = std::chrono::steady_clock::now();
}