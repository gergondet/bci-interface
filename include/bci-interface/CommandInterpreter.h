#ifndef _H_COMMANDINTERPRETER_H_
#define _H_COMMANDINTERPRETER_H_

#include <vector>

namespace bciinterface
{

class DisplayObject;

class CommandInterpreter
{
public:
    CommandInterpreter() {}

    virtual ~CommandInterpreter() {}

    /* Set input command */
    virtual void InterpretCommand(int command, const std::vector<DisplayObject*> & objects) = 0;
};

} // namespace bciinterface

#endif

