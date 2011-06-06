/* Generic class for receiving commands from a BCI system */

#ifndef _H_COMMANDRECEIVER_H_
#define _H_COMMANDRECEIVER_H_

namespace bciinterface
{

class CommandReceiver
{
public:
    CommandReceiver() {}

    virtual ~CommandReceiver() {}

    /* Loop to receive command */
    virtual void CommandLoop() = 0;

    /* Close command receiver */
    virtual void Close() = 0;

    /* Returns the current command output from the BCI */
    virtual int GetCommand() = 0;
};

} // namespace bciinterface

#endif

