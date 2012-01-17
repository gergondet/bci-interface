#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/CoshellInterpreter.h>

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
    bciinterface->SetCommandReceiver(receiver);

    CoshellInterpreter * interpreter = new CoshellInterpreter("hrp2010c", 2809);
    std::vector<std::string> commands;
    commands.push_back("set pg.velocitydes [3](-0.0001,0.0,0.0)");
    commands.push_back("set pg.velocitydes [3](0.15,0.0,0.0)");
    commands.push_back("set pg.velocitydes [3](-0.0001,0.0,-0.15)");
    commands.push_back("set pg.velocitydes [3](-0.15,0.0,0.0)");
    commands.push_back("set pg.velocitydes [3](-0.0001,0.0,0.15)");
    interpreter->SetCommands(commands);
    std::string initialtest = "pg.velocitydes";
    interpreter->SetInitialTest(initialtest);
    std::vector<std::string> initialcommands;
    initialcommands.push_back("import walking/startherdt");
    initialcommands.push_back("set pg.velocitydes [3](-0.0001,0.0,0.0)");
    interpreter->SetInitialCommands(initialcommands);
    std::vector<std::string> finalcommands;
    finalcommands.push_back("set pg.velocitydes [3](0.0,0.0,0.0)");
    interpreter->SetFinalCommands(finalcommands);
    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->SetBackground(new VisionServerBG("hrp2010v", 4242, 640, 480, width, height, 1024, 768));
    
    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 100, 200,200, "UP.png", "UP_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-100, height/2, 200, 200, "RIGHT.png", "RIGHT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(11, 60, width/2, height-100, 200, 200, "DOWN.png", "DOWN_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(13, 60, 100, height/2,200, 200, "LEFT.png", "LEFT_HL.png"));


    bciinterface->StartParadigm();
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
