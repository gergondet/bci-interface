#include <bci-interface/CommandInterpreter/CoshellInterpreter.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <coshell-client/CoshellClient.h>
#include <ctime>
#include <SFML/Window/Event.hpp>

namespace bciinterface
{

struct CoshellInterpreterImpl : public SimpleInterpreter
{
private:
    coshell::CoshellClient * m_coshell;
    bool m_own_coshell;
    bool m_initialized;
    bool m_end_paradigm;
    int  m_command;
    int  m_exit_command;
    std::vector<std::string> m_commands;
    std::string m_initialtest;
    std::vector<std::string> m_initialcommands;
    std::vector<std::string> m_finalcommands;
public:
    CoshellInterpreterImpl(const std::string & server_name, int server_port)
        : SimpleInterpreter(), m_coshell(new coshell::CoshellClient(server_name.c_str(), server_port)),
            m_own_coshell(true),
            m_initialized(false), m_end_paradigm(false), m_command(0), m_exit_command(-1),
            m_commands(0), m_initialtest(""), m_initialcommands(0), m_finalcommands(0)
    {
        if(!m_coshell || !m_coshell->Initialize())
        {
            std::cerr << "Error in coshell creation/initialization" << std::endl;
        }
    }

    CoshellInterpreterImpl(coshell::CoshellClient * coshell)
        : SimpleInterpreter(), m_coshell(coshell),
            m_own_coshell(false),
            m_initialized(false), m_command(0), m_exit_command(-1),
            m_commands(0), m_initialtest(""), m_initialcommands(0), m_finalcommands(0)
    {
    }

    ~CoshellInterpreterImpl()
    {
        if(m_own_coshell)
        {
            delete m_coshell;
        }
    }

    coshell::CoshellClient * GetCoshell()
    {
        return m_coshell;
    }

    void Reset()
    {
        m_commands.resize(0);
        m_initialtest = "";
        m_initialcommands.resize(0);
        m_finalcommands.resize(0);
        m_end_paradigm = false;
    }

    void SetExitCommand(int command)
    {
        m_exit_command = command;
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

    void SetInitialCommands(const std::string & command)
    {
        SetInitialCommands(std::vector<std::string>(1, command));
    }

    void SetFinalCommands(const std::vector<std::string> & commands)
    {
        m_finalcommands = commands;
    }

    void SetFinalCommands(const std::string & command)
    {
        SetFinalCommands(std::vector<std::string>(1, command));
    }

    std::string SendCommand(const std::string & command)
    {
        return m_coshell->ExecuteACommand(command);
    }

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        SimpleInterpreter::InterpretCommand(command, objects);
        if(m_initialized)
        {
            if(m_end_paradigm)
            {
                return true;
            }
            if(command >= 0 && command < (int)m_commands.size())
            {
                if(command != m_command)
                {
                    m_coshell->ExecuteACommand(m_commands[command]);
                    m_command = command;
                    return ( m_command != 0 && (m_exit_command == -1 || m_command == m_exit_command) );
                }
            }
        }
        return false;
    }

    void Initialize()
    {
        if(!m_initialized)
        {
            bool initial_test = true;
            if(m_initialtest.size() != 0)
            {
                std::string answer = m_coshell->ExecuteACommand(m_initialtest);
                std::string prefix = "Catch ";
                if(answer.compare(0, prefix.length(), prefix) != 0)
                {
                    initial_test = false;
                }
            }
            if(initial_test)
            {
                    for(size_t i = 0; i < m_initialcommands.size(); ++i)
                    {   
                        m_coshell->ExecuteACommand(m_initialcommands[i]);
                    }
            }
            m_initialized = true;
        }
    }

    void Process(sf::Event & event, const BCIInterface & interface)
    {
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
        {
            if(!m_initialized)
            {
                Initialize();
            }
            else
            {
                ExecuteFinalCommands();
            }
        }
    }

    void ExecuteFinalCommands()
    {
        for(size_t i = 0; i < m_finalcommands.size(); ++i)
        {
            m_coshell->ExecuteACommand(m_finalcommands[i]);
        }
        m_end_paradigm = true;
    }
};

CoshellInterpreter::CoshellInterpreter(const std::string & server_name, int server_port)
    : m_impl(new CoshellInterpreterImpl(server_name, server_port))
{
}

CoshellInterpreter::CoshellInterpreter(coshell::CoshellClient * coshell)
    : m_impl(new CoshellInterpreterImpl(coshell))
{
}

coshell::CoshellClient * CoshellInterpreter::GetCoshell()
{
    return m_impl->GetCoshell();
}

void CoshellInterpreter::Initialize()
{
    m_impl->Initialize();
}

void CoshellInterpreter::Reset()
{
    m_impl->Reset();
}

bool CoshellInterpreter::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

void CoshellInterpreter::Process(sf::Event & event, const BCIInterface & interface)
{
    m_impl->Process(event, interface);
}

void CoshellInterpreter::SetCommands(const std::vector<std::string> & commands)
{
    m_impl->SetCommands(commands);
}

void CoshellInterpreter::SetExitCommand(int command)
{
    m_impl->SetExitCommand(command);
}

void CoshellInterpreter::SetInitialTest(const std::string & test)
{
    m_impl->SetInitialTest(test);
}

void CoshellInterpreter::SetInitialCommands(const std::vector<std::string> & commands)
{
    m_impl->SetInitialCommands(commands);
}

void CoshellInterpreter::SetInitialCommands(const std::string & command)
{
    m_impl->SetInitialCommands(command);
}

void CoshellInterpreter::SetFinalCommands(const std::vector<std::string> & commands)
{
    m_impl->SetFinalCommands(commands);
}

void CoshellInterpreter::SetFinalCommands(const std::string & command)
{
    m_impl->SetFinalCommands(command);
}

void CoshellInterpreter::ExecuteFinalCommands()
{
    m_impl->ExecuteFinalCommands();
}

std::string CoshellInterpreter::SendCommand(const std::string & command)
{
    return m_impl->SendCommand(command);
}

} // namespace bciinterface

