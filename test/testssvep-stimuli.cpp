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
public:
    TestStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a)
        : SSVEPStimulus(frequency, screenFrequency, x, y, size_x, size_y, r, g, b, a)
    {} 

    void Process(sf::Event & event)
    {
        if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Left)
        {
            SetPosition(0,0);
        }
        if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Right)
        {
            SetPosition(400, 400);
        }
    }
};

int main(int argc, char * argv[])
{
    bool fullscreen = false;
    unsigned int width = 1280;
    unsigned int height = 800;

    BCIInterface * bciinterface = new BCIInterface(width, height);
    UDPReceiver * receiver = new UDPReceiver(1111);
    SimpleInterpreter * interpreter = new SimpleInterpreter();
    bciinterface->SetCommandReceiver(receiver);
    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->SetBackground(new VisionServerBG("localhost", 4242, 640, 480));
    
    bciinterface->AddObject(new TestStimulus(7,60, width/2, 150, 150, 150, 255, 0, 0, 255));

    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
