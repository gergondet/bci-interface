/* Simple intepreter: just highlight the selected object */

#ifndef _H_SIMPLEINTERPRETER_H_
#define _H_SIMPLEINTERPRETER_H_

#include <bci-interface/CommandInterpreter.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

class SimpleInterpreter : public CommandInterpreter
{
public:
    BCIINTERFACE_API virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);
};

} // namespace bciinterface

#endif

