#ifndef _H_DISPLAY_OBJECT_H_
#define _H_DISPLAY_OBJECT_H_

namespace sf
{
    class RenderWindow;

    class Event;
}

namespace bciinterface
{

class DisplayObject
{
public:
    DisplayObject() {}; 

    virtual ~DisplayObject() {};

    /* Display function which needs to be implemented, can expect to receive frameCount from the interface */
    virtual void Display(sf::RenderWindow * app, unsigned int frameCount = 0) = 0;

    /* Process an event */
    virtual void Process(sf::Event & event) {}

    virtual void Highlight() {}

    virtual void Unhighlight() {}
};

} // namespace bciinterface

#endif // ifndef _H_DISPLAY_OBJECT_H_
