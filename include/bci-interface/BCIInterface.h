#ifndef _H_BCIINTERFACE_H_
#define _H_BCIINTERFACE_H_

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics/Font.hpp>
#include <string>

#include <bci-interface/BCIAPI.h>

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
    BCIINTERFACE_API BCIInterface(unsigned int width, unsigned int height);

    BCIINTERFACE_API bool ParadigmStatus();
    
    BCIINTERFACE_API void StartParadigm();

    BCIINTERFACE_API void StopParadigm();

    /* Add an event handler, the interface assumes ownership of the object */
    BCIINTERFACE_API void AddEventHandler(EventHandler * handler);

    /* Set Background */
    BCIINTERFACE_API void SetBackground(Background * background);

    /* Get Background, can be zero */
    BCIINTERFACE_API Background * GetBackground() const;

    /* Add a display object to the interface, the interface assumes ownership of the object */
    BCIINTERFACE_API void AddObject(DisplayObject * object);

    /* Add a display object not owned by the interface*/
    BCIINTERFACE_API void AddNonOwnedObject(DisplayObject * object);

    /* Set the current command receiver: BCIInterface does NOT assume ownership of the receiver */
    BCIINTERFACE_API void SetCommandReceiver(CommandReceiver * receiver);

    /* Set the current command overrider: BCIInterface does NOT assume ownership of the overrider */
    BCIINTERFACE_API void SetCommandOverrider(CommandOverrider * overrider);

    /* Set the current command interpreter: BCIInterface does NOT assume ownership of the interpreter */
    BCIINTERFACE_API void SetCommandInterpreter(CommandInterpreter * interpreter);

    /* Get the current command interpreter */
    BCIINTERFACE_API CommandInterpreter * GetCommandInterpreter();

    /* Remove all objects from the interface */
    BCIINTERFACE_API void Clean();

    /* Infinite DisplayLoop function */
    BCIINTERFACE_API void DisplayLoop(bool fullscreen = true);

    /* DisplayLoop function for interface chaining */
    BCIINTERFACE_API sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, bool fullscreen, int & cmd, float timeout = 0);

    /* DisplayLoop function for interface chaining alternate proto */
    BCIINTERFACE_API sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, int & cmd, float timeout = 0);

    /* Init things for the oculus display, this should be called if you wish to
     * access the renderscale before starting the display */
    BCIINTERFACE_API void InitOculus();

    /* DisplayLoop for Oculus */
    BCIINTERFACE_API void OculusDisplayLoop(int & cmd);

    BCIINTERFACE_API sf::RenderWindow * GetRenderWindow();

    BCIINTERFACE_API OculusWindow * GetOculusWindow();

    /* Enable FPS counter in Oculus window */
    BCIINTERFACE_API void EnableFPSCounter(sf::Font & font);

    BCIINTERFACE_API float GetRenderScale();

    /* Close the interface */
    BCIINTERFACE_API void Close();

private:
    boost::shared_ptr<BCIInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif
