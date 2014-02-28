#ifndef _H_SPRITEOBJECT_H_
#define _H_SPRITEOBJECT_H_

#include <bci-interface/DisplayObject.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct SpriteObjectImpl;

class SpriteObject : public DisplayObject
{
public:
    BCIINTERFACE_API SpriteObject(const std::string & image_src);

    BCIINTERFACE_API void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock);

    BCIINTERFACE_API void SetSubRect(int left, int top, int width, int height);

    BCIINTERFACE_API void SetPosition(float x, float y);

    BCIINTERFACE_API void Resize(float size_x, float size_y);
private:
    boost::shared_ptr<SpriteObjectImpl> m_impl;
};

}  // namespace bciinterface

#endif
