#include "bci-interface/CommandInterpreter/HoldInterpreter.h"

#include <stdint.h>
#include <sys/time.h>

namespace bciinterface
{

class HoldInterpreterImpl
{
private:
    int m_command;
    uint64_t m_time_reset;
    uint64_t m_time_hold;
public:
    HoldInterpreterImpl(unsigned int msec) :
    m_command(0), m_time_reset(0), m_time_hold(msec*1000)
    {        
    }

    bool InterpretCommand(int command, const std::vector<DisplayObject*> & objects)
    {
        if(command == 0)
        {
            m_command = 0;
            return false;
        }
        struct timeval tv_now;
        gettimeofday(&tv_now, 0);
        uint64_t time_now = tv_now.tv_sec*1e6 + tv_now.tv_usec;
        if(command == m_command)
        {
            if(time_now - m_time_reset > m_time_hold)
            {
                m_command = 0; // reset mechanism
                return true;
            }
        }
        else
        {
            m_command = command;
            m_time_reset = time_now;
        }
        return false;
    }
};

HoldInterpreter::HoldInterpreter(unsigned int msec) :
    m_impl(new HoldInterpreterImpl(msec))
{
}

bool HoldInterpreter::InterpretCommand(int command, const std::vector<DisplayObject*> & objects)
{
    SimpleInterpreter::InterpretCommand(command, objects);
    return m_impl->InterpretCommand(command, objects);
}   

} //namespace bciinterface

