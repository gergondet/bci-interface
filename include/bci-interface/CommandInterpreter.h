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
    BCIINTERFACE_API CommandInterpreter() {}

    BCIINTERFACE_API virtual ~CommandInterpreter() {}

    /* Set input command */
    BCIINTERFACE_API virtual bool InterpretCommand(int command, const std::vector<DisplayObject*> & objects) = 0;

    /* Handle events */
    BCIINTERFACE_API virtual void Process(sf::Event &, const BCIInterface &) {}
};

} // namespace bciinterface

#endif

