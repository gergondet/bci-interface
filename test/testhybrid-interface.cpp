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

int main(int argc, char * argv[])
{
    bool fullscreen = false;
    int winW = 1280;
    int winH = 800;
    HybridInterface * interface = new HybridInterface(winW, winH, 2);

    void (HybridInterface::*fn)(bool) = &HybridInterface::DisplayLoop;
    boost::thread th(fn, interface, fullscreen);

    th.join();

    return 0;
}
