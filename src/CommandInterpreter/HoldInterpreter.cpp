#include "bci-interface/CommandInterpreter/HoldInterpreter.h"

#include <stdint.h>

#ifndef WIN32
#include <sys/time.h>
#else
#include <bci-interface/Utils/Win32.h>
#endif

namespace bciinterface
{

class HoldInterpreterImpl
{
private:
    int m_command;
    std::vector<int> m_ign_cmds;
    uint64_t m_time_reset;
    uint64_t m_time_hold;
public:
    HoldInterpreterImpl(unsigned int msec, int ign_cmd) :
    m_command(0), m_ign_cmds(1, ign_cmd), m_time_reset(0), m_time_hold(msec*1000)
    {        
    }

    HoldInterpreterImpl(unsigned int msec, const std::vector<int> &  ign_cmds) :
    m_command(0), m_ign_cmds(ign_cmds), m_time_reset(0), m_time_hold(msec*1000)
    {        
    }

    bool InterpretCommand(int command, const std::vector<DisplayObject*> & objects)
    {
        if(command == 0)
        {
            m_command = 0;
            return false;
        }
        for(size_t i = 0; i < m_ign_cmds.size(); ++i)
        {
            if(command == m_ign_cmds[i])
            {
                m_command = 0;
                return false;
            }
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

    void Reset()
    {
        m_command = 0;
        struct timeval tv_now;
        gettimeofday(&tv_now, 0);
        uint64_t time_now = tv_now.tv_sec*1e6 + tv_now.tv_usec;
        m_time_reset = time_now;
    }
};

HoldInterpreter::HoldInterpreter(unsigned int msec, int ign_cmd) :
    m_impl(new HoldInterpreterImpl(msec, ign_cmd))
{
}

HoldInterpreter::HoldInterpreter(unsigned int msec, const std::vector<int> & ign_cmds) :
    m_impl(new HoldInterpreterImpl(msec, ign_cmds))
{
}

bool HoldInterpreter::InterpretCommand(int command, const std::vector<DisplayObject*> & objects)
{
    SimpleInterpreter::InterpretCommand(command, objects);
    return m_impl->InterpretCommand(command, objects);
}   

void HoldInterpreter::Reset()
{
    m_impl->Reset();
}

} //namespace bciinterface

