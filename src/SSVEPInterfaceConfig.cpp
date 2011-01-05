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

float SSVPInterfaceConfig::ParseOperation(std::string & operationLine)
{
    size_t found;
    while( (found = operationLine.find("winH")) != std::string::npos )
    {
        std::stringstream height;
        height << m_height;
        operationLine.replace(found, 4, height.str());
    }
    while( (found = operationLine.find("winW")) != std::string::npos )
    {
        std::stringstream width;
        width << m_width;
        operationLine.replace(found, 4, width.str());
    }
    
    float result = 0;
    float opN;
    char  op;
    std::stringstream operation;
    operation << operationLine;

    operation >> result;
    while(operation.good())
    {
        operation >> op;
        switch(op)
        {
            case '+':
                operation >> opN;
                result = result+opN;
                break;
            case '-':
                operation >> opN;
                result = result-opN;
                break;
            case '*':
                operation >> opN;
                result = result*opN;
                break;
            case '/':
                operation >> opN;
                result = result/opN;
                break;
            default:
                break;
        }
    }
    return result;
}

FlickeringSquare * SSVPInterfaceConfig::ParseSquareConfig(const std::string & configLine)
{
    if(configLine.size() == 0) return false;
    if(configLine[0] == '#') return false;

    int frequency, screenFrequency;
    float x, y, size;
    int r,g,b;
    bool fill;
    std::string opString;
    
    std::stringstream config;
    config << configLine;
    config >> frequency >> screenFrequency;

    config >> opString;
    x = ParseOperation(opString);
    config >> opString;
    y = ParseOperation(opString);

    config >> size >> r >> g >> b >> fill;

    return new FlickeringSquare(frequency, screenFrequency, x, y, size, r, g, b, 255, fill);
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

