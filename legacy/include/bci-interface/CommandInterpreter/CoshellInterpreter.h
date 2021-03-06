/* A command interpreter that relates BCI commands and coshell */

#ifndef _H_COSHELLINTERPRETER_H_
#define _H_COSHELLINTERPRETER_H_

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

struct CoshellInterpreterImpl;

class CoshellInterpreter : public CommandInterpreter 
{
public:
    CoshellInterpreter(const std::string & server_name, int server_port);

    CoshellInterpreter(coshell::CoshellClient * coshell);

    coshell::CoshellClient * GetCoshell();
    
    void Initialize();

    void Reset();

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);

    virtual void Process(sf::Event & event, const BCIInterface & interface);

    void SetCommands(const std::vector<std::string> & commands);

    void SetExitCommand(int command);

    void SetInitialTest(const std::string & test);

    void SetInitialCommands(const std::vector<std::string> & commands);

    void SetInitialCommands(const std::string & command);

    void SetFinalCommands(const std::vector<std::string> & commands);

    void SetFinalCommands(const std::string & command);

    void ExecuteFinalCommands();

    std::string SendCommand(const std::string & command);
private:
    boost::shared_ptr<CoshellInterpreterImpl> m_impl;
};

}

#endif

