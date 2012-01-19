#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/StaticBG.h>
#include <bci-interface/DisplayObject/P300Object.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
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
    unsigned int iwidth = 1024;
    unsigned int iheight = 768;

    BCIInterface * bciinterface = new BCIInterface(width, height);
    UDPReceiver * receiver = new UDPReceiver(1111);
    SimpleInterpreter * interpreter = new SimpleInterpreter();
    bciinterface->SetCommandReceiver(receiver);
//    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->SetBackground(new StaticBG("static.png", width, height, iwidth, iheight));
    unsigned int square_size = 6;
    for(int i = 0; i < square_size; ++i)
    {
        for(int j = 0; j < square_size; ++j)
        {
            bciinterface->AddObject(new P300Object(80, (width - iwidth)/2 + i*iwidth/square_size + iwidth/(2*square_size), (height - iheight)/2 + j*iheight/square_size + iheight/(2*square_size), 100, 100, 255, 0, 0));
        }
    }
    
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
