#include <bci-interface/Utils/FontManager.h>

#include <SFML/Graphics/Font.hpp>

#include <iostream>

int main(int argc, char * argv[])
{
    bciinterface::FontManager fm;

    sf::Font & arial = fm.GetDefaultFont();
    std::cout << "Got arial" << std::endl;

    sf::Font & free = fm.GetFont("FreeMono.ttf");
    std::cout << "Got FreeMono.ttf" << std::endl;

    return 0;
}
