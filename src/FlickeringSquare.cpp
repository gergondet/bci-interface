#include <ssvp-interface/FlickeringSquare.h>

#include <ssvp-interface/square-function.h>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace ssvpinterface
{

struct FlickeringSquareImpl
{
private:
    sf::Shape square;
    sf::Shape blackSquare;
    sf::Color on;
    sf::Color off;
    bool squareDisplay;
    int frequency;
    int screenFrequency;
    std::vector<bool> frameSeq;
public:
    FlickeringSquareImpl(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a, bool fill) : 
        square(sf::Shape::Rectangle(x, y, x+size, y+size, sf::Color(r, g, b, a), 0, sf::Color(r,g,b,a))) , 
        blackSquare(sf::Shape::Rectangle(x, y, x+size, y+size, sf::Color(0, 0, 0, 255))) ,
        on(r,g,b,a), off(0,0,0,255), squareDisplay(true),
        frequency(frequency) , screenFrequency(screenFrequency)
    {
        blackSquare.EnableOutline(true);
        blackSquare.SetOutlineWidth(3);
        if(!fill)
        {
            square.EnableFill(false);
            blackSquare.EnableFill(false);
            square.EnableOutline(true);
            blackSquare.SetOutlineWidth(15);
            square.SetOutlineWidth(10);
        }

        std::vector< std::pair<int, int> > tmpSeq;
        if(squarefunction(frequency, screenFrequency, tmpSeq))
        {
            for(unsigned int i = 0; i < tmpSeq.size(); ++i)
            {
                for(int j = 0; j < tmpSeq[i].first; ++j)
                {
                    frameSeq.push_back(true);
                }
                for(int j = 0; j < tmpSeq[i].second; ++j)
                {
                    frameSeq.push_back(false);
                }
            }
        }
    }
    ~FlickeringSquareImpl()
    {
    }

    bool ChangeFrequency(int frequency, int screenFrequency)
    {
        std::vector< std::pair<int, int> > tmpSeq;
        std::vector<bool> newFrameSeq;
        if(squarefunction(frequency, screenFrequency, tmpSeq))
        {
            for(unsigned int i = 0; i < tmpSeq.size(); ++i)
            {
                for(int j = 0; j < tmpSeq[i].first; ++j)
                {
                    newFrameSeq.push_back(true);
                }
                for(int j = 0; j < tmpSeq[i].second; ++j)
                {
                    newFrameSeq.push_back(false);
                }
            }
        }
        else
        {
            return false;
        }
        this->frequency = frequency;
        this->screenFrequency = screenFrequency;
        this->frameSeq = newFrameSeq;
        return true;
    }

    void UpdateForNewFrame(unsigned int frameIndex)
    {
        squareDisplay = frameSeq[frameIndex % screenFrequency];
    }

    void SetSquareDisplay(bool const display)
    {
        this->squareDisplay = display;
    }

    bool SquareDisplay()
    {
        return squareDisplay;
    }

    sf::Shape * GetShape()
    {
        return &square;
    }

    sf::Shape * GetBlackShape()
    {
        return &blackSquare;
    }

}; //class FlickeringSquareImpl

FlickeringSquare::FlickeringSquare(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a, bool fill) :
    m_flsqimpl(new FlickeringSquareImpl(frequency, screenFrequency, x, y, size, r, g, b, a, fill))
{
}

bool FlickeringSquare::ChangeFrequency(int frequency, int screenFrequency)
{
    return m_flsqimpl->ChangeFrequency(frequency, screenFrequency);
}

void FlickeringSquare::UpdateForNewFrame(unsigned int frameIndex)
{
    m_flsqimpl->UpdateForNewFrame(frameIndex);
}

void FlickeringSquare::SetSquareDisplay(bool const display)
{
    m_flsqimpl->SetSquareDisplay(display);
}

bool FlickeringSquare::SquareDisplay()
{
    return m_flsqimpl->SquareDisplay();
}

sf::Shape * FlickeringSquare::GetShape()
{
    return m_flsqimpl->GetShape();
}

sf::Shape * FlickeringSquare::GetBlackShape()
{
    return m_flsqimpl->GetBlackShape();
}

} // namespace ssvpinterface

