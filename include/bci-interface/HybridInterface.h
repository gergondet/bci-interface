#ifndef _H_HYBRIDINTERFACE_H_
#define _H_INTERFACE_H_

#include <bci-interface/SSVEPInterface.h>
#include <bci-interface/P300Interface.h>

#ifdef WIN32
#define HYBRIDINTERFACE_API __declspec(dllexport)
#else
#define HYBRIDINTERFACE_API 
#endif

namespace bciinterface
{

struct HybridInterfaceImpl;

class HybridInterface 
{
public:
    HYBRIDINTERFACE_API HybridInterface(unsigned int width, unsigned int height, unsigned int mode);

    HYBRIDINTERFACE_API void DisplayLoop(bool fullscreen = true);

    /* P300 objects related functions */

    HYBRIDINTERFACE_API void AddObject(P300Object * object);

    HYBRIDINTERFACE_API void RemoveObject(const std::string & name);

    HYBRIDINTERFACE_API void ClearObjects();

    /* SSVEP flickering squares related functions */
    HYBRIDINTERFACE_API void AddSquare(FlickeringSquare * square);

    HYBRIDINTERFACE_API void AddSquare(int frequency, int screenFrequency, float x, float y, float size_x = 100, float size_y = 100, int r = 255, int g = 0, int b = 0, int a = 255);

private:
    boost::shared_ptr<HybridInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif

