#include <bci-interface/HybridInterface.h>
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

HybridInterface * interface = 0;

int main(int argc, char * argv[])
{
    bool fullscreen = false;
    int winW = 640;
    int winH = 480;
    if(argc > 1)
    {
        std::string configName = argv[1];
        SSVEPInterfaceConfig config;
        config.ReadFromFile(configName);
        winW = config.GetWidth(); 
        winH = config.GetHeight();

        interface = new HybridInterface(config.GetWidth(), config.GetHeight(), 1);

        fullscreen = config.IsFullScreen();

        for(unsigned int i = 0; i < config.GetSquares().size(); ++i)
        {
            interface->AddSquare(config.GetSquares()[i]);
        }

    }
    else
    {
        interface = new HybridInterface(winW, winH, 1);

        interface->AddSquare(new FlickeringSquare(5,60, winW/2-75, 50, 150, 255, 0, 0, 255, false));
        interface->AddSquare(new FlickeringSquare(7,60, winW-350, winH/2-50, 150, 255, 0, 0, 255, false));
        interface->AddSquare(new FlickeringSquare(9,60, winW/2-75, winH-200, 150, 255, 0, 0, 255, false));
        interface->AddSquare(new FlickeringSquare(11,60, 200, winH/2-50, 150, 255, 0, 0, 255, false));
    }

    interface->AddObject(new P300Object("1", winW/2-75, 50, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("2", winW-350, winH/2-50, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("3", winW/2-75, winH-200, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("4", 200, winH/2-50, 150, 150, 255, 0, 0));

    void (HybridInterface::*fn)(bool) = &HybridInterface::DisplayLoop;
    boost::thread th(fn, interface, fullscreen);

    
    th.join();

    return 0;
}
