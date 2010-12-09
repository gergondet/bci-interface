#ifndef _H_SSVPINTERFACECONFIG_H_
#define _H_SSVPINTERFACECONFIG_H_

#include <ssvp-interface/FlickeringSquare.h>

#ifdef WIN32
#define SSVPINTERFACE_API __declspec(dllexport)
#else
#define SSVPINTERFACE_API 
#endif

namespace ssvpinterface
{

class SSVPInterfaceConfig
{
public:
    SSVPINTERFACE_API SSVPInterfaceConfig();

    SSVPINTERFACE_API void ReadFromFile(const std::string & fileName);

    SSVPINTERFACE_API int GetWidth();

    SSVPINTERFACE_API int GetHeight();

    SSVPINTERFACE_API bool IsFullScreen();

    SSVPINTERFACE_API std::vector<FlickeringSquare *>  & GetSquares();

private:
    bool ParseGeneralConfig(const std::string & configLine);

    float ParseOperation(std::string & operationLine);

    FlickeringSquare * ParseSquareConfig(const std::string & configLine);

private:
    int m_width;
    int m_height;
    bool m_fullscreen;
    std::vector<FlickeringSquare *> m_squares;
};

} // namespace ssvpinterface

#endif
