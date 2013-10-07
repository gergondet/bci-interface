#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <coshell-client/CoshellClient.h>

#include "SSVEPTrainingManager.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <getopt.h>

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
    bool training_mode = false;
    bool moving_robot = false;

    struct option options[3] = {
        {"with-training", 0, 0, 1},
        {"with-robot", 0, 0, 2},
        {0, 0, 0, 0}
    };
   
    int opt;
    while( (opt = getopt_long(argc, argv, "", options, 0) ) != -1 )
    {
        switch(opt)
        {
            case 1:
                training_mode = true;
                break;
            case 2:
                moving_robot = true;
                break;
            default:
                break;
        }
    }

    bool fullscreen = true;
    unsigned int width = 1280;
    unsigned int height = 1024;
    bool compress_data = true;
    if(!fullscreen)
    {
        width = 1024;
        height = 768;
    }

    BCIInterface * bciinterface = new BCIInterface(width, height);


    coshell::CoshellClient * m_client = 0;
    if(moving_robot)
    {
        m_client = new coshell::CoshellClient("hrp2010c", 2809);
        m_client->Initialize();
    }

    SSVEPTrainingManager * manager = new SSVEPTrainingManager(width, height, 1111, 2222, m_client);
    if(training_mode && moving_robot)
    {
        manager->SetTrainWithMovingRobot(true);
    }

    bciinterface->SetCommandReceiver(manager);
    bciinterface->SetCommandInterpreter(manager);

    bciinterface->SetBackground(new VisionServerBG("hrp2010v", 4242, 640, 480, compress_data, width, height, width, height));
    
    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 100, 200,200, "UP.png", "UP_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-100, height/2, 200, 200, "RIGHT.png", "RIGHT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(10, 60, width/2, height-100, 200, 200, "DOWN.png", "DOWN_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, 100, height/2,200, 200, "LEFT.png", "LEFT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(14, 60, width/2, height/2, 200, 200, "STOP.png", "STOP_HL.png"));

    bciinterface->AddNonOwnedObject(&(manager->GetShape()));

    bciinterface->StartParadigm();
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete manager;
    delete m_client;

    return 0;
}
