#ifndef _H_TESTINTERFACE_H_
#define _H_TESTINTERFACE_H_

#include <bci-interface/FlickeringSquare.h>
#include <bci-interface/MoovingCursor.h>
#include <bci-interface/RandomPoint.h>

#include <vector>
#include <string>

#ifdef WIN32
#define TESTINTERFACE_API __declspec(dllexport)
#else
#define TESTINTERFACE_API 
#endif

namespace bciinterface
{

struct TestInterfaceImpl;

class TestInterface
{
public:
    TESTINTERFACE_API TestInterface(unsigned int width, unsigned int height);

    TESTINTERFACE_API void AddSquare(FlickeringSquare * square);

    TESTINTERFACE_API void AddSquare(int frequency, int screenFrequency, float x, float y, float size_x = 100, float size_y = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    TESTINTERFACE_API void CleanUpSquares();

    TESTINTERFACE_API void AddCursor(MoovingCursor * moovingCursor);

    TESTINTERFACE_API void AddCursor(float x_init=0, float y_init=0, float size=100, float x_fin=500, int r=0, int g=0, int b=0, int a=255);

    TESTINTERFACE_API void AddPoint(RandomPoint * randomPoint);

    TESTINTERFACE_API void AddPoint(float x=0, float y=0, float size=5, float period=300, int r=0, int g=0, int b=0, int a=255);

    TESTINTERFACE_API void ClearPositionsTabs();

    TESTINTERFACE_API void AddPositionsTab(float positionX);

    TESTINTERFACE_API void AddPositionsTab(float posXSquareLeft, float posXSquareRight, float size, int numPos);

    TESTINTERFACE_API void ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency);

    TESTINTERFACE_API void EnableFlash(bool enable);

    TESTINTERFACE_API void DisplayLoop(bool fullScreen = true);

    TESTINTERFACE_API void Close();


private:
    boost::shared_ptr<TestInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif

