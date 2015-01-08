#include <bci-interface/BCIInterface.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/Utils/FontManager.h>
#include <bci-interface/DisplayObject/FPSCounter.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/CommandOverrider.h>

#include <bci-interface/Background/BufferBG.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

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

std::string dirname(char * path)
{
    return ".";
}

#else
#include <inttypes.h>
#include <libgen.h>
#endif

using namespace bciinterface;

int main(int argc, char * argv[])
{
    BCIInterface * bciinterface = new BCIInterface();
    bciinterface->InitOculus();
    /* Render width/height */
    unsigned int rwidth = bciinterface->GetWidth();
    unsigned int rheight = bciinterface->GetHeight();

    FontManager fm;
    FPSCounter fps_c(fm.GetDefaultFont());

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

    BufferBG * bufferBG = new BufferBG(rwidth, rheight, rwidth, rheight);
    uint32_t * buffer = new uint32_t[rwidth*rheight];
    for(unsigned int x = 0; x < rwidth; ++x)
    {
        for(unsigned int y = 0; y < rheight; ++y)
        {
            buffer[rwidth*y + x] = 0xFF0000FF;
            if(x >= rwidth/2 && y < rheight/2)
            {
                buffer[rwidth*y + x] = 0xFF00FF00;
            }
            if(x < rwidth/2 && y < rheight/2)
            {
                buffer[rwidth*y + x] = 0xFFFF0000;
            }
            if(x < rwidth/2 && y >= rheight/2)
            {
                buffer[rwidth*y + x] = 0xFF00FFFF;
            }
        }
    }
    bufferBG->UpdateFromBuffer_RGB((unsigned char*)buffer);
    //bciinterface->SetBackground(bufferBG);

    std::string dir = dirname(argv[0]);
    dir += "/";
    bciinterface->AddObject(new SSVEPStimulus(6, 60, (float)rwidth/2, 100, 200,200, dir + "UP.png", dir + "UP_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, (float)rwidth-100, (float)rheight/2, 200, 200, dir + "RIGHT.png", dir + "RIGHT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(10, 60, (float)rwidth/2, (float)rheight-100, 200, 200, dir + "DOWN.png", dir + "DOWN_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, 100, (float)rheight/2,200, 200, dir + "LEFT.png", dir + "LEFT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(14, 60, (float)rwidth/2, (float)rheight/2, 200, 200, dir + "STOP.png", dir + "STOP_HL.png"));
    bciinterface->AddNonOwnedObject(&fps_c);

    int cmd = -1;
    bciinterface->OculusDisplayLoop(cmd);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
