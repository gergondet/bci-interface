#include "bci-interface/DisplayObject/FPSCounter.h"

#include <SFML/Graphics.hpp>

#include <sstream>

namespace bciinterface
{

struct FPSCounterImpl
{
    FPSCounterImpl() : initFrameCount(0), timeAsMicroSeconds(0)
    {}

    inline void reset(unsigned int && iF, sf::Int64 && tM)
    {
        initFrameCount = iF;
        timeAsMicroSeconds = tM;
    }

    inline std::string getNewText(unsigned int && iF, sf::Int64 && tM)
    {
        std::stringstream ss;
        ss << "FPS: ";
        ss << 1e6*(iF - initFrameCount)/(double)(tM - timeAsMicroSeconds);
        reset(std::move(iF), std::move(tM));
        return ss.str();
    }

    unsigned int initFrameCount;
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
    if(impl->initFrameCount == 0)
    {
        impl->reset(std::move(frameCount), clock.getElapsedTime().asMicroseconds());
    }
    if(frameCount - impl->initFrameCount > 99)
    {
        SetText( impl->getNewText(std::move(frameCount), clock.getElapsedTime().asMicroseconds()) );
    }
    TextObject::Display(app, frameCount, clock);
}

}
