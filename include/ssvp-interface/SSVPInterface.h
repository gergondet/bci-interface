#ifndef _H_SSVPINTERFACE_H_
#define _H_SSVPINTERFACE_H_

#include <ssvp-interface/FlickeringSquare.h>

namespace ssvpinterface
{

struct SSVPInterfaceImpl;

class SSVPInterface
{
public:
    SSVPInterface(unsigned int width, unsigned int height);

    void AddSquare(FlickeringSquare * square);

    void AddSquare(int frequency, int screenFrequency, float x, float y, float size = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    void DisplayLoop();

    void Close();
private:
    boost::shared_ptr<SSVPInterfaceImpl> m_impl;
};

} // namespace ssvpinterface

#endif

