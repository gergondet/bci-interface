#ifndef _H_SSVEPINTERFACECONFIG_H_
#define _H_SSVEPINTERFACECONFIG_H_

#include <bci-interface/FlickeringSquare.h>

#ifdef WIN32
#define SSVEPINTERFACE_API __declspec(dllexport)
#else
#define SSVEPINTERFACE_API 
#endif

namespace bciinterface
{

class SSVEPInterfaceConfig
{
public:
    SSVEPINTERFACE_API SSVEPInterfaceConfig();

    SSVEPINTERFACE_API void ReadFromFile(const std::string & fileName);

    SSVEPINTERFACE_API int GetWidth();

    SSVEPINTERFACE_API int GetHeight();

    SSVEPINTERFACE_API bool IsFullScreen();

    SSVEPINTERFACE_API std::vector<FlickeringSquare *>  & GetSquares();

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

} // namespace bciinterface

#endif

