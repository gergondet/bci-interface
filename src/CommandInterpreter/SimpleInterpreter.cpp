#include "bci-interface/CommandInterpreter/SimpleInterpreter.h"

#include "bci-interface/DisplayObject.h"

namespace bciinterface
{

bool SimpleInterpreter::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    for(size_t i = 0; i < objects.size(); ++i)
    {
        objects[i]->Unhighlight();
    }
    if(command - 1 < (int)objects.size() && command - 1 >= 0)
    {
        objects[command-1]->Highlight();
    }
    return false;
}

} // namespace bciinterface

