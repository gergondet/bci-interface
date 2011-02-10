#include <bci-interface/SSVEPInterface.h>
#include <bci-interface/SSVEPInterfaceConfig.h>

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

#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace bciinterface;

SSVEPInterface * interface = 0;

int main(int argc, char * argv[])
{
    bool fullscreen = true;
    if(argc > 1)
    {
        std::string configName = argv[1];
        SSVEPInterfaceConfig config;
        config.ReadFromFile(configName);

        interface = new SSVEPInterface(config.GetWidth(), config.GetHeight());

        fullscreen = config.IsFullScreen();

        for(unsigned int i = 0; i < config.GetSquares().size(); ++i)
        {
            interface->AddSquare(config.GetSquares()[i]);
        }

    }
    else
    {
        int winW = 1600;
        int winH = 1200;

        interface = new SSVEPInterface(winW, winH);

        interface->AddSquare(new FlickeringSquare(5,60, winW/2-75, 50, 150, 255, 0, 0, 255, true));
        interface->AddSquare(new FlickeringSquare(7,60, winW-350, winH/2-50, 150, 255, 0, 0, 255, true));
        interface->AddSquare(new FlickeringSquare(9,60, winW/2-75, winH-200, 150, 255, 0, 0, 255, true));
        interface->AddSquare(new FlickeringSquare(11,60, 200, winH/2-50, 150, 255, 0, 0, 255, true));

        interface->AddCursor(new MoovingCursor(320, winH/2-50, 150, winW/3, 0, 255, 0, 255));

    }

    BackgroundSprite * background = new BackgroundSprite("hrp2010v", 4242, 640, 480);
    interface->SetBackgroundSprite(background);
    void (SSVEPInterface::*fn)(bool) = &SSVEPInterface::DisplayLoop;
    boost::thread th(fn, interface, fullscreen);
    
    th.join();

    return 0;
}
