#ifndef _H_EVENT_HANDLER_H_
#define _H_EVENT_HANDLER_H_

#include <bci-interface/BCIAPI.h>

namespace sf
{
    class Event;
}

namespace bciinterface
{

class EventHandler
{
public:
    BCIINTERFACE_API EventHandler() {}; 

    BCIINTERFACE_API virtual ~EventHandler() {};

    /* Process an event */
    BCIINTERFACE_API virtual void Process(sf::Event & event) = 0;
};

} // namespace bciinterface

#endif // ifndef _H_EVENT_HANDLER_H_
