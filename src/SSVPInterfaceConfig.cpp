#include <ssvp-interface/SSVPInterfaceConfig.h>

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>

namespace ssvpinterface
{

SSVPInterfaceConfig::SSVPInterfaceConfig() :
    m_width(1280), m_height(800), m_fullscreen(true)
{
    m_squares.resize(0);
}

void SSVPInterfaceConfig::ReadFromFile(const std::string & fileName)
{
    std::stringstream fullFileName;
    char * cwd = new char[256];
    getcwd(cwd, 256);
    fullFileName << cwd << "/" << fileName;
    delete cwd;
    std::cout << "Open config: " << fullFileName.str() << std::endl;

    std::ifstream config(fullFileName.str().c_str());
    
    if(!config)
    {
        std::cerr << "Config file does not exist!" << std::endl;
    }

    /* First line:
        width height fullscreen? */
    config >> m_width >> m_height >> m_fullscreen;

    /* After that only lines for squares */
    while(config.good())
    {
        int frequency, screenFrequency;
        float x, y, size;
        int r,g,b,a;
        bool fill;

        config >> frequency >> screenFrequency >> x >> y >> size >> r >> g >> b >> a >> fill;
        m_squares.push_back(new FlickeringSquare(frequency, screenFrequency, x, y, UP, size, r, g, b, a, fill));
    }

    config.close();
}

int SSVPInterfaceConfig::GetWidth()
{
    return m_width;
}

int SSVPInterfaceConfig::GetHeight()
{
    return m_height;
}

bool SSVPInterfaceConfig::IsFullScreen()
{
    return m_fullscreen;
}


std::vector<FlickeringSquare *> & SSVPInterfaceConfig::GetSquares()
{
    return m_squares;
}

} //namespace ssvpinterface

