#include "main.h"
#include "RmlUi/Core.h"
#include "RmlUi/Debugger.h"

int main(int argc, char* argv[])
{
    CAppManager::Get()->Create(1280, 720);
    CD3D11Manager::Get()->Create();

    CSystemInterfaceImpl::Get()->SetStartTime();

    Rml::SetSystemInterface(CSystemInterfaceImpl::Get());
    Rml::SetRenderInterface(CRenderInterfaceImpl::Get());

    CRenderInterfaceImpl::Get()->Init();

    if (!Rml::Initialise())
    {
        CAppManager::Get()->Msg("Failed to init RmlUi!");
    }

    CContextManager::Get()->Create();
    auto context = CContextManager::Get()->GetContext();

    Rml::Debugger::Initialise(context);
    Rml::Debugger::SetVisible(true);

    if (!Rml::LoadFontFace("arial.ttf"))
        CAppManager::Get()->Msg("Couldn't load font!");

    Rml::ElementDocument* document = context->LoadDocument("demo.rml");

    if (document)
        document->Show();
    else
        CAppManager::Get()->Msg("Document couldn't be created!");

    while (CAppManager::Get()->IsRunning())
    {
        CAppManager::Get()->HandleMessages();

        context->Update();

        CD3D11Manager::Get()->PreRender();
        context->Render();

        CD3D11Manager::Get()->Render();
    }

    Rml::Shutdown();
    Rml::Debugger::Shutdown();
}