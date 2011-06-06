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
    BackgroundSprite() {}

    virtual ~BackgroundSprite {}

    virtual void UpdateLoop() = 0;

    virtual sf::Sprite * GetSprite() = 0;
};

} // namespace bciinterface

#endif

