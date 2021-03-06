/* Rely on CoshellInterpreter to provide walking steering */

#ifndef _H_ADAPTIVESTEERING_H_
#define _H_ADAPTIVESTEERING_H_

#include <bci-interface/CommandInterpreter.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace coshell
{
    class CoshellClient;
}

namespace bciinterface
{

struct AdaptiveSteeringImpl;

class AdaptiveSteering : public CommandInterpreter 
{
public:
    AdaptiveSteering(coshell::CoshellClient * coshell, const std::string & start_script = "import walking/startherdt", const std::string & pg_entity = "pg");

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);

    void UpdateDepth(int32_t new_d);

    void SetDepthThreshold(int32_t new_dt);

private:
    boost::shared_ptr<AdaptiveSteeringImpl> m_impl;
};

}

#endif

