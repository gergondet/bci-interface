#include <bci-interface/DisplayObject/TextObject.h>

#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct TextObjectImpl
{
private:
    sf::Text m_text;
public:
    TextObjectImpl(const std::string & txt)
        : m_text(txt)
    {
        m_text.SetPosition(0,0);
    }

    ~TextObjectImpl()
    {
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
    {
        app->Draw(m_text);
    } 

    void SetPosition(float x, float y)
    {
        m_text.SetPosition(x, y);
    }

    void SetText(const std::string & txt)
    {
        m_text.SetString(txt);
    }

    void SetCharacterSize(unsigned int size)
    {
        m_text.SetCharacterSize(size);
    }
};

TextObject::TextObject(const std::string & txt) : m_impl(new TextObjectImpl(txt))
{}

void TextObject::Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
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

} // namespace bciinterface

