#ifndef _H_DISPLAY_OBJECT_H_
#define _H_DISPLAY_OBJECT_H_

namespace sf
{
    class Clock;

    class RenderWindow;

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
    virtual void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock) = 0;

    /* Override to implement Display using OpenGL directly */
    virtual bool DrawWithGL() { return false; }

    /* Process an event */
    virtual void Process(sf::Event & event, const BCIInterface & interface) {}

    virtual void SetPosition(float X, float Y) {}

    virtual void Resize(float size_x, float size_y) {}

    virtual void Highlight() {}

    virtual void Unhighlight() {}
};

} // namespace bciinterface

#endif // ifndef _H_DISPLAY_OBJECT_H_
