#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
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

    TestStimulus(int frequency, int screenFrequency, float x, float y, float radius, int r, int g, int b, int a)
        : SSVEPStimulus(frequency, screenFrequency, x, y, radius, r, g, b, a), hl(false)
    {} 

    TestStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a)
        : SSVEPStimulus(frequency, screenFrequency, x, y, size_x, size_y, r, g, b, a), hl(false)
    {} 

    void Process(sf::Event & event, const BCIInterface & intface)
    {
        if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Left)
        {
            SetPosition(0,0);
        }
        if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Right)
        {
            SetPosition(400, 400);
        }
        if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Space)
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

int main(int argc, char * argv[])
{
    bool fullscreen = false;
    unsigned int width = 1280;
    unsigned int height = 800;

    BCIInterface * bciinterface = new BCIInterface(width, height);

    bool data_compressed = true;
    bciinterface->SetBackground(new VisionServerBG("localhost", 4242, 640, 480, data_compressed, width, height));
    
    //bciinterface->AddObject(new TestStimulus(7,60, width/2, 150, 100, 100, "UP.png", "UP_HL.png"));
    bciinterface->AddObject(new TestStimulus(7,60, width/2, 150, 50, 255, 0, 0, 255));

    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;

    return 0;
}
