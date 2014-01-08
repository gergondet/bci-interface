/* Background class is a generic interface to feed the interface background */

#ifndef _H_BACKGROUND_H_
#define _H_BACKGROUND_H_

namespace sf
{
    class RenderTarget;
}

namespace bciinterface
{

class Background
{
public:
    Background() {}

    virtual ~Background() {}

    /* Loop for background update will be launched in its own thread */
    virtual void UpdateLoop() = 0;

    /* Close the update loop properly */
    virtual void Close() = 0;

    /* Returns the current sprite of background */
    virtual void Draw(sf::RenderTarget * app) = 0;

    /* "Zoom" on a sub-rectangle, optionnaly implemented */
    /* Arguments are (left, top, width, height) */
    virtual void SetSubRect(int, int, int, int) {}

    /* Overrider if you wish to implement Draw with OpenGL */
    virtual bool DrawWithGL() { return false; }
};

} // namespace bciinterface

#endif

