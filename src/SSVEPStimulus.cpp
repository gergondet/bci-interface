#include <bci-interface/SSVEPStimulus.h>

#include <bci-interface/square-function.h>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct SSVEPStimulusImpl
{
private:
    sf::Shape m_shape;
    int m_frequency;
    int m_screenFrequency;
    std::vector<bool> m_frameSeq;
public:
    SSVEPStimulusImpl(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a) : 
        m_shape(sf::Shape::Rectangle(x - size_x/2, y - size_y/2, x + size_x/2, y + size_y/2, sf::Color(r, g, b, a), 6, sf::Color(0, 255, 0, 128))) , 
        m_frequency(frequency) , m_screenFrequency(screenFrequency)
    {
        m_shape.EnableOutline(false);
        std::vector< std::pair<int, int> > tmpSeq;
        if(squarefunction(m_frequency, m_screenFrequency, tmpSeq))
        {
            for(unsigned int i = 0; i < tmpSeq.size(); ++i)
            {
                for(int j = 0; j < tmpSeq[i].first; ++j)
                {
                    m_frameSeq.push_back(true);
                }
                for(int j = 0; j < tmpSeq[i].second; ++j)
                {
                    m_frameSeq.push_back(false);
                }
            }
        }
    }
    ~SSVEPStimulusImpl()
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
        m_frequency = frequency;
        m_screenFrequency = screenFrequency;
        m_frameSeq = newFrameSeq;
        return true;
    }

    void Highlight()
    {
        m_shape.EnableOutline(true);
    }

    void Unhighlight()
    {
        m_shape.EnableOutline(false);
    }

    void Move(float diffX, float diffY)
    {
        m_shape.Move(diffX, diffY);
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount)
    {
        if(m_frameSeq[frameCount % m_screenFrequency])
        {
            app->Draw(m_shape);
        }
    }

}; //class SSVEPStimulusImpl

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency, x, y, size_x, size_y, r, g, b, a))
{
}

bool SSVEPStimulus::ChangeFrequency(int frequency, int screenFrequency)
{
    return m_impl->ChangeFrequency(frequency, screenFrequency);
}

void SSVEPStimulus::Highlight()
{
    m_impl->Highlight();
}

void SSVEPStimulus::Unhighlight()
{
    m_impl->Unhighlight();
}

void SSVEPStimulus::Move(float diffX, float diffY)
{
    m_impl->Move(diffX, diffY);
}

void SSVEPStimulus::Display(sf::RenderWindow * app, unsigned int frameCount)
{
    m_impl->Display(app, frameCount);
}

} // namespace bciinterface

