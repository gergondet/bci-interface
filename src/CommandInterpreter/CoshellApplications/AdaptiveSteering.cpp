#include <bci-interface/CommandInterpreter/CoshellApplications/AdaptiveSteering.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-bci/CoshellClient.h>
#include <visionsystem/vs_plugins/recognition/recognition.h>

namespace bciinterface
{

struct AdaptiveSteeringImpl : public SimpleInterpreter
{
private:
    coshellbci::CoshellClient * m_coshell;
    Recognition * m_recognition;
    double m_time_in;
    int prev_cmd;
public:
    AdaptiveSteeringImpl(coshellbci::CoshellClient * coshell, Recognition * recognition)
        : m_coshell(coshell), m_recognition(recognition), m_time_in(0), prev_cmd(-1)
    {
        m_coshell->ExecuteACommand("StartWalking");
        m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.0)");
    }

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        SimpleInterpreter::InterpretCommand(command, objects);
        /* Update table stim position according to recognition plugin */
        vision::ImageRef cupboard_pos = m_recognition->GetObjectPosition("table");
        if(cupboard_pos.x != 0 && cupboard_pos.y != 0)
        {
            objects[4]->SetPosition(cupboard_pos.x, cupboard_pos.y);
        }
        else
        {
            objects[4]->Unhighlight();
        }
        if(prev_cmd == command) 
        { 
            return false; 
        }
        else if(command == 0)
        {
            double time_now;
            timeval tv;
            gettimeofday(&tv, 0);
            time_now = 1000000*tv.tv_sec + tv.tv_usec;
            if(time_now < m_time_in + 3000000) { return false; }
            m_time_in = time_now;
        }
        else
        {
            timeval tv;
            gettimeofday(&tv, 0);
            m_time_in = 1000000*tv.tv_sec + tv.tv_usec;
        }
        prev_cmd = command;
        /* 1: Up, 2: Right, 3: Down, 4: Left, 5: table stim */
        std::stringstream cmd;
        switch(command)
        {
            case 0:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.0)");
                break;
            case 1:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](0.15,0.0,0.0)");
                break;
            case 2:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,-0.15)");
                break;
            case 3:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.15,0.0,0.0)");
                break;
            case 4:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.15)");
                break;
            case 5:
                return true;
                break;
        }
        return false;
    }
};

AdaptiveSteering::AdaptiveSteering(coshellbci::CoshellClient * coshell, Recognition * recognition_plugin)
: m_impl(new AdaptiveSteeringImpl(coshell, recognition_plugin))
{}

bool AdaptiveSteering::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

