#ifndef _H_BOXTEXTOBJECTIMPL_H_
#define _H_BOXTEXTOBJECTIMPL_H_

#include <iostream>
#include <SFML/Graphics.hpp>

#include "TextObjectImpl.h"

namespace bciinterface
{

struct BoxedTextObjectImpl : public TextObjectImpl
{
protected:
  sf::RectangleShape m_box;
  float box_margin;

  void UpdateBox();
public:
  BoxedTextObjectImpl(sf::Font & font, const std::string & txt);

  ~BoxedTextObjectImpl();

  void Display(sf::RenderTarget * app, unsigned int, sf::Clock &);

  void SetPosition(float x, float y);

  void SetText(const std::string & txt);

  void SetCharacterSize(unsigned int size);

  void SetColor(const sf::Color & color);

  void SetBoxColor(const sf::Color & color);

  void SetBoxMargin(const float & margin);
};

} // namespace bciinterface


#endif
