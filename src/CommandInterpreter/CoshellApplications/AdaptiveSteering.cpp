#include <bci-interface/CommandInterpreter/CoshellApplications/AdaptiveSteering.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-client/CoshellClient.h>

#include <inttypes.h>
#include <sys/time.h>
#include <sstream>

namespace bciinterface
{

struct AdaptiveSteeringImpl : public SimpleInterpreter
{
private:
    coshell::CoshellClient * m_coshell;
    std::string m_pg;
    uint64_t m_time_in;
    int prev_cmd;
    int32_t depth_;
    int32_t depth_threshold_;
    bool command_ready;
public:
    AdaptiveSteeringImpl(coshell::CoshellClient * coshell, const std::string & start_script, const std::string & pg_entity)
        : m_coshell(coshell), m_pg(pg_entity), m_time_in(0), prev_cmd(-1), 
          depth_(0), depth_threshold_(950), command_ready(false)
    {
        m_coshell->ExecuteACommand(start_script);
        m_coshell->ExecuteACommand("set " + m_pg + ".velocitydes [3](-0.0001,0.0,0.0)");
    }

    void UpdateDepth(int32_t v)
    {
        depth_ = v;
    }

    void SetDepthTreshold(int32_t v)
    {
        depth_threshold_ = v;
    }

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        if(depth_ < depth_threshold_ && depth_ > 0 && command == 1)
        {
            command = 0;
            command_ready = true;
        }
        else if(prev_cmd == command)
        {
            uint64_t time_now;
            timeval tv;
            gettimeofday(&tv, 0);
            time_now = 1000000*tv.tv_sec + tv.tv_usec;
            if(command != 3)
            {
                if(time_now - m_time_in < 5e5 || command_ready) { return false; } 
                else { command_ready = true; }
            }
            if(command == 3)
            {
                if(time_now - m_time_in < 3e6) { return false; }
                else { return true; }
            }
        }
        else
        {
            timeval tv;
            gettimeofday(&tv, 0);
            m_time_in = 1000000*tv.tv_sec + tv.tv_usec;
            command_ready = false;
        }
        prev_cmd = command;
        SimpleInterpreter::InterpretCommand(command, objects);
        /* 1: Up, 2: Right, 3: Down, 4: Left, 5: table stim */
        if(command_ready)
        {
            std::stringstream cmd;
            switch(command)
            {
                case 0:
                    m_coshell->ExecuteACommand("set " + m_pg + ".velocitydes [3](0.0001,0.0,0.0)");
                    break;
                case 1:
                    m_coshell->ExecuteACommand("set " + m_pg + ".velocitydes [3](0.1,0.0,0.0)");
                    break;
                case 2:
                    m_coshell->ExecuteACommand("set " + m_pg + ".velocitydes [3](0.0001,0.0,-0.1)");
                    break;
                case 3:
//                    m_coshell->ExecuteACommand("set " + m_pg + ".velocitydes [3](-0.15,0.0,0.0)");
//                    return true;
                    break;
                case 4:
                    m_coshell->ExecuteACommand("set " + m_pg + ".velocitydes [3](0.0001,0.0,0.1)");
                    break;
                case 5:
                    break;
            }
        }
        return false;
    }
};

AdaptiveSteering::AdaptiveSteering(coshell::CoshellClient * coshell, const std::string & start_script, const std::string & pg_entity)
: m_impl(new AdaptiveSteeringImpl(coshell, start_script, pg_entity))
{}

bool AdaptiveSteering::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

void AdaptiveSteering::UpdateDepth(int32_t v)
{
    m_impl->UpdateDepth(v);
}

void AdaptiveSteering::SetDepthThreshold(int32_t v)
{
    m_impl->SetDepthTreshold(v);
}


} // namespace bciinterface

