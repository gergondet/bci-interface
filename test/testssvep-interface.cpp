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
    
    bciinterface->AddObject(new SSVEPStimulus(7,60, width/2, 150, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(12,60, width-150, height/2, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(5,60, width/2, height-150, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(9,60, 150, height/2, 150, 150, 255, 0, 0, 255));

    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
