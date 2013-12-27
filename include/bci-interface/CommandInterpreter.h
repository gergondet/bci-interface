#ifndef _H_COMMANDINTERPRETER_H_
#define _H_COMMANDINTERPRETER_H_

#include <vector>

#include <bci-interface/BCIInterface.h>

namespace sf
{
    class Event;
}

namespace bciinterface
{

class DisplayObject;

class CommandInterpreter
{
public:
    CommandInterpreter() {}

    virtual ~CommandInterpreter() {}

    /* Set input command */
    virtual bool InterpretCommand(int command, const std::vector<DisplayObject*> & objects) = 0;

    /* Handle events */
    virtual void Process(sf::Event &, const BCIInterface &) {}
};

} // namespace bciinterface

#endif

