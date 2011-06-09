#include <bci-interface/BCIInterface.h>
#include <bci-interface/BackgroundSprite/VisionServerBS.h>
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

std::string GenerateCommand(double fwd_speed, double turn_speed, int x, int y)
{
    std::stringstream ss;
    if(x > 2 || y > 2 || x < -2 || y < -2)
    {
        /* Out of screen stimulus */
        return GenerateCommand(fwd_speed, turn_speed, 0, 0);
    }
    if(y == 0)
    {
        ss << "set pg.velocitydes [3](0.0001,0.0," << x*turn_speed << ")";
    }
    else
    {
        ss << "set pg.velocitydes [3](" << y*fwd_speed << ",0.0," << x*turn_speed << ")";
    }
    return ss.str();
}

std::vector<std::string> GenerateCommands(int x, int y)
{
    std::vector<std::string> result;
    double fwd_speed = 0.1;
    double turn_speed = -0.1;
    result.push_back(GenerateCommand(fwd_speed, turn_speed, x ,y));
    result.push_back(GenerateCommand(fwd_speed, turn_speed, x, y+1));
    result.push_back(GenerateCommand(fwd_speed, turn_speed, x+1, y));
    result.push_back(GenerateCommand(fwd_speed, turn_speed, x, y-1));
    result.push_back(GenerateCommand(fwd_speed, turn_speed, x-1, y));
    return result;
}

void CreateObjects(BCIInterface * bciinterface, int cross_x, int cross_y, float width, float height)
{
    float orig_x = width/2 + cross_x*width/6;
    float orig_y = height/2 - cross_y*height/6;
    bool out_of_screen = false;
    if(cross_y > 1)
    {
        bciinterface->AddObject(new SSVEPStimulus(7, 60, width/2, height/2, 100, 100, 255, 0, 0, 255));
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(7,60, orig_x, orig_y - height/6, 100, 100, 255, 0, 0, 255)); }
    if(cross_x > 1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(12, 60, width/2, height/2, 100, 100, 255, 0, 0, 255)); }
        else { bciinterface->AddObject(new SSVEPStimulus(12, 60, width+200, height+200, 100, 100, 255, 0, 0, 255)); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(12,60, orig_x + width/6, orig_y, 100, 100, 255, 0, 0, 255)); }
    if(cross_y < -1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(5, 60, width/2, height/2, 100, 100, 255, 0, 0, 255)); }
        else { bciinterface->AddObject(new SSVEPStimulus(5, 60, width+200, height+200, 100, 100, 255, 0, 0, 255)); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(5,60, orig_x, orig_y + height/6, 100, 100, 255, 0, 0, 255)); }
    if(cross_x < -1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(9, 60, width/2, height/2, 100, 100, 255, 0, 0, 255)); }
        else { bciinterface->AddObject(new SSVEPStimulus(9, 60, width+200, height+200, 100, 100, 255, 0, 0, 255)); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(9,60, orig_x - width/6, orig_y, 100, 100, 255, 0, 0, 255)); }
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
        std::vector<std::string> commands = GenerateCommands(cross_x,cross_y);
        interpreter->SetCommands(commands);

        bciinterface->Clean();
        CreateObjects(bciinterface, cross_x, cross_y, (float)width, (float)height);

        app = bciinterface->DisplayLoop(app, fullscreen, out_cmd, timeout);

        switch(*out_cmd)
        {
            case 1:
                cross_x = cross_y==2?0:cross_x;
                cross_y = cross_y==2?0:cross_y+1;
                break;
            case 2:
                cross_y = cross_x==2?0:cross_y;
                cross_x = cross_x==2?0:cross_x+1;
                break;
            case 3:
                cross_x = cross_y==-2?0:cross_x;
                cross_y = cross_y==-2?0:cross_y-1;
                break;
            case 4:
                cross_y = cross_x==-2?0:cross_y;
                cross_x = cross_x==-2?0:cross_x-1;
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
