#include "main.h"

void CContextManager::Create()
{
    context = Rml::CreateContext("main", Rml::Vector2i(1280, 800));

    if (!context)
    {
        CAppManager::Get()->Msg("Context couldn't be created!");
    }
}

Rml::Context* CContextManager::GetContext()
{
    return context;
}