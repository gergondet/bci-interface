/* Rely on CoshellInterpreter to provide head steering + object recognition */

#ifndef _H_OBJECTSLOOKOUT_H_
#define _H_OBJECTSLOOKOUT_H_

#include <bci-interface/CommandInterpreter.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace coshellbci
{
    class CoshellClient;
}

class Recognition;

namespace bciinterface
{

struct ObjectsLookoutImpl;

class ObjectsLookout : public CommandInterpreter 
{
public:
    ObjectsLookout(coshellbci::CoshellClient * coshell, Recognition * recognition_plugin, const std::vector<std::string> & objects);

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);

private:
    boost::shared_ptr<ObjectsLookoutImpl> m_impl;
};

}

#endif

