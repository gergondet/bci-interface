#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <coshell-bci/CoshellClient.h>
#include <bci-interface/CommandInterpreter/CoshellApplications/StaticSteering.h>

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
    unsigned int height = 1024;
    bool compress_data = true;
    if(!fullscreen)
    {
        width = 1024;
        height = 768;
    }

    BCIInterface * bciinterface = new BCIInterface(width, height);

    UDPReceiver * receiver = new UDPReceiver(1111);
    bciinterface->SetCommandReceiver(receiver);

    coshellbci::CoshellClient * m_client = new coshellbci::CoshellClient("hrp2010c", 2809);
    m_client->Initialize();
    StaticSteering * interpreter = new StaticSteering(m_client);
    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->SetBackground(new VisionServerBG("hrp2010v", 4242, 640, 480, compress_data, width, height, 1024, 768));
    
    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 100, 200,200, "UP.png", "UP_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-100, height/2, 200, 200, "RIGHT.png", "RIGHT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(11, 60, width/2, height-100, 200, 200, "DOWN.png", "DOWN_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(13, 60, 100, height/2,200, 200, "LEFT.png", "LEFT_HL.png"));


    bciinterface->StartParadigm();
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete m_client;
    delete receiver;

    return 0;
}
