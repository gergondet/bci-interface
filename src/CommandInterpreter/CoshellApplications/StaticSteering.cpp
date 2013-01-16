#include <bci-interface/CommandInterpreter/CoshellApplications/StaticSteering.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-client/CoshellClient.h>

#include <SFML/Graphics.hpp>

#include <sys/time.h>
#include <sstream>

namespace bciinterface
{

struct StaticSteeringImpl
{
private:
    coshell::CoshellClient * m_coshell;
    bool m_stop;
    int prev_cmd;
public:
    StaticSteeringImpl(coshell::CoshellClient * coshell)
        : m_coshell(coshell), m_stop(false)
    {
        if(m_coshell)
        {
            m_coshell->ExecuteACommand("import walking/startherdt");
            m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.0)");
        }
    }

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        if(!m_coshell) { return false; }
        if(m_stop)
        {
            return true;
        }
        if(prev_cmd == command) 
        { 
            return false; 
        }
        prev_cmd = command;
        /* 1: Up, 2: Right, 3: Down, 4: Left, 5: stop */
        std::stringstream cmd;
        switch(command)
        {
            case 0:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.0)");
                break;
            case 1:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](0.1,0.0,0.0)");
                break;
            case 2:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,-0.15)");
                break;
            case 3:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.1,0.0,0.0)");
                break;
            case 4:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.15)");
                break;
            case 5:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](0,0,0)");
                return true;
                break;
        }
        return false;
    }

    void Process(sf::Event & event)
    {
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
        {
            m_stop = true;
            if(m_coshell) { m_coshell->ExecuteACommand("set pg.velocitydes [3](0,0,0)"); }
        }
    }
};

void StaticSteering::Process(sf::Event & event)
{
    m_impl->Process(event);
}

StaticSteering::StaticSteering(coshell::CoshellClient * coshell)
: m_impl(new StaticSteeringImpl(coshell))
{}

bool StaticSteering::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    SimpleInterpreter::InterpretCommand(command, objects);
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

