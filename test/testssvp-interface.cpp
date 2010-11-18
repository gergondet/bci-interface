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
    for(int timer = 0; timer < 5; ++timer)
    {
        std::cout << timer << std::endl;
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

    interface->AddSquare(new FlickeringSquare(5,60, winW/2-50, 50, DOWN));
    interface->AddSquare(new FlickeringSquare(10,60, winW-150, winH/2-50, LEFT));
    interface->AddSquare(new FlickeringSquare(15,60, winW/2-50, winH-150, UP));
    interface->AddSquare(new FlickeringSquare(20,60, 50, winH/2-50, RIGHT));

    boost::thread th(boost::bind(&SSVPInterface::DisplayLoop, interface, true));
    boost::thread th2(&otherLoop);
    
    th.join();
    th2.join();

    return 0;
}
