#ifndef _H_TEXTOBJECTIMPL_H_
#define _H_TEXTOBJECTIMPL_H_

#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct TextObjectImpl
{
protected:
    sf::Text m_text;
public:
    TextObjectImpl(sf::Font & font, const std::string & txt);

    ~TextObjectImpl();

    virtual void Display(sf::RenderTarget * app, unsigned int, sf::Clock &);

    virtual void SetPosition(float x, float y);

    virtual void SetText(const std::string & txt);

    virtual void SetCharacterSize(unsigned int size);

    virtual void SetColor(const sf::Color & color);
};

} // namespace bciinterface


#endif
