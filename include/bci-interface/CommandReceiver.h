/* Generic class for receiving commands from a BCI system */

#ifndef _H_COMMANDRECEIVER_H_
#define _H_COMMANDRECEIVER_H_

#include <bci-interface/BCIAPI.h>

namespace bciinterface
{

class CommandReceiver
{
public:
    BCIINTERFACE_API CommandReceiver() {}

    BCIINTERFACE_API virtual ~CommandReceiver() {}

    /* Loop to receive command */
    BCIINTERFACE_API virtual void CommandLoop() = 0;

    /* Close command receiver */
    BCIINTERFACE_API virtual void Close() = 0;

    /* Returns the current command output from the BCI */
    BCIINTERFACE_API virtual int GetCommand() = 0;
};

} // namespace bciinterface

#endif

