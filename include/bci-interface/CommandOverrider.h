#ifndef _H_COMMANDOVERRIDER_H_
#define _H_COMMANDOVERRIDER_H_

#include <bci-interface/BCIInterface.h>

#include <SFML/Window/Event.hpp>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct CommandOverriderImpl;

class CommandOverrider
{
public:
    BCIINTERFACE_API CommandOverrider();

    BCIINTERFACE_API virtual void Process(sf::Event & event, const BCIInterface & iface);

    /* Handle keyboards events */
    BCIINTERFACE_API void AddOverrideCommand(sf::Keyboard::Key ekey, int command);

    /* Handle mouse click events */
    BCIINTERFACE_API void AddOverrideCommand(sf::Mouse::Button ebutton, int command);

    /* Handle joypad button events */
    BCIINTERFACE_API void AddOverrideCommand(unsigned int button, int command);

    /* Remove all handled events */
    BCIINTERFACE_API void Clean();

    BCIINTERFACE_API virtual int GetCommand();

    BCIINTERFACE_API virtual bool IsOverriding(); 

private:
    boost::shared_ptr<CommandOverriderImpl> m_impl;
};

} // namespace bciinterface

#endif
