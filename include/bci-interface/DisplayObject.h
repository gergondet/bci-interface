#ifndef _H_DISPLAY_OBJECT_H_
#define _H_DISPLAY_OBJECT_H_

#include <bci-interface/BCIAPI.h>

namespace sf
{
    class Clock;

    class RenderTarget;

    class Event;
}

namespace bciinterface
{

class BCIInterface;

class DisplayObject
{
public:
    BCIINTERFACE_API DisplayObject() {}; 

    BCIINTERFACE_API virtual ~DisplayObject() {};

    /* Display function which needs to be implemented, can expect to receive frameCount from the interface */
    BCIINTERFACE_API virtual void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock) = 0;

    /* Override to implement Display using OpenGL directly */
    BCIINTERFACE_API virtual bool DrawWithGL() { return false; }

    /* Process an event */
    BCIINTERFACE_API virtual void Process(sf::Event &, const BCIInterface &) {}

    /* Set the position of the object, arguments: X and Y */
    BCIINTERFACE_API virtual void SetPosition(float, float) {}

    /* Set absolute rotation, in degree */
    BCIINTERFACE_API virtual void SetRotation(float) {}

    /* Resize object, arguments: size_x and size_y */
    BCIINTERFACE_API virtual void Resize(float, float) {}

    BCIINTERFACE_API virtual void Highlight() {}

    BCIINTERFACE_API virtual void Unhighlight() {}
};

} // namespace bciinterface

#endif // ifndef _H_DISPLAY_OBJECT_H_
