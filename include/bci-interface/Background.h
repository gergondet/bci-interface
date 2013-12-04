/* Background class is a generic interface to feed the interface background */

#ifndef _H_BACKGROUND_H_
#define _H_BACKGROUND_H_

namespace sf
{
    class RenderWindow;
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
    virtual void Draw(sf::RenderWindow * app) = 0;

    /* "Zoom" on a sub-rectangle, optionnaly implemented */
    virtual void SetSubRect(int left, int top, int width, int height) {}

    /* Overrider if you wish to implement Draw with OpenGL */
    virtual bool DrawWithGL() { return false; }
};

} // namespace bciinterface

#endif

