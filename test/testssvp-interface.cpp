#include <ssvp-interface/SSVPInterface.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace ssvpinterface;

void otherLoop()
{
    for(int timer = 0; timer < 10; ++timer)
    {
        std::cout << timer << std::endl;
        #ifndef WIN32
        sleep(1);
        #else
        Sleep(1000);
        #endif
    }
}

int main(int argc, char * argv[])
{
    int winW = 1280;
    int winH = 800;
    SSVPInterface interface(winW, winH);

    interface.AddSquare(new FlickeringSquare(5,60, winW/2-50, 50));
    interface.AddSquare(new FlickeringSquare(10,60, winW-150, winH/2-50));
    interface.AddSquare(new FlickeringSquare(15,60, winW/2-50, winH-150));
    interface.AddSquare(new FlickeringSquare(20,60, 50, winH/2-50));

    boost::thread th(boost::bind(&SSVPInterface::DisplayLoop, &interface));
    boost::thread th2(&otherLoop);
    
    th.join();
    th2.join();

    return 0;
}
