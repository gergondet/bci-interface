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
    sf::Color on;
    sf::Color off;
    sf::Image calibrationImage;
    sf::Sprite calibrationArrow;
    bool arrowDisplay;
    int frequency;
    int screenFrequency;
    std::vector<bool> frameSeq;
public:
    FlickeringSquareImpl(int frequency, int screenFrequency, float x, float y, ArrowPosition arrowPos, float size, int r, int g, int b, int a) : 
        square(sf::Shape::Rectangle(x, y, x+size, y+size, sf::Color(r, g, b, a))) , 
        on(r,g,b,a) , off(r,g,b,0) ,
        arrowDisplay(false),
        frequency(frequency) , screenFrequency(screenFrequency)
    {
        calibrationImage.LoadFromFile("data/arrow.png");
        calibrationArrow = sf::Sprite(calibrationImage);
        float arrowRotation = 0;
        float newArrowX = 0;
        float newArrowY = 0;
        switch(arrowPos)
        {
            case RIGHT:
                calibrationArrow.Rotate(90);
                calibrationArrow.SetCenter(calibrationArrow.GetSize().x/2, calibrationArrow.GetSize().y/2);
                calibrationArrow.SetX(x+size+calibrationArrow.GetSize().x);
                calibrationArrow.SetY(y+size/2);
                break;
            case DOWN:
                calibrationArrow.SetCenter(calibrationArrow.GetSize().x/2, calibrationArrow.GetSize().y/2);
                calibrationArrow.SetX(x+size/2);
                calibrationArrow.SetY(y+size+calibrationArrow.GetSize().y);
                break;
            case LEFT:
                calibrationArrow.Rotate(270);
                calibrationArrow.SetCenter(calibrationArrow.GetSize().x/2, calibrationArrow.GetSize().y/2);
                calibrationArrow.SetX(x-calibrationArrow.GetSize().x);
                calibrationArrow.SetY(y+size/2);
                break;
            default:
                calibrationArrow.Rotate(180);
                calibrationArrow.SetCenter(calibrationArrow.GetSize().x/2, calibrationArrow.GetSize().y/2);
                calibrationArrow.SetX(x+size/2);
                calibrationArrow.SetY(y-calibrationArrow.GetSize().y);
                break;
        }

        std::vector< std::pair<int, int> > tmpSeq;
        if(squarefunction(frequency, screenFrequency, tmpSeq))
        {
            for(int i = 0; i < tmpSeq.size(); ++i)
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
            for(int i = 0; i < tmpSeq.size(); ++i)
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
        if(frameSeq[frameIndex % screenFrequency])
        {
            square.SetColor(on);
        }
        else
        {
            square.SetColor(off);
        }
    }
    sf::Shape * GetShape()
    {
        return &square;
    }

    void SetArrowDisplay(bool const arrowDisplay)
    {
        this->arrowDisplay = arrowDisplay;
    }

    bool ArrowDisplay()
    {
        return arrowDisplay;
    }

    sf::Sprite * GetArrow()
    {
        return &calibrationArrow;
    }
}; //class FlickeringSquareImpl

FlickeringSquare::FlickeringSquare(int frequency, int screenFrequency, float x, float y, ArrowPosition arrowPos, float size, int r, int g, int b, int a) :
    m_flsqimpl(new FlickeringSquareImpl(frequency, screenFrequency, x, y, arrowPos, size, r, g, b, a))
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

sf::Shape * FlickeringSquare::GetShape()
{
    return m_flsqimpl->GetShape();
}

void FlickeringSquare::SetArrowDisplay(bool const arrowDisplay)
{
    m_flsqimpl->SetArrowDisplay(arrowDisplay);
}

bool FlickeringSquare::ArrowDisplay()
{
    return m_flsqimpl->ArrowDisplay();
}

sf::Sprite * FlickeringSquare::GetArrow()
{
    return m_flsqimpl->GetArrow();
}

} // namespace ssvpinterface
