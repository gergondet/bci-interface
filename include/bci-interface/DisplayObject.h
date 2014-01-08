#ifndef _H_DISPLAY_OBJECT_H_
#define _H_DISPLAY_OBJECT_H_

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
    DisplayObject() {}; 

    virtual ~DisplayObject() {};

    /* Display function which needs to be implemented, can expect to receive frameCount from the interface */
    virtual void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock) = 0;

    /* Override to implement Display using OpenGL directly */
    virtual bool DrawWithGL() { return false; }

    /* Process an event */
    virtual void Process(sf::Event &, const BCIInterface &) {}

    /* Set the position of the object, arguments: X and Y */
    virtual void SetPosition(float, float) {}

    /* Set absolute rotation, in degree */
    virtual void SetRotation(float) {}

    /* Resize object, arguments: size_x and size_y */
    virtual void Resize(float, float) {}

    virtual void Highlight() {}

    virtual void Unhighlight() {}
};

} // namespace bciinterface

#endif // ifndef _H_DISPLAY_OBJECT_H_
