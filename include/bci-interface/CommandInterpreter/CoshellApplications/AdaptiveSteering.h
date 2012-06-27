/* Rely on CoshellInterpreter to provide walking steering */

#ifndef _H_ADAPTIVESTEERING_H_
#define _H_ADAPTIVESTEERING_H_

#include <bci-interface/CommandInterpreter.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace coshellbci
{
    class CoshellClient;
}

namespace bciinterface
{

struct AdaptiveSteeringImpl;

class AdaptiveSteering : public CommandInterpreter 
{
public:
    AdaptiveSteering(coshellbci::CoshellClient * coshell);

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);

private:
    boost::shared_ptr<AdaptiveSteeringImpl> m_impl;
};

}

#endif

