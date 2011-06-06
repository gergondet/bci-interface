#include <bci-interface/BCIInterface.h>
#include <bci-interface/VisionServerBS.h>
#include <bci-interface/SSVEPStimulus.h>

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

    bciinterface->SetBackgroundSprite(new VisionServerBS("localhost", 4242, 640, 480));
    
    bciinterface->AddObject(new SSVEPStimulus(7,60, width/2-75, 50, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(12,60, width-225, height/2-75, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(5,60, width/2-75, height-200, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(9,60, 75, height/2-75, 150, 150, 255, 0, 0, 255));

    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;

    return 0;
}
