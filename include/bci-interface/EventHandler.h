#ifndef _H_EVENT_HANDLER_H_
#define _H_EVENT_HANDLER_H_

namespace sf
{
    class Event;
}

namespace bciinterface
{

class EventHandler
{
public:
    EventHandler() {}; 

    virtual ~EventHandler() {};

    /* Process an event */
    virtual void Process(sf::Event & event) = 0;
};

} // namespace bciinterface

#endif // ifndef _H_EVENT_HANDLER_H_
