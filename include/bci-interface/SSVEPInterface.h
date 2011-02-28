#ifndef _H_SSVEPINTERFACE_H_
#define _H_SSVEPINTERFACE_H_

#include <bci-interface/FlickeringSquare.h>
#include <bci-interface/MoovingCursor.h>
#include <bci-interface/BackgroundSprite.h>

#include <vector>
#include <string>

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

    SSVEPINTERFACE_API void AddSquare(int frequency, int screenFrequency, float x, float y, float size_x = 100, float size_y = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    SSVEPINTERFACE_API void CleanUpSquares();

    SSVEPINTERFACE_API void AddCursor(MoovingCursor * moovingCursor);

    SSVEPINTERFACE_API void AddCursor(float x_init=0, float y_init=0, float size=100, float x_fin=500, int r=0, int g=0, int b=0, int a=255);

    SSVEPINTERFACE_API void ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency);

    SSVEPINTERFACE_API void SetBackgroundSprite(BackgroundSprite * sprite);

    SSVEPINTERFACE_API void SetUpdateBackgroundManually(bool enable);

    SSVEPINTERFACE_API void UpdateBackground(unsigned char * img);

    SSVEPINTERFACE_API void EnableFlash(bool enable);

    SSVEPINTERFACE_API void SetCoshellCommands(const std::vector<std::string> & commands);

    SSVEPINTERFACE_API void DisplayLoop(sf::RenderWindow * app, unsigned int * cmd = 0, float timeout = 0);

    SSVEPINTERFACE_API void DisplayLoop(bool fullScreen = true);

    SSVEPINTERFACE_API void Close();
private:
    boost::shared_ptr<SSVEPInterfaceImpl> m_impl;
};

} // namespace bciinterface

#endif

