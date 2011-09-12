#include <bci-interface/DisplayObject/SSVEPStimulus.h>

#include <bci-interface/Utils/square-function.h>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct GraphImpl
{
public:
    GraphImpl() {}
    virtual void Highlight() = 0;
    virtual void Unhighlight() = 0;
    virtual void Move(float diffX, float diffY) = 0;
    virtual void SetPosition(float X, float Y) = 0;
    virtual void Draw(sf::RenderWindow * app) = 0;
};

struct ShapeImpl : public GraphImpl
{
private:
    sf::Shape m_shape;
public:
    ShapeImpl(float x, float y, float size_x, float size_y, int r, int g, int b, int a) : 
        m_shape(sf::Shape::Rectangle(-size_x/2, -size_y/2, size_x, size_y, sf::Color(r, g, b, a), 6, sf::Color(0, 255, 0, 128))) 
    {
        m_shape.SetPosition(x,y);
        m_shape.EnableOutline(false);
    }

    ShapeImpl(float x, float y, float radius, int r, int g, int b, int a) : 
        m_shape(sf::Shape::Circle(0, 0, radius, sf::Color(r, g, b, a), 6, sf::Color(0, 255, 0, 128))) 
    {
        m_shape.SetPosition(x,y);
        m_shape.EnableOutline(false);
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

    void SetPosition(float X, float Y)
    {
        m_shape.SetPosition(X, Y);
    }

    void Draw(sf::RenderWindow * app)
    {
        app->Draw(m_shape);
    }
};

struct SpriteImpl : public GraphImpl
{
private:
    sf::Texture m_texture;
    sf::Texture m_texture_hl;
    sf::Sprite  m_sprite;
    float size_x;
    float size_y;
public:
    SpriteImpl(float x, float y, float size_x, float size_y, const std::string & texture, const std::string & texture_hl)
        : size_x(size_x), size_y(size_y)
    {
        m_texture.LoadFromFile(texture);
        m_texture_hl.LoadFromFile(texture_hl);
        m_sprite.SetTexture(m_texture);
        m_sprite.SetPosition(x - size_x/2 ,y - size_y/2);
        m_sprite.Resize(size_x, size_y);
    }

    void Highlight()
    {
        m_sprite.SetTexture(m_texture_hl);
    }

    void Unhighlight()
    {
        m_sprite.SetTexture(m_texture);
    }

    void Move(float diffX, float diffY)
    {
        m_sprite.Move(diffX, diffY);
    }

    void SetPosition(float X, float Y)
    {
        m_sprite.SetPosition(X - size_x/2 ,Y - size_y/2);
    }

    void Draw(sf::RenderWindow * app)
    {
        app->Draw(m_sprite);
    }

};

struct SSVEPStimulusImpl
{
private:
    GraphImpl * m_graph;
    int m_frequency;
    int m_screenFrequency;
    std::vector<bool> m_frameSeq;
public:
    SSVEPStimulusImpl(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & texture, const std::string & texture_hl) : 
        m_graph(new SpriteImpl(x, y, size_x, size_y, texture, texture_hl)),
        m_frequency(frequency) , m_screenFrequency(screenFrequency)
    {
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

    SSVEPStimulusImpl(int frequency, int screenFrequency, float x, float y, float radius, int r, int g, int b, int a) : 
        m_graph(new ShapeImpl(x, y, radius, r, g, b, a)),
        m_frequency(frequency) , m_screenFrequency(screenFrequency)
    {
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

    SSVEPStimulusImpl(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a) : 
        m_graph(new ShapeImpl(x, y, size_x, size_y, r, g, b, a)),
        m_frequency(frequency) , m_screenFrequency(screenFrequency)
    {
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
        delete m_graph;
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
        m_graph->Highlight();
    }

    void Unhighlight()
    {
        m_graph->Unhighlight();
    }

    void Move(float diffX, float diffY)
    {
        m_graph->Move(diffX, diffY);
    }

    void SetPosition(float X, float Y)
    {
        m_graph->SetPosition(X, Y);
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
    {
        if(m_frameSeq[frameCount % m_screenFrequency])
        {
            m_graph->Draw(app);
        }
    }

}; //class SSVEPStimulusImpl

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency, x, y, size_x, size_y, tx, tx_hl))
{
}

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float radius, int r, int g, int b, int a) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency, x, y, radius, r, g, b, a))
{
}

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

void SSVEPStimulus::SetPosition(float X, float Y)
{
    m_impl->SetPosition(X, Y);
}

void SSVEPStimulus::Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
{
    m_impl->Display(app, frameCount, clock);
}

} // namespace bciinterface

