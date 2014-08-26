#include <bci-interface/BCIInterface.h>
#include <bci-interface/EventHandler.h>
#include <bci-interface/Utils/FontManager.h>
#include <bci-interface/DisplayObject/BoxedTextObject.h>

#include <stdlib.h>
#include <iostream>
#include <SFML/Window.hpp>

using namespace bciinterface;

class BoxMover : public EventHandler
{
public:
  BoxMover(BoxedTextObject & box)
  : box(box), moving(false), csize(30)
  {}

  void Process(sf::Event & event)
  {
    if(event.type == sf::Event::MouseButtonPressed)
    {
      moving = !moving;
    }
    if(event.type == sf::Event::MouseMoved && moving)
    {
      box.SetPosition(event.mouseMove.x, event.mouseMove.y);
    }
    if(event.type == sf::Event::KeyPressed)
    {
      if(event.key.code == sf::Keyboard::Up)
      {
        box.SetCharacterSize(++csize);
      }
      if(event.key.code == sf::Keyboard::Down)
      {
        box.SetCharacterSize(--csize);
      }
      if(event.key.code == sf::Keyboard::Space)
      {
        SetRandomText();
      }
    }
  }

  void SetRandomText()
  {
    unsigned int rN = 1 + rand() % 100;
    std::string foo(rN, 0);
    for(unsigned int i = 0; i < rN; ++i)
    {
      foo[i] = (char)(32 + rand() % 96);
      if(foo[i] == 32+95) { foo[i] = '\n'; }
    }
    box.SetText(foo);
  }
private:
  BoxedTextObject & box;
  bool moving;
  unsigned int csize;
};

int main(int argc, char * argv[])
{
  srand(time(0));
  bool fullscreen = false;
  unsigned int width = 1024;
  unsigned int height = 768;

  BCIInterface * bciinterface = new BCIInterface(width, height);
  FontManager fm;

  BoxedTextObject text(fm.GetDefaultFont(), "Hello \nworld!");
  text.SetPosition(width/2, height/2);
  text.SetCharacterSize(30);
  bciinterface->AddNonOwnedObject(&text);

  BoxMover mover(text);
  bciinterface->AddEventHandler(&mover);

  bciinterface->DisplayLoop(fullscreen);

  delete bciinterface;

  return 0;
}
