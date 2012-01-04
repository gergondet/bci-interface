#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/DisplayObject/SpriteObject.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/CoshellInterpreter.h>
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

inline std::string GenerateCommand(double fwd_speed, double turn_speed, int x, int y)
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

inline std::vector<std::string> GenerateCommands(int x, int y)
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

inline void CreateObjects(BCIInterface * bciinterface, int cross_x, int cross_y, float width, float height)
{
    bool out_of_screen = false;
    if(cross_y > 1)
    {
        bciinterface->AddObject(new SSVEPStimulus(7, 60, width-100, height-100, 150, 150, 255, 0, 0, 255));
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(7,60, width/2, 100, 150, 150, 255, 0, 0, 255)); }
    if(cross_x > 1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(12, 60, width-100, height-100, 150, 150, 255, 0, 0, 255)); }
        else { bciinterface->AddObject(new SSVEPStimulus(12, 60, width+200, height+200, 1, 1, 255, 0, 0, 255)); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(12,60, width-100, height/2, 150, 150, 255, 0, 0, 255)); }
    if(cross_y < -1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(5, 60, width-100, height-100, 150, 150, 255, 0, 0, 255)); }
        else { bciinterface->AddObject(new SSVEPStimulus(5, 60, width+200, height+200, 1, 1, 255, 0, 0, 255)); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(5,60, width/2, height-100, 150, 150, 255, 0, 0, 255)); }
    if(cross_x < -1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(9, 60, width-100, height-100, 150, 150, 255, 0, 0, 255)); }
        else { bciinterface->AddObject(new SSVEPStimulus(9, 60, width+200, height+200, 1, 1, 255, 0, 0, 255)); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(9,60, 100, height/2, 150, 150, 255, 0, 0, 255)); }
}

inline void GenerateHUD(BCIInterface * bciinterface, SpriteObject * m_hud_sprite, int cross_x, int cross_y)
{
    m_hud_sprite->SetSubRect((cross_x+2)*100+1, (-cross_y+2)*100+1, (cross_x+3)*100, (-cross_y+3)*100);
    bciinterface->AddNonOwnedObject(m_hud_sprite);
}

int main(int argc, char * argv[])
{
    bool fullscreen = false;
    unsigned int width = 1280;
    unsigned int height = 800;

    /* Get data path with a "good enough" version of basename */
    std::string app_path(argv[0]);
    size_t fslash = app_path.find_last_of('/');
    std::string data_path = app_path.substr(0, fslash)+"/data/";

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
    std::vector<std::string> finalcommands;
    finalcommands.push_back("set pg.velocitydes [3](0.0,0.0,0.0)");
    interpreter->SetFinalCommands(finalcommands);
    bciinterface->SetCommandInterpreter(interpreter);

    /* TODO DEBUG ONLY */
    SimpleInterpreter * sinterpreter = new SimpleInterpreter();
    bciinterface->SetCommandInterpreter(sinterpreter);

    bciinterface->SetBackground(new VisionServerBG("localhost", 4242, 640, 480, width, height));
    
    int * out_cmd = new int(-1);
    float timeout = 2;
    int cross_x = 0;
    int cross_y = 0;
    sf::RenderWindow * app = 0;

    SpriteObject * m_hud_sprite = new SpriteObject(data_path+"HUDSprite.png");
    m_hud_sprite->SetPosition(50, height-150);

    while(*out_cmd != 0)
    {
        std::vector<std::string> commands = GenerateCommands(cross_x,cross_y);
        interpreter->SetCommands(commands);

        bciinterface->Clean();
        CreateObjects(bciinterface, cross_x, cross_y, (float)width, (float)height);
        GenerateHUD(bciinterface, m_hud_sprite, cross_x, cross_y);

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
    delete m_hud_sprite;
    delete interpreter;
    delete sinterpreter;
    delete receiver;
    delete out_cmd;

    return 0;
}
