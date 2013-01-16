#include <bci-interface/CommandInterpreter/CoshellApplications/AdaptiveSteering.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-client/CoshellClient.h>

#include <sys/time.h>
#include <sstream>

namespace bciinterface
{

struct AdaptiveSteeringImpl : public SimpleInterpreter
{
private:
    coshell::CoshellClient * m_coshell;
    double m_time_in;
    int prev_cmd;
    int32_t depth_;
    int32_t depth_threshold_;
public:
    AdaptiveSteeringImpl(coshell::CoshellClient * coshell)
        : m_coshell(coshell), m_time_in(0), prev_cmd(-1), 
          depth_(0), depth_threshold_(950)
    {
//        m_coshell->ExecuteACommand("StartWalking");
        m_coshell->ExecuteACommand("import walking/startherdt");
        m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.0)");
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
        }
        if(prev_cmd == command && command != 3) 
        { 
            return false; 
        }
        else if(prev_cmd == command && command == 3)
        {
            double time_now;
            timeval tv;
            gettimeofday(&tv, 0);
            time_now = 1000000*tv.tv_sec + tv.tv_usec;
            if(time_now < m_time_in + 3000000) { return false; }
            return true;
        }
        else
        {
            timeval tv;
            gettimeofday(&tv, 0);
            m_time_in = 1000000*tv.tv_sec + tv.tv_usec;
        }
        prev_cmd = command;
        SimpleInterpreter::InterpretCommand(command, objects);
        /* 1: Up, 2: Right, 3: Down, 4: Left, 5: table stim */
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
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,-0.1)");
                break;
            case 3:
//                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.15,0.0,0.0)");
//                return true;
                break;
            case 4:
                m_coshell->ExecuteACommand("set pg.velocitydes [3](-0.0001,0.0,0.1)");
                break;
            case 5:
                return true;
                break;
        }
        return false;
    }
};

AdaptiveSteering::AdaptiveSteering(coshell::CoshellClient * coshell)
: m_impl(new AdaptiveSteeringImpl(coshell))
{}

bool AdaptiveSteering::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

