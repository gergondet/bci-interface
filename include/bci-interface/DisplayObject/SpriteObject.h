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
    SpriteObject(const std::string & image_src);

    void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock);

    void SetSubRect(int left, int top, int width, int height);

    void SetPosition(float x, float y);

    void Resize(float size_x, float size_y);
private:
    boost::shared_ptr<SpriteObjectImpl> m_impl;
};

}  // namespace bciinterface

#endif
