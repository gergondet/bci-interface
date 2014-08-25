#include "bci-interface/DisplayObject/TextObject.h"
#include "../private/DisplayObject/TextObjectImpl.h"

#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

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

void TextObject::SetColor(const sf::Color & color)
{
    m_impl->SetColor(color);
}

} // namespace bciinterface

