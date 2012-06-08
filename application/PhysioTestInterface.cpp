#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <coshell-bci/CoshellClient.h>
#include <bci-middleware/TCPTrigger.h>
#include <bci-interface/CommandInterpreter/CoshellApplications/StaticSteering.h>
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
using namespace bcimw;


class CustomInterpreter : public StaticSteering
{
public:
    TCPTrigger * tcpTrigger;

	CustomInterpreter(coshellbci::CoshellClient * coshell) : StaticSteering(coshell), tcpTrigger(new TCPTrigger("150.29.145.137", 4242)) 
    {
    }

    ~CustomInterpreter()
    {
        delete tcpTrigger;
    }

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);
    virtual void Process(sf::Event & event, const BCIInterface & interface);
    
private:
    	bool m_stop;
    	int prev_cmd;
        
};

bool CustomInterpreter::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
	return StaticSteering::InterpretCommand(command, objects);
}

void CustomInterpreter::Process(sf::Event & sfevent, const BCIInterface & interface)
{
    std::cout << "CustomInterpreter::Process" << std::endl;
	if(sfevent.type == sf::Event::MouseButtonPressed && sfevent.mouseButton.button == sf::Mouse::Left)
	{
                tcpTrigger->TCPTrigger::SendTrigger();
	}
}

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
    coshellbci::CoshellClient * m_client = 0;
    
    
    if(moving_robot)
    {
        m_client = new coshellbci::CoshellClient("hrp2010c", 2809);
        m_client->Initialize();
    }
	CustomInterpreter * interpreter = new CustomInterpreter(m_client);
bciinterface->SetCommandInterpreter(interpreter);
    SSVEPTrainingManager * manager = new SSVEPTrainingManager(width, height, 1111, 2222, m_client);
    if(training_mode && moving_robot)
    {
        manager->SetTrainWithMovingRobot(true);
    }

    bciinterface->SetCommandReceiver(manager);

    //bciinterface->SetBackground(new VisionServerBG("hrp2010v", 4242, 640, 480, compress_data, width, height, width, height));
    
    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 100, 200,200, "data/UP.png", "data/UP_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-100, height/2, 200, 200, "data/RIGHT.png", "data/RIGHT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(10, 60, width/2, height-100, 200, 200, "data/DOWN.png", "data/DOWN_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, 100, height/2,200, 200, "data/LEFT.png", "data/LEFT_HL.png"));

    bciinterface->AddNonOwnedObject(&(manager->GetShape()));
    bciinterface->StartParadigm();
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete manager;
    delete m_client;
    delete interpreter;
    
    return 0;
}
