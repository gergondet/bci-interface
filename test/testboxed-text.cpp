#include <bci-interface/BCIInterface.h>
#include <bci-interface/EventHandler.h>
#include <bci-interface/Utils/FontManager.h>
#include <bci-interface/DisplayObject/BoxedTextObject.h>

#include <iostream>
#include <SFML/Window.hpp>

using namespace bciinterface;

class BoxMover : public EventHandler
{
public:
  BoxMover(BoxedTextObject & box)
  : box(box), moving(false)
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
  }
private:
  BoxedTextObject & box;
  bool moving;
};

int main(int argc, char * argv[])
{
    bool fullscreen = false;
    unsigned int width = 1024;
    unsigned int height = 768;

    BCIInterface * bciinterface = new BCIInterface(width, height);
    FontManager fm;

    BoxedTextObject text(fm.GetDefaultFont(), "Hello world");
    text.SetPosition(width/2, height/2);
    bciinterface->AddNonOwnedObject(&text);

    BoxMover mover(text);
    bciinterface->AddEventHandler(&mover);

    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;

    return 0;
}
