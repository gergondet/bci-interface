#include <bci-interface/P300Interface.h>

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

P300Interface * interface = 0;

int main(int argc, char * argv[])
{
    bool fullscreen = true;
    int winW = 1280;
    int winH = 800;

    interface = new P300Interface(winW, winH);

    interface->AddObject(new P300Object("1", winW/2-75, 50, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("2", winW-350, winH/2-50, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("3", winW/2-75, winH-200, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("4", 200, winH/2-50, 150, 150, 255, 0, 0));

    interface->StartP300Client("127.0.0.1", 4242);

    boost::thread th(&P300Interface::DisplayLoop, interface, fullscreen);
    
    th.join();

    return 0;
}
