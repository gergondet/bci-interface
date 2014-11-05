#include "bci-interface/DisplayObject/FPSCounter.h"

#include <SFML/Graphics.hpp>

#include <sstream>

namespace bciinterface
{

struct FPSCounterImpl
{
    FPSCounterImpl() : fc(0), timeAsMicroSeconds(0)
    {}

    inline void reset(sf::Int64 && tM)
    {
        fc = 0;
        timeAsMicroSeconds = tM;
    }

    inline std::string getNewText(sf::Int64 && tM)
    {
        std::stringstream ss;
        ss << "FPS: ";
        ss << 1e6*(100)/(double)(tM - timeAsMicroSeconds);
        reset(std::move(tM));
        return ss.str();
    }

    unsigned int fc;
    sf::Int64 timeAsMicroSeconds;
};

FPSCounter::FPSCounter(sf::Font & font) : TextObject(font, "FPS: 0"), impl(new FPSCounterImpl())
{
    SetPosition(10, 10);
    SetCharacterSize(40);
    SetColor(sf::Color::Red);
}

void FPSCounter::Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock)
{
    impl->fc++;
    if(impl->fc > 99)
    {
        SetText( impl->getNewText(clock.getElapsedTime().asMicroseconds()) );
    }
    TextObject::Display(app, frameCount, clock);
}

}
