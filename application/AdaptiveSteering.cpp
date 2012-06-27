#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/CoshellInterpreter.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>

#include <boost/thread.hpp>

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

template<typename T>
inline int sgn(T a)
{
    return a > 0 ? 1 : ( a == 0 ? 0 : -1 );
}

inline void CreateObjects(BCIInterface * bciinterface, int cross_x, int cross_y, float width, float height)
{
    float orig_x = width/2 + cross_x*width/6 + sgn(cross_x)*50;
    float orig_y = height/2 - cross_y*height/6 - sgn(cross_y) * 50;
    bool out_of_screen = false;
    if(cross_y > 1)
    {
        bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, height/2, 150, 150, "STOP.png", "STOP_HL.png"));
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(6,60, orig_x, orig_y - height/6, 200, 200, "UP.png", "UP_HL.png")); }
    if(cross_x > 1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(8, 60, width/2, height/2, 150, 150, "STOP.png", "STOP_HL.png")); }
        else { bciinterface->AddObject(new SSVEPStimulus(8, 60, width+200, height+200, 200, 200, "RIGHT.png", "RIGHT_HL.png")); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(8,60, orig_x + width/6, orig_y, 200, 200, "RIGHT.png", "RIGHT_HL.png")); }
    if(cross_y < -1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(11, 60, width/2, height/2, 150, 150, "STOP.png", "STOP_HL.png")); }
        else { bciinterface->AddObject(new SSVEPStimulus(11, 60, width+200, height+200, 200, 200, "DOWN.png", "DOWN_HL.png")); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(11,60, orig_x, orig_y + height/6, 200, 200, "DOWN.png", "DOWN_HL.png")); }
    if(cross_x < -1)
    {
        if(!out_of_screen) { bciinterface->AddObject(new SSVEPStimulus(13, 60, width/2, height/2, 150, 150, "STOP.png", "STOP_HL.png")); }
        else { bciinterface->AddObject(new SSVEPStimulus(13, 60, width+200, height+200, 200, 200, "LEFT.png", "LEFT_HL.png")); }
        out_of_screen = true;
    }
    else { bciinterface->AddObject(new SSVEPStimulus(13,60, orig_x - width/6, orig_y, 200, 200, "LEFT.png", "LEFT_HL.png")); }
}

int main(int argc, char * argv[])
{
    bool fullscreen = true;
    unsigned int width = 1280;
    unsigned int height = 800;
    bool compressed_data = true;

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

    bciinterface->SetBackground(new VisionServerBG("hrp2010v", 4242, 640, 480, compressed_data, width, height));
    
    int * out_cmd = new int(-1);
    float timeout = 2;
    int cross_x = 0;
    int cross_y = 0;
    sf::RenderWindow * app = 0;
    boost::thread th(boost::bind(&bciinterface::BCIInterface::DisplayLoop, bciinterface, app, fullscreen, out_cmd, timeout));

    while(*out_cmd != 0)
    {
        std::vector<std::string> commands = GenerateCommands(cross_x,cross_y);
        interpreter->SetCommands(commands);

        bciinterface->Clean();
        CreateObjects(bciinterface, cross_x, cross_y, (float)width, (float)height);
        bciinterface->StartParadigm();
        bciinterface->SetCommandInterpreter(interpreter);

        while(bciinterface->ParadigmStatus())
        {
            usleep(100000);
        }
        sleep(2);

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
        app->close();
    }

    delete bciinterface;
    delete interpreter;
    delete receiver;
    delete out_cmd;

    return 0;
}
