/* Hold intepreter: just highlight the selected object */

#ifndef _H_HOLDINTERPRETER_H_
#define _H_HOLDINTERPRETER_H_

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

class HoldInterpreterImpl;

class HoldInterpreter : public SimpleInterpreter
{
public:
    /* Time to hold a command before triggering switch */
    HoldInterpreter(unsigned int msec, int ign_cmd = -1);

    HoldInterpreter(unsigned int msec, const std::vector<int> & ign_cmds);

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);

    void Reset();
private:
    boost::shared_ptr<HoldInterpreterImpl> m_impl;
};

} // namespace bciinterface

#endif

