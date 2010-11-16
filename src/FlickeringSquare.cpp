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
    int frequency;
    int screenFrequency;
    std::vector<bool> frameSeq;
public:
    FlickeringSquareImpl(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a) : 
        square(sf::Shape::Rectangle(x, y, x+size, y+size, sf::Color(r, g, b, a))) , 
        on(r,g,b,a) , off(r,g,b,0) ,
        frequency(frequency) , screenFrequency(screenFrequency)
    {
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
}; //class FlickeringSquareImpl

FlickeringSquare::FlickeringSquare(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a) :
    m_flsqimpl(new FlickeringSquareImpl(frequency, screenFrequency, x, y, size, r, g, b, a))
{
}

void FlickeringSquare::UpdateForNewFrame(unsigned int frameIndex)
{
    m_flsqimpl->UpdateForNewFrame(frameIndex);
}

sf::Shape * FlickeringSquare::GetShape()
{
    return m_flsqimpl->GetShape();
}

} // namespace ssvpinterface

