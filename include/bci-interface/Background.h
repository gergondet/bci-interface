/* Background class is a generic interface to feed the interface background */

#ifndef _H_BACKGROUND_H_
#define _H_BACKGROUND_H_

#include <bci-interface/BCIAPI.h>

namespace sf
{
    class RenderTarget;
}

namespace bciinterface
{

class Background
{
public:
    BCIINTERFACE_API Background() {}

    BCIINTERFACE_API virtual ~Background() {}

    /* Loop for background update will be launched in its own thread */
    BCIINTERFACE_API virtual void UpdateLoop() = 0;

    /* Close the update loop properly */
    BCIINTERFACE_API virtual void Close() = 0;

    /* Returns the current sprite of background */
    BCIINTERFACE_API virtual void Draw(sf::RenderTarget * app) = 0;

    /* "Zoom" on a sub-rectangle, optionnaly implemented */
    /* Arguments are (left, top, width, height) */
    BCIINTERFACE_API virtual void SetSubRect(int, int, int, int) {}

    /* Overrider if you wish to implement Draw with OpenGL */
    BCIINTERFACE_API virtual bool DrawWithGL() { return false; }
};

} // namespace bciinterface

#endif

