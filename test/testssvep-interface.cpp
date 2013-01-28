#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/CommandOverrider.h>

#include <bci-interface/Background/BufferBG.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#ifdef WIN32
#include <Windows.h>
void sleep(DWORD t)
{
	Sleep(1000*t);
}
#include <stdint.h>
#else
#include <inttypes.h>
#endif

using namespace bciinterface;

int main(int argc, char * argv[])
{
    bool fullscreen = true;
    unsigned int width = 1280;
    unsigned int height = 1024; 
    if(!fullscreen)
    {
        width = 1024;
        height = 768;
    }

    BCIInterface * bciinterface = new BCIInterface(width, height);
    UDPReceiver * receiver = new UDPReceiver(1111);
    SimpleInterpreter * interpreter = new SimpleInterpreter();
    bciinterface->SetCommandReceiver(receiver);
    bciinterface->SetCommandInterpreter(interpreter);

    CommandOverrider overrider;
    overrider.AddOverrideCommand(sf::Keyboard::Up, 1);
    overrider.AddOverrideCommand(sf::Keyboard::Right, 2);
    overrider.AddOverrideCommand(sf::Keyboard::Down, 3);
    overrider.AddOverrideCommand(sf::Keyboard::Left, 4);
    bciinterface->SetCommandOverrider(&overrider);

    bool data_compressed = true;
    //bciinterface->SetBackground(new VisionServerBG("hrp2010v", 4242, 640, 480, data_compressed, width, height, 800, 600));
    BufferBG * bufferBG = new BufferBG(640, 480, width, height, 800, 600);
    uint32_t * buffer = new uint32_t[640*480];
    for(unsigned int x = 0; x < 640; ++x)
    {
        for(unsigned int y = 0; y < 480; ++y)
        {
            buffer[640*y + x] = 0xFF0000FF;
            if(x >= 320 && y < 240)
            {
                buffer[640*y + x] = 0xFF00FF00;
            }
            if(x < 320 && y < 240)
            {
                buffer[640*y + x] = 0xFFFF0000;
            }
            if(x < 320 && y >= 240)
            {
                buffer[640*y + x] = 0xFF00FFFF;
            }
        }
    }
    bufferBG->SetSubRect(0, 0, 320, 240);
    bufferBG->SetSubRect(0, 0, 640, 480);
    bufferBG->UpdateFromBuffer_RGB((unsigned char*)buffer);
    bciinterface->SetBackground(bufferBG);
    
    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 100, 200,200, "UP.png", "UP_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-100, height/2, 200, 200, "RIGHT.png", "RIGHT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(10, 60, width/2, height-100, 200, 200, "DOWN.png", "DOWN_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, 100, height/2,200, 200, "LEFT.png", "LEFT_HL.png"));
    
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
