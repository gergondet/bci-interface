#include "bci-interface/BCIInterface.h"

#include "private/BCIInterfaceImpl.h"

namespace bciinterface
{

BCIInterface::BCIInterface(unsigned int width, unsigned int height) : m_impl(new BCIInterfaceImpl(*this, width, height))
{}

bool BCIInterface::ParadigmStatus()
{
    return m_impl->ParadigmStatus();
}

void BCIInterface::StartParadigm()
{
    m_impl->StartParadigm();
}

void BCIInterface::StopParadigm()
{
    m_impl->StopParadigm();
}

void BCIInterface::AddEventHandler(EventHandler * handler)
{
    m_impl->AddEventHandler(handler);
}

void BCIInterface::SetBackground(Background * background)
{
    m_impl->SetBackground(background);
}

Background * BCIInterface::GetBackground() const
{
    return m_impl->GetBackground();
}

void BCIInterface::AddObject(DisplayObject * object)
{
    m_impl->AddObject(object);
}

void BCIInterface::AddNonOwnedObject(DisplayObject * object)
{
    m_impl->AddNonOwnedObject(object);
}

void BCIInterface::SetCommandReceiver(CommandReceiver * receiver)
{
    m_impl->SetCommandReceiver(receiver);
}

void BCIInterface::SetCommandOverrider(CommandOverrider * overrider)
{
    m_impl->SetCommandOverrider(overrider);
}

void BCIInterface::SetCommandInterpreter(CommandInterpreter * interpreter)
{
    m_impl->SetCommandInterpreter(interpreter);
}

CommandInterpreter * BCIInterface::GetCommandInterpreter()
{
    return m_impl->GetCommandInterpreter();
}

void BCIInterface::Clean()
{
    m_impl->Clean();
}

void BCIInterface::DisplayLoop(bool fullscreen)
{
    m_impl->DisplayLoop(fullscreen);
}

sf::RenderWindow * BCIInterface::DisplayLoop(sf::RenderWindow * app, bool fullscreen, int & cmd, float timeout)
{
    return m_impl->DisplayLoop(app, fullscreen, cmd, timeout);
}

sf::RenderWindow * BCIInterface::DisplayLoop(sf::RenderWindow * app, int & cmd, float timeout)
{
    return m_impl->DisplayLoop(app, true, cmd, timeout);
}

void BCIInterface::InitOculus()
{
    m_impl->InitOculus();
}

void BCIInterface::OculusDisplayLoop(int & cmd)
{
    m_impl->OculusDisplayLoop(cmd);
}

void BCIInterface::Close()
{
    m_impl->Close();
}

sf::RenderWindow * BCIInterface::GetRenderWindow()
{
    return m_impl->GetRenderWindow();
}

OculusWindow * BCIInterface::GetOculusWindow()
{
    return m_impl->GetOculusWindow();
}

float BCIInterface::GetRenderScale()
{
    return m_impl->GetRenderScale();
}

void BCIInterface::EnableFPSCounter(sf::Font & font)
{
    return m_impl->EnableFPSCounter(font);
}

} // namespace bciinterface

