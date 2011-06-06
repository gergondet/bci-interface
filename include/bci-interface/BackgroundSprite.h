/* BackgroundSprite class is a generic interface to feed the interface background */

#ifndef _H_BACKGROUNDSPRITE_H_
#define _H_BACKGROUNDSPRITE_H_

namespace sf
{
    class Sprite;
}

namespace bciinterface
{

class BackgroundSprite
{
public:
    BackgroundSprite(unsigned int width, unsigned int height) {}

    virtual ~BackgroundSprite() {}

    /* Loop for background update will be launched in its own thread */
    virtual void UpdateLoop() = 0;

    /* Close the update loop properly */
    virtual void Close() = 0;

    /* Returns the current sprite of background */
    virtual sf::Sprite * GetSprite() = 0;
};

} // namespace bciinterface

#endif

