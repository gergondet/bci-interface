#ifndef _H_BCIINTERFACE_H_
#define _H_BCIINTERFACE_H_

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics/Font.hpp>
#include <string>

namespace sf
{
    class RenderWindow;
}

class OculusWindow;

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
    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, bool fullscreen, int & cmd, float timeout = 0);

    /* DisplayLoop function for interface chaining alternate proto */
    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, int & cmd, float timeout = 0);

    /* Init things for the oculus display, this should be called if you wish to
     * access the renderscale before starting the display */
    void InitOculus();

    /* DisplayLoop for Oculus */
    void OculusDisplayLoop(int & cmd);

    sf::RenderWindow * GetRenderWindow();

    OculusWindow * GetOculusWindow();

    /* Enable FPS counter in Oculus window */
    void EnableFPSCounter(sf::Font & font);

    float GetRenderScale();

    /* Close the interface */
    void Close();

private:
    boost::shared_ptr<BCIInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif
