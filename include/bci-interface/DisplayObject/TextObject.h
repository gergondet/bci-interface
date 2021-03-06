#ifndef _H_TEXTOBJECT_H_
#define _H_TEXTOBJECT_H_

#include <bci-interface/DisplayObject.h>

#include <boost/shared_ptr.hpp>

namespace sf
{
    class Color;
    class Font;
}

namespace bciinterface
{

struct TextObjectImpl;

class TextObject : public DisplayObject
{
public:
    BCIINTERFACE_API TextObject(sf::Font & font, const std::string & txt);

    BCIINTERFACE_API virtual void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock);

    BCIINTERFACE_API void SetPosition(float x, float y);

    BCIINTERFACE_API void SetText(const std::string & txt);

    BCIINTERFACE_API void SetCharacterSize(unsigned int size);

    BCIINTERFACE_API void SetColor(const sf::Color & color);
protected:
    boost::shared_ptr<TextObjectImpl> m_impl;
};

}  // namespace bciinterface

#endif
