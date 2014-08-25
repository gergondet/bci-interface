#include "TextObjectImpl.h"

namespace bciinterface
{

TextObjectImpl::TextObjectImpl(sf::Font & font, const std::string & txt)
: m_text()
{
    m_text.setFont(font);
    m_text.setString(txt);
    m_text.setPosition(0,0);
}

TextObjectImpl::~TextObjectImpl()
{
}

void TextObjectImpl::Display(sf::RenderTarget * app, unsigned int, sf::Clock &)
{
    app->draw(m_text);
}

void TextObjectImpl::SetPosition(float x, float y)
{
    m_text.setPosition(x, y);
}

void TextObjectImpl::SetText(const std::string & txt)
{
    m_text.setString(txt);
}

void TextObjectImpl::SetCharacterSize(unsigned int size)
{
    m_text.setCharacterSize(size);
}

void TextObjectImpl::SetColor(const sf::Color & color)
{
    m_text.setColor(color);
}

} // namespace bciinterface

