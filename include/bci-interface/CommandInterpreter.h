#ifndef _H_COMMANDINTERPRETER_H_
#define _H_COMMANDINTERPRETER_H_

namespace bciinterface
{

class CommandInterpreter
{
public:
    CommandInterpreter() {}

    virtual ~CommandInterpreter() {}

    /* Loop for commands interpretation */
    virtual void InterpreterLoop() = 0;

    /* Close the interpreter loop */
    virtual void Close() = 0;

    /* Set input command */
    virtual void SetInCommand(int command) = 0;
};

} // namespace bciinterface

#endif

