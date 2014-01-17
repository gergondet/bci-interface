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
    TextObject(sf::Font & font, const std::string & txt);

    virtual void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock);

    void SetPosition(float x, float y);

    void SetText(const std::string & txt);

    void SetCharacterSize(unsigned int size);

    void SetColor(const sf::Color & color);
private:
    boost::shared_ptr<TextObjectImpl> m_impl;
};

}  // namespace bciinterface

#endif
