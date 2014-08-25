#include "BoxedTextObjectImpl.h"

using namespace bciinterface;

BoxedTextObjectImpl::BoxedTextObjectImpl(sf::Font & font, const std::string & txt)
: TextObjectImpl(font, txt), m_box(), box_margin(20)
{
  UpdateBox();
  SetBoxColor(sf::Color::Red);
  m_box.setOutlineColor(sf::Color::Green);
  m_box.setOutlineThickness(0);
}

BoxedTextObjectImpl::~BoxedTextObjectImpl()
{
}

void BoxedTextObjectImpl::Display(sf::RenderTarget * app, unsigned int fc, sf::Clock & clock)
{
  app->draw(m_box);
  TextObjectImpl::Display(app, fc, clock);
}

void BoxedTextObjectImpl::SetPosition(float x, float y)
{
  TextObjectImpl::SetPosition(x, y);
  UpdateBox();
}

void BoxedTextObjectImpl::SetText(const std::string & txt)
{
  TextObjectImpl::SetText(txt);
  UpdateBox();
}

void BoxedTextObjectImpl::SetCharacterSize(unsigned int size)
{
  TextObjectImpl::SetCharacterSize(size);
  UpdateBox();
}

void BoxedTextObjectImpl::SetColor(const sf::Color & color)
{
  TextObjectImpl::SetColor(color);
}

void BoxedTextObjectImpl::SetBoxColor(const sf::Color & color)
{
  m_box.setFillColor(color);
}

void BoxedTextObjectImpl::SetBoxMargin(const float & m)
{
  box_margin = m;
  UpdateBox();
}


void BoxedTextObjectImpl::UpdateBox()
{
  sf::FloatRect textB = m_text.getGlobalBounds();
  m_box.setPosition(textB.left - box_margin, textB.top - box_margin);
  m_box.setSize(sf::Vector2f(textB.width + 2*box_margin, textB.height + 2*box_margin));
}

