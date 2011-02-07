#ifndef _H_SSVEPINTERFACE_H_
#define _H_SSVEPINTERFACE_H_

#include <bci-interface/FlickeringSquare.h>

#ifdef WIN32
#define SSVEPINTERFACE_API __declspec(dllexport)
#else
#define SSVEPINTERFACE_API 
#endif

namespace bciinterface
{

struct SSVEPInterfaceImpl;

class SSVEPInterface
{
public:
    SSVEPINTERFACE_API SSVEPInterface(unsigned int width, unsigned int height);

    SSVEPINTERFACE_API void AddSquare(FlickeringSquare * square);

    SSVEPINTERFACE_API void AddSquare(int frequency, int screenFrequency, float x, float y, float size = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    SSVEPINTERFACE_API void ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency);

    SSVEPINTERFACE_API void EnableFlash(bool enable);

    SSVEPINTERFACE_API void DisplayLoop(sf::RenderWindow * app);

    SSVEPINTERFACE_API void DisplayLoop(bool fullScreen = true);

    SSVEPINTERFACE_API void Close();
private:
    boost::shared_ptr<SSVEPInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif

