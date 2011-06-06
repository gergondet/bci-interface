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

struct BCIInterfaceImpl;

class BCIInterface
{
public:
    BCIInterface(unsigned int width, unsigned int height);

    /* Set BackgroundSprite */
    void SetBackgroundSprite(BackgroundSprite * background);

    /* Add a display object to the interface, the interface assumes ownership of the object */
    void AddObject(DisplayObject * object);

    /* Set the current command receiver */
    void SetCommandReceiver(CommandReceiver * receiver);

    /* Remove all objects from the interface */
    void Clean();

    /* Infinite DisplayLoop function */
    void DisplayLoop(bool fullscreen = true);

    /* DisplayLoop function for interface chaining */
    void DisplayLoop(sf::RenderWindow * app, unsigned int * cmd = 0, float timeout = 0);

    /* Close the interface */
    void Close();

private:
    boost::shared_ptr<BCIInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif
