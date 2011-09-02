/* Rely on CoshellInterpreter to provide head steering + object recognition */

#ifndef _H_CUPBOARDLOOKOUT_H_
#define _H_CUPBOARDLOOKOUT_H_

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

struct CupboardLookoutImpl;

class CupboardLookout : public CommandInterpreter 
{
public:
    CupboardLookout(unsigned int widht, unsigned int height, coshellbci::CoshellClient * coshell, Recognition * recognition_plugin);

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);

private:
    boost::shared_ptr<CupboardLookoutImpl> m_impl;
};

}

#endif

