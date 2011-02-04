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
    unsigned int mode = 2; /* default to free type mode */
    std::stringstream ss;
    if(argc > 1)
    {
        ss << argv[1];
        ss >> mode;
    }

    bool fullscreen = true;
    int winW = 1280;
    int winH = 800;

    interface = new P300Interface(winW, winH, mode);
    BackgroundSprite * background = new BackgroundSprite("hrp2010v", 4242, 640, 480);
    interface->SetBackgroundSprite(background);

    interface->SetUpdateBackgroundManually(true);
    unsigned char * img = new unsigned char[640*480];
    for(int i = 0; i < 640*480; ++i)
    {
        img[i] = 51 * (i / (640*480/6));
    }
    interface->UpdateBackground(img);

    interface->AddObject(new P300Object("1", winW/2-75, 50, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("2", winW-350, winH/2-50, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("3", winW/2-75, winH-200, 150, 150, 255, 0, 0));
    interface->AddObject(new P300Object("4", 200, winH/2-50, 150, 150, 255, 0, 0));
//    interface->AddObject(new P300Object("1",  0*winW/6+25, 0*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("2",  1*winW/6+25, 0*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("3",  2*winW/6+25, 0*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("4",  3*winW/6+25, 0*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("5",  4*winW/6+25, 0*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("6",  5*winW/6+25, 0*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("7",  0*winW/6+25, 1*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("8",  1*winW/6+25, 1*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("9",  2*winW/6+25, 1*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("10", 3*winW/6+25, 1*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("11", 4*winW/6+25, 1*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("12", 5*winW/6+25, 1*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("13", 0*winW/6+25, 2*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("14", 1*winW/6+25, 2*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("15", 2*winW/6+25, 2*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("16", 3*winW/6+25, 2*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("17", 4*winW/6+25, 2*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("18", 5*winW/6+25, 2*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("19", 0*winW/6+25, 3*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("20", 1*winW/6+25, 3*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("21", 2*winW/6+25, 3*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("22", 3*winW/6+25, 3*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("23", 4*winW/6+25, 3*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("24", 5*winW/6+25, 3*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("25", 0*winW/6+25, 4*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("26", 1*winW/6+25, 4*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("27", 2*winW/6+25, 4*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("28", 3*winW/6+25, 4*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("29", 4*winW/6+25, 4*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("30", 5*winW/6+25, 4*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("31", 0*winW/6+25, 5*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("32", 1*winW/6+25, 5*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("33", 2*winW/6+25, 5*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("34", 3*winW/6+25, 5*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("35", 4*winW/6+25, 5*winH/6+25, 80,80, 255,0,0));
//    interface->AddObject(new P300Object("36", 5*winW/6+25, 5*winH/6+25, 80,80, 255,0,0));

    interface->StartP300Client("150.29.145.150", 4242);

    if(mode == 1)
    {
        interface->SetNbTrials(10);
    }
    else
    {
        interface->SetNbTrials(4);
    }

    void (P300Interface::*fn)(bool) = &P300Interface::DisplayLoop;
    boost::thread th(fn, interface, fullscreen);
    
    th.join();

    return 0;
}
