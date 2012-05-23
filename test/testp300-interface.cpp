#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/StaticBG.h>
#include <bci-interface/DisplayObject/P300Object.h>
#include <bci-interface/CommandReceiver/TCPClient.h>
#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/Utils/Win32.h>

#include <SFML/Graphics.hpp>

#include <boost/thread.hpp>

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
    bool fullscreen = false;
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
    TCPClient * receiver = new TCPClient("150.29.145.92", 4242);
    SimpleInterpreter * interpreter = new SimpleInterpreter();
    bciinterface->SetCommandReceiver(receiver);
    bciinterface->SetCommandInterpreter(interpreter);

    bciinterface->SetBackground(new StaticBG("static.png", width, height, iwidth, iheight));
    unsigned int square_size = 4;
    for(int j = 0; j < square_size; ++j)
    {
        for(int i = 0; i < square_size; ++i)
        {
            bciinterface->AddObject(new P300Object(80, (width - iwidth)/2 + i*iwidth/square_size + iwidth/(2*square_size), (height - iheight)/2 + j*iheight/square_size + iheight/(2*square_size), 100, 100, 255, 0, 0));
        }
    }
    
    int out_cmd = -1;
    sf::RenderWindow * app = 0;
    boost::thread th(boost::bind(&bciinterface::BCIInterface::DisplayLoop, bciinterface, app, fullscreen, &out_cmd, 0));
    while(out_cmd != 0)
    {
        receiver->Send("resume");
        while(bciinterface->ParadigmStatus())
        {
            usleep(100000);
        }
    }

    if(app)
    {
        app->close();
    }

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
