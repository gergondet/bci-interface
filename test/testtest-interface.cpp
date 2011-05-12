#include <bci-interface/TestInterface.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

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

TestInterface * interface = 0;

int main(int argc, char * argv[])
{
    bool fullscreen = true;
   int winW = 1280;
   int winH = 800;

   interface = new TestInterface(winW, winH);

   /*(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a, bool fill)*/
   /* Squares */
   //interface->AddSquare(new FlickeringSquare(7,60, winW/2-75, 50, 150, 150, 255, 0, 0, 255, true));
   //interface->AddSquare(new FlickeringSquare(12,60, winW-225, winH/2-75, 150, 150, 255, 0, 0, 255, true));
   //interface->AddSquare(new FlickeringSquare(5,60, winW/2-75, winH-200, 150, 150, 255, 0, 0, 255, true));
   //interface->AddSquare(new FlickeringSquare(9,60, 75, winH/2-75, 150, 150, 255, 0, 0, 255, true));

   /* Circles */
   interface->AddSquare(new FlickeringSquare(7,60, winW/2, winH/2, 40, 255, 0, 0, 255, true));
   /*interface->AddSquare(new FlickeringSquare(12,60, winW-150, winH/2, 75, 255, 0, 0, 255, true));
   interface->AddSquare(new FlickeringSquare(5,60, winW/2, winH-150, 75, 255, 0, 0, 255, true));
   interface->AddSquare(new FlickeringSquare(9,60, 150, winH/2, 75, 255, 0, 0, 255, true));*/

   interface->ClearPositionsTabs();
   /*interface->AddPositionsTab(25);
   interface->AddPositionsTab(100+(winW-350)/6);
   interface->AddPositionsTab(100+2*(winW-350)/6);
   interface->AddPositionsTab(100+3*(winW-350)/6);
   interface->AddPositionsTab(100+4*(winW-350)/6);
   interface->AddPositionsTab(100+5*(winW-350)/6);*/

   /*(float posXSquareLeft, float posXSquareRight, float size, int numPos)*/
   interface->AddPositionsTab(25, winW - 175, 150, 6);

   /*(x, y, size, final_position, r, g, b, a)*/
   //interface->AddCursor(new MoovingCursor(0, winH/2-50, 150, 0, 255, 0, 0, 255));

   /*( x, y, size, period, r, g, b, a)*/
   interface->AddPoint(new RandomPoint(200, 200, 10, 100, 0, 255, 0, 255));

    void (TestInterface::*fn)(bool) = &TestInterface::DisplayLoop;
    boost::thread th(fn, interface, fullscreen);
    
    th.join();

    return 0;
}
