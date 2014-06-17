#include <bci-interface/BCIInterface.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/Utils/FontManager.h>
#include <bci-interface/DisplayObject/FPSCounter.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter.h>
#include <bci-interface/CommandOverrider.h>

#include <bci-interface/Background/BufferBG.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <OculusWindow.h>

#include <iostream>
#include <sstream>
#include <fstream>

#ifdef WIN32
#include <Windows.h>
void sleep(DWORD t)
{
	Sleep(1000*t);
}
#include <stdint.h>

std::string dirname(char * path)
{
    return ".";
}

#else
#include <inttypes.h>
#include <libgen.h>
#endif

using namespace bciinterface;

class TrainingInterpreter : public CommandInterpreter
{
public:
    TrainingInterpreter(unsigned int width, unsigned int height) : 
    CommandInterpreter(), width(width), height(height)
    {}

    bool InterpretCommand(int command, const std::vector<DisplayObject*> & objects)
    {
        if(command == 0)
        {
            for(size_t i =0; i < objects.size(); ++i)
            {
                objects[i]->SetPosition(-200, -200);
            }
        }
        else
        {
            if(command - 1 < objects.size())
            {
                objects[command-1]->SetPosition(width/2, height/2);
            }
        }
        return false;
    }
private:
    unsigned int width;
    unsigned int height;
};

int main(int argc, char * argv[])
{
    unsigned int width = 1280;
    unsigned int height = 800;

    /* Render width/height */
    unsigned int rwidth = width/2;
    unsigned int rheight = 480;

    BCIInterface * bciinterface = new BCIInterface(width, height);
    bciinterface->InitOculus();

    FontManager fm;

    rwidth = rwidth*bciinterface->GetRenderScale();
    rheight = rheight*bciinterface->GetRenderScale();

    UDPReceiver * receiver = new UDPReceiver(2222);
    TrainingInterpreter * interpreter = new TrainingInterpreter(rwidth, rheight);
    bciinterface->SetCommandReceiver(receiver);
    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->AddObject(new SSVEPStimulus(6, 60, (float)rwidth/2, 100, 200,200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, (float)rwidth-100, (float)rheight/2, 200, 200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(10, 60, (float)rwidth/2, (float)rheight-100, 200, 200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, 100, (float)rheight/2,200, 200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(14, 60, (float)rwidth/2, (float)rheight/2, 200, 200, 255, 0, 0, 255));

    int cmd = -1;
    while(!bciinterface->GetOculusWindow()); /* Wait for the full initialization of the oculus window */
    bciinterface->GetOculusWindow()->enableFPSCounter(fm.GetDefaultFont());
    bciinterface->OculusDisplayLoop(cmd);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
