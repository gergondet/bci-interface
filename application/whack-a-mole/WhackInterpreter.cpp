#include "WhackInterpreter.h"

#include <bci-interface/DisplayObject.h>

#ifndef WIN32
#include <sys/time.h>
#else
#include <bci-interface/Utils/Win32.h>
#endif

bool WhackInterpreter::InterpretCommand(int command, const std::vector<bciinterface::DisplayObject *> & objects)
{
    SimpleInterpreter::InterpretCommand(command, objects);
    if(command != 0)
    {
        struct timeval tv_now;
        gettimeofday(&tv_now, 0);
        if(command != prev_cmd)
        {
            prev_cmd = command;
            hold_cmd_since = tv_now.tv_sec*1000000 + tv_now.tv_usec;
            return false; 
        }
        if(tv_now.tv_sec*1000000 + tv_now.tv_usec - hold_cmd_since > 2000000)
        {
            prev_cmd = 0; /* reset for next session */
            if(command == current_target)
            {
                objects[command-1]->Unhighlight();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
