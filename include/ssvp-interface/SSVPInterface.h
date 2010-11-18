#ifndef _H_SSVPINTERFACE_H_
#define _H_SSVPINTERFACE_H_

#include <ssvp-interface/FlickeringSquare.h>

#ifdef WIN32
#define SSVPINTERFACE_API __declspec(dllexport)
#else
#define SSVPINTERFACE_API 
#endif

namespace ssvpinterface
{

struct SSVPInterfaceImpl;

class SSVPInterface
{
public:
    SSVPINTERFACE_API SSVPInterface(unsigned int width, unsigned int height);

    SSVPINTERFACE_API void AddSquare(FlickeringSquare * square);

    SSVPINTERFACE_API void AddSquare(int frequency, int screenFrequency, float x, float y, ArrowPosition arrowPos = UP, float size = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    SSVPINTERFACE_API void DisplayLoop(bool fullScreen = true);

    SSVPINTERFACE_API void Close();
private:
    boost::shared_ptr<SSVPInterfaceImpl> m_impl;
};

} // namespace ssvpinterface

#endif

