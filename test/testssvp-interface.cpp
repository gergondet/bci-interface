#include <ssvp-interface/SSVPInterface.h>

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

using namespace ssvpinterface;

SSVPInterface * interface = 0;

void otherLoop()
{
    for(int timer = 0; timer < 15; ++timer)
    {
        std::cout << timer << std::endl;
        if(timer == 2)
        {
            interface->EnableCross(true);
        }
        if(timer >= 2)
        {
            interface->EnableArrow(timer - 2);
        }
        if(timer == 7)
        {
            interface->EnableFlash(false);
        }
        if(timer == 10)
        {
            interface->EnableFlash(true);
        }
        sleep(1);
    }
    if(interface)
    {
        interface->Close();
    }   
}

int main(int argc, char * argv[])
{
    int winW = 1280;
    int winH = 800;
    interface = new SSVPInterface(winW, winH);

    interface->AddSquare(new FlickeringSquare(5,60, winW/2-75, 50, DOWN, 150, 255, 0, 0));
    interface->AddSquare(new FlickeringSquare(7,60, winW-350, winH/2-50, LEFT, 150, 255, 0, 0));
    interface->AddSquare(new FlickeringSquare(9,60, winW/2-75, winH-200, UP, 150, 255, 0, 0));
    interface->AddSquare(new FlickeringSquare(11,60, 200, winH/2-50, RIGHT, 150, 255, 0, 0));

    boost::thread th(&SSVPInterface::DisplayLoop, interface, false);
    
    th.join();

    return 0;
}
