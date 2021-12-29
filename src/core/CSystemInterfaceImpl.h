#pragma once
#include "utils/CSingleton.h"
#include <RmlUi/Core.h>
#include <chrono>

class CSystemInterfaceImpl : public Rml::SystemInterface, public CSingleton<CSystemInterfaceImpl>
{
    std::chrono::steady_clock::time_point startTime;
    double GetElapsedTime() override;
    void SetClipboardText(const Rml::String& text) override;
    void GetClipboardText(Rml::String& text) override;

    bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;

public:
    void SetStartTime();
};