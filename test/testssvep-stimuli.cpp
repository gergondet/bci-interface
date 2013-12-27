#include <bci-interface/BCIInterface.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/DisplayObject/SpriteObject.h>
#include <bci-interface/DisplayObject/TextObject.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>

#ifdef WIN32
#include <Windows.h>
void sleep(DWORD t)
{
	Sleep(1000*t);
}
#endif

using namespace bciinterface;

class TestStimulus : public SSVEPStimulus
{
private:
    bool hl;
public:
    TestStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl)
        : SSVEPStimulus(frequency, screenFrequency, x, y, size_x, size_y, tx, tx_hl), hl(false)
    {}

    TestStimulus(int frequency, int screenFrequency, float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : SSVEPStimulus(frequency, screenFrequency, x, y, radius, r, g, b, a), hl(false)
    {} 

    TestStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : SSVEPStimulus(frequency, screenFrequency, x, y, size_x, size_y, r, g, b, a), hl(false)
    {} 

    void Process(sf::Event & event, const BCIInterface &)
    {
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
        {
            SetPosition(0,0);
        }
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
        {
            SetPosition(1280/2, 1024/2);
        }
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
        {
            hl = !hl;
            if(hl)
            {
                Highlight();
            }
            else
            {
                Unhighlight();
            }
        }
    }
};

int main(int, char * [])
{
    bool fullscreen = true;
    unsigned int width = 1280;
    unsigned int height = 1024;

    BCIInterface * bciinterface = new BCIInterface(width, height);

    /* Sprite stimulus */
    //bciinterface->AddObject(new TestStimulus(6,60, 50, 50, 100, 100, "TEST.png", "TEST_HL.png"));
    /* Circle stimulus */
    bciinterface->AddObject(new TestStimulus(6,60, 50, 50, 50, 255, 0, 0, 255));
    /* Rectangle stimulus */
    //bciinterface->AddObject(new TestStimulus(6, 60, 50, 50, 100, 100, 255, 0, 0, 255));

    //SpriteObject sprite("TEST.png");
    //sprite.SetPosition(100, 100);
    //sprite.Resize(200,200);
    //bciinterface->AddNonOwnedObject(&sprite);

    //TextObject text("Here's a text!");
    //bciinterface->AddNonOwnedObject(&text);

    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;

    return 0;
}
