#include <bci-interface/BCIInterface.h>
#include <bci-interface/VisionServerBS.h>
#include <bci-interface/SSVEPStimulus.h>
#include <bci-interface/UDPReceiver.h>
#include <bci-interface/CoshellInterpreter.h>

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

std::string GenerateCommand(double fwd_speed, double turn_speed)
{
    std::stringstream ss;
    if(fwd_speed == 0)
    {
        ss << "set pg.velocitydes [3](0.0001,0.0," << turn_speed << ")";
    }
    else
    {
        ss << "set pg.velocitydes [3](" << fwd_speed << ",0.0," << turn_speed << ")";
    }
    return ss.str();
}

std::vector<std::string> GenerateCommands(int x, int y)
{
    std::vector<std::string> result;
    double fwd_speed = 0.1;
    double turn_speed = -0.1;
    result.push_back(GenerateCommand(y*fwd_speed, x*turn_speed));
    result.push_back(GenerateCommand((y+1)*fwd_speed, x*turn_speed));
    result.push_back(GenerateCommand(y*fwd_speed, (x+1)*turn_speed));
    result.push_back(GenerateCommand((y-1)*fwd_speed, x*turn_speed));
    result.push_back(GenerateCommand(y*fwd_speed, (x-1)*turn_speed));
    return result;
}

void CreateObjects(BCIInterface * bciinterface, int cross_x, int cross_y)
{
    int width = 1280;
    int height = 800;
    bciinterface->AddObject(new SSVEPStimulus(7,60, width/2-75, 50, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(12,60, width-225, height/2-75, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(5,60, width/2-75, height-200, 150, 150, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(9,60, 75, height/2-75, 150, 150, 255, 0, 0, 255));
}

int main(int argc, char * argv[])
{
    bool fullscreen = false;
    unsigned int width = 1280;
    unsigned int height = 800;

    BCIInterface * bciinterface = new BCIInterface(width, height);

    UDPReceiver * receiver = new UDPReceiver(1111);
    bciinterface->SetCommandReceiver(receiver);

    CoshellInterpreter * interpreter = new CoshellInterpreter("localhost", 2809);
    std::string initialtest = "pg.velocitydes";
    interpreter->SetInitialTest(initialtest);
    std::vector<std::string> initialcommands;
    initialcommands.push_back("import walking/startherdt");
    initialcommands.push_back("set pg.velocitydes [3](0.0001,0.0,0.0)");
    interpreter->SetInitialCommands(initialcommands);
    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->SetBackgroundSprite(new VisionServerBS("localhost", 4242, 640, 480));
    
    int * out_cmd = new int(-1);
    float timeout = 2;
    int cross_x = 0;
    int cross_y = 0;
    sf::RenderWindow * app = 0;

    while(*out_cmd != 0)
    {
        *out_cmd = 0;

        std::vector<std::string> commands = GenerateCommands(cross_x,cross_y);
        interpreter->SetCommands(commands);

        bciinterface->Clean();
        CreateObjects(bciinterface, cross_x, cross_y);

        app = bciinterface->DisplayLoop(app, fullscreen, out_cmd, timeout);

        switch(*out_cmd)
        {
            case 1:
                cross_y += 1;
                break;
            case 2:
                cross_x += 1;
                break;
            case 3:
                cross_y -= 1;
                break;
            case 4:
                cross_x -= 1;
                break;
            default:
                break;
        }
    }

    if(app)
    {
        app->Close();
    }

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
