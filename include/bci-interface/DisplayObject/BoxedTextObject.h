#ifndef _H_BOXEDTEXTOBJECT_H_
#define _H_BOXEDTEXTOBJECT_H_

#include <bci-interface/DisplayObject.h>

#include <boost/shared_ptr.hpp>

namespace sf
{
    class Color;
    class Font;
}

namespace bciinterface
{

struct BoxedTextObjectImpl;

class BoxedTextObject : public DisplayObject
{
public:
    BCIINTERFACE_API BoxedTextObject(sf::Font & font, const std::string & txt);

    BCIINTERFACE_API virtual void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock);

    BCIINTERFACE_API void SetPosition(float x, float y);

    BCIINTERFACE_API void SetText(const std::string & txt);

    BCIINTERFACE_API void SetCharacterSize(unsigned int size);

    BCIINTERFACE_API void SetColor(const sf::Color & color);

    BCIINTERFACE_API void SetBoxColor(const sf::Color & color);

    BCIINTERFACE_API void SetBoxMargin(const float & m);

    BCIINTERFACE_API virtual void Highlight();

    BCIINTERFACE_API virtual void Unhighlight();
private:
    boost::shared_ptr<BoxedTextObjectImpl> m_impl;
};

}  // namespace bciinterface

#endif
