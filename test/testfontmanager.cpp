#include <bci-interface/Utils/FontManager.h>

#include <SFML/Graphics/Font.hpp>

#include <iostream>

int main(int argc, char * argv[])
{
    std::cout << "Creating font manager" << std::endl;
    bciinterface::FontManager fm;
    std::cout << "Font manager created, fetching default font" << std::endl;

    sf::Font & arial = fm.GetDefaultFont();
    std::cout << "Got arial" << std::endl;

#ifndef WIN32
    sf::Font & free = fm.GetFont("FreeMono.ttf");
    std::cout << "Got FreeMono.ttf" << std::endl;
#endif

    return 0;
}
