#include <bci-interface/SimpleInterpreter.h>

#include <bci-interface/DisplayObject.h>

namespace bciinterface
{

void SimpleInterpreter::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    for(size_t i = 0; i < objects.size(); ++i)
    {
        objects[i]->Unhighlight();
    }
    if(command - 1 < (int)objects.size() && command - 1 >= 0)
    {
        objects[command-1]->Highlight();
    }
}

} // namespace bciinterface

