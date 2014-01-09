#include "bci-interface/DisplayObject/TextObject.h"

#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct TextObjectImpl
{
private:
    sf::Text m_text;
public:
    TextObjectImpl(sf::Font & font, const std::string & txt)
        : m_text()
    {
        m_text.setFont(font);
        m_text.setString(txt);
        m_text.setPosition(0,0);
    }

    ~TextObjectImpl()
    {
    }

    void Display(sf::RenderTarget * app, unsigned int, sf::Clock &)
    {
        app->draw(m_text);
    } 

    void SetPosition(float x, float y)
    {
        m_text.setPosition(x, y);
    }

    void SetText(const std::string & txt)
    {
        m_text.setString(txt);
    }

    void SetCharacterSize(unsigned int size)
    {
        m_text.setCharacterSize(size);
    }

    void SetColor(sf::Color & color)
    {
        m_text.setColor(color);
    }
};

TextObject::TextObject(sf::Font & font, const std::string & txt) : m_impl(new TextObjectImpl(font, txt))
{}

void TextObject::Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock)
{
    m_impl->Display(app, frameCount, clock);
}

void TextObject::SetPosition(float x, float y)
{
    m_impl->SetPosition(x, y);
}

void TextObject::SetText(const std::string & txt)
{
    m_impl->SetText(txt);
}

void TextObject::SetCharacterSize(unsigned int size)
{
    m_impl->SetCharacterSize(size);
}

void TextObject::SetColor(sf::Color & color)
{
    m_impl->SetColor(color);
}

} // namespace bciinterface

