#include <bci-interface/CommandOverrider.h>

#include <iostream>
#include <map>

namespace bciinterface
{

struct CommandOverriderImpl
{
public:
    CommandOverriderImpl() : m_command(0), m_override(false)
    {
    }

    void Process(sf::Event & event, const BCIInterface & interface)
    {
        if(m_keyboard_override.size() != 0)
        {
            if(event.type == sf::Event::KeyPressed && ! m_override)
            {
                std::map<sf::Keyboard::Key, int>::iterator it = m_keyboard_override.find(event.key.code);
                if(it != m_keyboard_override.end())
                {
                    m_override = true;
                    m_command = it->second;
                }
                return;
            }
            if(event.type == sf::Event::KeyReleased && m_override)
            {
                std::map<sf::Keyboard::Key, int>::iterator it = m_keyboard_override.find(event.key.code);
                if(it != m_keyboard_override.end())
                {
                    if(it->second == m_command)
                    {
                        m_override = false;
                        m_command = 0;
                    }
                }
                return;
            }
        }
        if(m_mouse_override.size() != 0)
        {
            if(event.type == sf::Event::MouseButtonPressed && ! m_override)
            {
                std::map<sf::Mouse::Button, int>::iterator it = m_mouse_override.find(event.mouseButton.button);
                if(it != m_mouse_override.end())
                {
                    m_override = true;
                    m_command = it->second;
                }
                return;
            }
            if(event.type == sf::Event::MouseButtonReleased && m_override)
            {
                std::map<sf::Mouse::Button, int>::iterator it = m_mouse_override.find(event.mouseButton.button);
                if(it != m_mouse_override.end())
                {
                    if(it->second == m_command)
                    {
                        m_override = false;
                        m_command = 0;
                    }
                }
                return;
            }
        }
        if(m_joystick_override.size() != 0)
        {
            if(event.type == sf::Event::JoystickButtonPressed && ! m_override)
            {
                std::map<unsigned int, int>::iterator it = m_joystick_override.find(event.joystickButton.button);
                if(it != m_joystick_override.end())
                {
                    m_override = true;
                    m_command = it->second;
                }
                return;
            }
            if(event.type == sf::Event::JoystickButtonReleased && m_override)
            {
                std::map<unsigned int, int>::iterator it = m_joystick_override.find(event.joystickButton.button);
                if(it != m_joystick_override.end())
                {
                    if(it->second == m_command)
                    {
                        m_override = false;
                        m_command = 0;
                    }
                }
                return;
            }
        }
    }

    /* Handle keyboards events */
    void AddOverrideCommand(sf::Keyboard::Key ekey, int command)
    {
        if(m_keyboard_override.count(ekey))
        {
            std::cerr << "The key you tried to bound to command " << command << " is already used to bound " << m_keyboard_override[ekey] << std::endl;
            return;
        }
        m_keyboard_override[ekey] = command;
    }

    /* Handle mouse click events */
    void AddOverrideCommand(sf::Mouse::Button ebutton, int command)
    {
        if(m_mouse_override.count(ebutton))
        {
            std::cerr << "The mouse button you tried to bound to command " << command << " is already used to bound " << m_mouse_override[ebutton] << std::endl;
            return;
        }
        m_mouse_override[ebutton] = command;
    }

    /* Handle joypad button events */
    void AddOverrideCommand(unsigned int button, int command)
    {
        if(m_joystick_override.count(button))
        {
            std::cerr << "The joystick button you tried to bound to command " << command << " is already used to bound " << m_joystick_override[button] << std::endl;
            return;
        }
        m_joystick_override[button] = command;
    }

    int GetCommand()
    {
        return m_command;
    }

    int IsOverriding()
    {
        return m_override;
    }

private:
    int m_command;
    bool m_override;
    std::map<sf::Keyboard::Key, int> m_keyboard_override;
    std::map<sf::Mouse::Button, int> m_mouse_override;
    std::map<unsigned int, int> m_joystick_override;
};

CommandOverrider::CommandOverrider() : m_impl(new CommandOverriderImpl())
{
}

void CommandOverrider::Process(sf::Event & event, const BCIInterface & interface)
{
    m_impl->Process(event, interface);
}

void CommandOverrider::AddOverrideCommand(sf::Keyboard::Key ekey, int command)
{
    m_impl->AddOverrideCommand(ekey, command);
}

void CommandOverrider::AddOverrideCommand(sf::Mouse::Button ebutton, int command)
{
    m_impl->AddOverrideCommand(ebutton, command);
}

void CommandOverrider::AddOverrideCommand(unsigned int button, int command)
{
    m_impl->AddOverrideCommand(button, command);
}

int CommandOverrider::GetCommand()
{
    return m_impl->GetCommand();
}

bool CommandOverrider::IsOverriding()
{
    return m_impl->IsOverriding();
}

} // namespace bciinterface

