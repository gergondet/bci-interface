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

    void Display(sf::RenderTarget * app, unsigned int, sf::Clock &);

    void SetPosition(float x, float y);

    void SetText(const std::string & txt);

    void SetCharacterSize(unsigned int size);

    void SetColor(const sf::Color & color);
};

} // namespace bciinterface


#endif
