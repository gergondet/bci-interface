/* Rely on CoshellInterpreter to provide walking steering 
   with a static paradigm */

#ifndef _H_STATICSTEERING_H_
#define _H_STATICSTEERING_H_

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace coshell
{
    class CoshellClient;
}

namespace bciinterface
{

class DisplayObject;

struct StaticSteeringImpl;

class StaticSteering : public SimpleInterpreter 
{
public:
    StaticSteering(coshell::CoshellClient * coshell);

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);

    void Process(sf::Event & event);
private:
    boost::shared_ptr<StaticSteeringImpl> m_impl;
};

}

#endif

