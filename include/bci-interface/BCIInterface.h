#ifndef _H_BCIINTERFACE_H_
#define _H_BCIINTERFACE_H_

#include <boost/shared_ptr.hpp>

namespace sf
{
    class RenderWindow;
}

namespace bciinterface
{
class BackgroundSprite;
class DisplayObject;

class CommandReceiver;
class CommandInterpreter;

struct BCIInterfaceImpl;

class BCIInterface
{
public:
    BCIInterface(unsigned int width, unsigned int height);

    /* Set BackgroundSprite */
    void SetBackgroundSprite(BackgroundSprite * background);

    /* Add a display object to the interface, the interface assumes ownership of the object */
    void AddObject(DisplayObject * object);

    /* Set the current command receiver: BCIInterface does NOT assume ownership of the receiver */
    void SetCommandReceiver(CommandReceiver * receiver);

    /* Set the current command interpreter: BCIInterface does NOT assume ownership of the interpreter */
    void SetCommandInterpreter(CommandInterpreter * interpreter);

    /* Remove all objects from the interface */
    void Clean();

    /* Infinite DisplayLoop function */
    void DisplayLoop(bool fullscreen = true);

    /* DisplayLoop function for interface chaining */
    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, bool fullscreen, unsigned int * cmd, float timeout = 0);

    /* DisplayLoop function for interface chaining alternate proto */
    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, unsigned int * cmd, float timeout = 0);

    /* Close the interface */
    void Close();

private:
    boost::shared_ptr<BCIInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif
