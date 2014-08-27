#include <bci-interface/DisplayObject/BoxedTextObject.h>

#include "../private/DisplayObject/BoxedTextObjectImpl.h"

using namespace bciinterface;

BoxedTextObject::BoxedTextObject(sf::Font & font, const std::string & txt)
: m_impl(new BoxedTextObjectImpl(font, txt))
{
}

void BoxedTextObject::Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock)
{
  m_impl->Display(app, frameCount, clock);
}

void BoxedTextObject::SetPosition(float x, float y)
{
  m_impl->SetPosition(x, y);
}

void BoxedTextObject::SetText(const std::string & txt)
{
  m_impl->SetText(txt);
}

void BoxedTextObject::SetCharacterSize(unsigned int size)
{
  m_impl->SetCharacterSize(size);
}

void BoxedTextObject::SetColor(const sf::Color & color)
{
  m_impl->SetColor(color);
}

void BoxedTextObject::SetBoxColor(const sf::Color & color)
{
  m_impl->SetBoxColor(color);
}

void BoxedTextObject::SetBoxMargin(const float & m)
{
  m_impl->SetBoxMargin(m);
}

void BoxedTextObject::Highlight()
{
  m_impl->Highlight();
}

void BoxedTextObject::Unhighlight()
{
  m_impl->Unhighlight();
}
