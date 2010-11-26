#include <ssvp-interface/SSVPInterfaceConfig.h>

#include <string>
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

/* Return false if commentary or wrong line, true if succesfull parse */
bool SSVPInterfaceConfig::ParseGeneralConfig(const std::string & configLine)
{
    if(configLine.size() == 0) return false;
    if(configLine[0] == '#') return false;

    std::stringstream config;
    config << configLine;
    config >> m_width >> m_height >> m_fullscreen;
    return true;
}

FlickeringSquare * SSVPInterfaceConfig::ParseSquareConfig(const std::string & configLine)
{
    if(configLine.size() == 0) return false;
    if(configLine[0] == '#') return false;

    int frequency, screenFrequency;
    float x, y, size;
    int r,g,b;
    bool fill;
    
    std::stringstream config;
    config << configLine;
    config >> frequency >> screenFrequency >> x >> y >> size >> r >> g >> b >> fill;
    return new FlickeringSquare(frequency, screenFrequency, x, y, UP, size, r, g, b, 255, fill);
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

    std::string configLine;
    getline(config, configLine);
    while(!ParseGeneralConfig(configLine))
    {
        getline(config, configLine);
    }

    while(config.good())
    {
        getline(config, configLine);
        FlickeringSquare * square = ParseSquareConfig(configLine);
        if(square)
        {
            m_squares.push_back(square);
        }
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

