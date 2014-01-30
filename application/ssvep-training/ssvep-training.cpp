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
    unsigned int width = 1024;
    unsigned int height = 768;
    bool fullscreen = false;

    BCIInterface * bciinterface = new BCIInterface(width, height);

    FontManager fm;
    FPSCounter fps_c(fm.GetDefaultFont());

    UDPReceiver * receiver = new UDPReceiver(2222);
    TrainingInterpreter * interpreter = new TrainingInterpreter(width, height);
    bciinterface->SetCommandReceiver(receiver);
    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->AddObject(new SSVEPStimulus(6, 60, (float)width/2, 100, 200,200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, (float)width-100, (float)height/2, 200, 200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(10, 60, (float)width/2, (float)height-100, 200, 200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, 100, (float)height/2,200, 200, 255, 0, 0, 255));
    bciinterface->AddObject(new SSVEPStimulus(14, 60, (float)width/2, (float)height/2, 200, 200, 255, 0, 0, 255));
    bciinterface->AddNonOwnedObject(&fps_c);

    int cmd = -1;
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
