#ifndef _H_BCIINTERFACE_H_
#define _H_BCIINTERFACE_H_

#include <boost/shared_ptr.hpp>

namespace sf
{
    class RenderWindow;
}

namespace bciinterface
{
class EventHandler;
class Background;
class DisplayObject;

class CommandReceiver;
class CommandOverrider;
class CommandInterpreter;

struct BCIInterfaceImpl;

class BCIInterface
{
public:
    BCIInterface(unsigned int width, unsigned int height);

    bool ParadigmStatus();
    
    void StartParadigm();

    void StopParadigm();

    /* Add an event handler, the interface assumes ownership of the object */
    void AddEventHandler(EventHandler * handler);

    /* Set Background */
    void SetBackground(Background * background);

    /* Get Background, can be zero */
    Background * GetBackground() const;

    /* Add a display object to the interface, the interface assumes ownership of the object */
    void AddObject(DisplayObject * object);

    /* Add a display object not owned by the interface*/
    void AddNonOwnedObject(DisplayObject * object);

    /* Set the current command receiver: BCIInterface does NOT assume ownership of the receiver */
    void SetCommandReceiver(CommandReceiver * receiver);

    /* Set the current command overrider: BCIInterface does NOT assume ownership of the overrider */
    void SetCommandOverrider(CommandOverrider * overrider);

    /* Set the current command interpreter: BCIInterface does NOT assume ownership of the interpreter */
    void SetCommandInterpreter(CommandInterpreter * interpreter);

    /* Get the current command interpreter */
    CommandInterpreter * GetCommandInterpreter();

    /* Remove all objects from the interface */
    void Clean();

    /* Infinite DisplayLoop function */
    void DisplayLoop(bool fullscreen = true);

    /* DisplayLoop function for interface chaining */
    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, bool fullscreen, int * cmd, float timeout = 0);

    /* DisplayLoop function for interface chaining alternate proto */
    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, int * cmd, float timeout = 0);

    sf::RenderWindow * GetRenderWindow();

    /* Close the interface */
    void Close();

private:
    boost::shared_ptr<BCIInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif
