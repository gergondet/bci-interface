#include <bci-interface/CoshellInterpreter.h>

#include <bci-interface/SimpleInterpreter.h>
#include <coshell-bci/CoshellClient.h>
#include <ctime>
#include <SFML/Window/Event.hpp>

namespace bciinterface
{

struct CoshellInterpreterImpl : public SimpleInterpreter
{
private:
    coshellbci::CoshellClient * m_coshell;
    bool m_initialized;
    int  m_command;
    std::vector<std::string> m_commands;
    std::string m_initialtest;
    std::vector<std::string> m_initialcommands;
public:
    CoshellInterpreterImpl(const std::string & server_name, int server_port)
        : SimpleInterpreter(), m_coshell(new coshellbci::CoshellClient(server_name.c_str(), server_port)),
            m_initialized(false), m_command(0), 
            m_commands(0), m_initialtest(""), m_initialcommands(0)
    {
        if(!m_coshell || !m_coshell->Initialize())
        {
            std::cerr << "Error in coshell creation/initialization" << std::endl;
        }
    }

    ~CoshellInterpreterImpl()
    {
        delete m_coshell;
    }

    void SetCommands(const std::vector<std::string> & commands)
    {
        m_commands = commands;
    }

    void SetInitialTest(const std::string & test)
    {
        m_initialtest = test;
    }

    void SetInitialCommands(const std::vector<std::string> & commands)
    {
        m_initialcommands = commands;
    }

    void InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        SimpleInterpreter::InterpretCommand(command, objects);
        if(m_initialized)
        {
            if(command >= 0 && command < (int)m_commands.size())
            {
                if(command != m_command)
                {
                    m_coshell->ExecuteACommand(m_commands[command]);
                    m_command = command;
                }
            }
        }
    }

    void Process(sf::Event & event)
    {
        if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Space)
        {
            if(!m_initialized)
            {
                if(m_initialtest.size() != 0)
                {
                    std::string answer = m_coshell->ExecuteACommand(m_initialtest);
                    std::string prefix = "Catch ";
                    if(answer.compare(0, prefix.length(), prefix) == 0)
                    {
                        for(size_t i = 0; i < m_initialcommands.size(); ++i)
                        {
                            m_coshell->ExecuteACommand(m_initialcommands[i]);
                        }
                    }
                }
                m_initialized = true;
            }
        }
    }
};

CoshellInterpreter::CoshellInterpreter(const std::string & server_name, int server_port)
    : m_impl(new CoshellInterpreterImpl(server_name, server_port))
{
}

void CoshellInterpreter::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    m_impl->InterpretCommand(command, objects);
}

void CoshellInterpreter::Process(sf::Event & event)
{
    m_impl->Process(event);
}

void CoshellInterpreter::SetCommands(const std::vector<std::string> & commands)
{
    m_impl->SetCommands(commands);
}

void CoshellInterpreter::SetInitialTest(const std::string & test)
{
    m_impl->SetInitialTest(test);
}

void CoshellInterpreter::SetInitialCommands(const std::vector<std::string> & commands)
{
    m_impl->SetInitialCommands(commands);
}

} // namespace bciinterface

