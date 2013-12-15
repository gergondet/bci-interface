#include "bci-interface/DisplayObject/SSVEPStimulus.h"

#include "bci-interface/Utils/square-function.h"
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
    virtual void SetRotation(float deg_angle) = 0;
    virtual void SetScale(float sX, float sY) = 0;
    virtual void Resize(float size_x, float size_y) {}
    virtual void Draw(sf::RenderWindow * app) = 0;
    virtual void DrawInactive(sf::RenderWindow * app) {}
};

struct ShapeImpl : public GraphImpl
{
private:
    sf::Shape * m_shape;
    float size_x;
    float size_y;
public:
    ShapeImpl(float x, float y, float size_x, float size_y, int r, int g, int b, int a) : 
        m_shape(new sf::RectangleShape(sf::Vector2f(size_x, size_y)))
        , size_x(size_x), size_y(size_y)
    {
        m_shape->setFillColor(sf::Color(r,g,b,a));
        m_shape->setOutlineColor(sf::Color(0,255,0,128));
        m_shape->setOutlineThickness(0);
        m_shape->setOrigin(size_x/2, size_y/2);
        m_shape->setPosition(x,y);
    }

    ShapeImpl(float x, float y, float radius, int r, int g, int b, int a) : 
        m_shape(new sf::CircleShape(radius)) 
        , size_x(2*radius), size_y(2*radius)
    {
        m_shape->setFillColor(sf::Color(r,g,b,a));
        m_shape->setOutlineColor(sf::Color(0,255,0,128));
        m_shape->setOutlineThickness(0);
        m_shape->setOrigin(radius, radius);
        m_shape->setPosition(x,y);
    }

    ~ShapeImpl()
    {
        delete m_shape;
    }

    void Highlight()
    {
        m_shape->setOutlineThickness(6);
    }

    void Unhighlight()
    {
        m_shape->setOutlineThickness(0);
    }

    void Move(float diffX, float diffY)
    {
        m_shape->move(diffX, diffY);
    }

    void SetPosition(float X, float Y)
    {
        m_shape->setPosition(X, Y);
    }

    void SetRotation(float deg_angle)
    {
        m_shape->setRotation(deg_angle);
    }

    void SetScale(float sX, float sY)
    {
        m_shape->setScale(sX, sY);
    }

    void Resize(float sx, float sy)
    {
        SetScale(sx/size_x, sy/size_y);
    }

    void Draw(sf::RenderWindow * app)
    {
        app->draw(*m_shape);
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
    SpriteImpl(float x, float y, const std::string & texture, const std::string & texture_hl, float size_x_i = 0, float size_y_i = 0)
    {
        m_texture.loadFromFile(texture);
        m_texture_hl.loadFromFile(texture_hl);
        m_sprite.setTexture(m_texture);
        size_x = m_texture.getSize().x;
        size_y = m_texture.getSize().y;
        m_sprite.setOrigin(m_texture.getSize().x/2, m_texture.getSize().y/2);
        m_sprite.setPosition(x,y);
        if(size_x_i == 0) { size_x_i = size_x; }
        if(size_y_i == 0) { size_y_i = size_y; }
        m_sprite.setScale((float)size_x_i/(float)m_texture.getSize().x, (float)size_y_i/(float)m_texture.getSize().y);
    }

    void Highlight()
    {
        m_sprite.setTexture(m_texture_hl);
    }

    void Unhighlight()
    {
        m_sprite.setTexture(m_texture);
    }

    void Move(float diffX, float diffY)
    {
        m_sprite.move(diffX, diffY);
    }

    void SetPosition(float X, float Y)
    {
        m_sprite.setPosition(X, Y);
    }

    void SetRotation(float deg_angle)
    {
        m_sprite.setRotation(deg_angle);
    }

    void SetScale(float sX, float sY)
    {
        m_sprite.setScale(sX, sY);
    }

    void Resize(float sx, float sy)
    {
        SetScale(sx/size_x, sy/size_y);
    }

    void Draw(sf::RenderWindow * app)
    {
        app->draw(m_sprite);
    }

};

struct SpriteWithInactiveImpl : public GraphImpl
{
private:
    sf::Texture m_texture;
    sf::Texture m_texture_hl;
    sf::Texture m_texture_inactive;
    sf::Texture m_texture_inactive_hl;
    sf::Sprite  m_sprite;
    sf::Sprite  m_sprite_inactive;
    float size_x;
    float size_y;
public:
    SpriteWithInactiveImpl(float x, float y, const std::string & texture, const std::string & texture_hl, const std::string & texture_inactive, const std::string & texture_inactive_hl, float size_x_i = 0, float size_y_i = 0)
    {
        m_texture.loadFromFile(texture);
        m_texture_hl.loadFromFile(texture_hl);
        m_texture_inactive.loadFromFile(texture_inactive);
        m_texture_inactive_hl.loadFromFile(texture_inactive_hl);
        m_sprite.setTexture(m_texture);
        m_sprite_inactive.setTexture(m_texture_inactive);
        size_x = m_texture.getSize().x;
        size_y = m_texture.getSize().y;
        m_sprite.setOrigin(m_texture.getSize().x/2, m_texture.getSize().y/2);
        m_sprite.setPosition(x,y);
        m_sprite_inactive.setOrigin(m_texture.getSize().x/2, m_texture.getSize().y/2);
        m_sprite_inactive.setPosition(x,y);
        if(size_x_i == 0) { size_x_i = size_x; }
        if(size_y_i == 0) { size_y_i = size_y; }
        m_sprite.setScale((float)size_x_i/(float)m_texture.getSize().x, (float)size_y_i/(float)m_texture.getSize().y);
        m_sprite_inactive.setScale((float)size_x_i/(float)m_texture.getSize().x, (float)size_y_i/(float)m_texture.getSize().y);
    }

    void Highlight()
    {
        m_sprite.setTexture(m_texture_hl);
        m_sprite_inactive.setTexture(m_texture_inactive_hl);
    }

    void Unhighlight()
    {
        m_sprite.setTexture(m_texture);
        m_sprite_inactive.setTexture(m_texture_inactive);
    }

    void Move(float diffX, float diffY)
    {
        m_sprite.move(diffX, diffY);
        m_sprite_inactive.move(diffX, diffY);
    }

    void SetPosition(float X, float Y)
    {
        m_sprite.setPosition(X, Y);
        m_sprite_inactive.setPosition(X, Y);
    }

    void SetRotation(float deg_angle)
    {
        m_sprite.setRotation(deg_angle);
        m_sprite_inactive.setRotation(deg_angle);
    }

    void SetScale(float sX, float sY)
    {
        m_sprite.setScale(sX, sY);
        m_sprite_inactive.setScale(sX, sY);
    }

    void Resize(float sx, float sy)
    {
        SetScale(sx/size_x, sy/size_y);
    }

    void Draw(sf::RenderWindow * app)
    {
        app->draw(m_sprite);
    }

    void DrawInactive(sf::RenderWindow * app)
    {
        app->draw(m_sprite_inactive);
    }

};

struct SSVEPStimulusImpl
{
private:
    GraphImpl * m_graph;
    bool m_gl;
    int m_frequency;
    int m_screenFrequency;
    std::vector<bool> m_frameSeq;
public:
    SSVEPStimulusImpl(int frequency, int screenFrequency, float x, float y, const std::string & texture, const std::string & texture_hl, float size_x = 0, float size_y = 0) : 
        m_graph(new SpriteImpl(x, y, texture, texture_hl, size_x, size_y)),
        m_gl(false),
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

    SSVEPStimulusImpl(int frequency, int screenFrequency, float x, float y, const std::string & texture, const std::string & texture_hl, const std::string & texture_inactive, const std::string & texture_inactive_hl, float size_x = 0, float size_y = 0) : 
        m_graph(new SpriteWithInactiveImpl(x, y, texture, texture_hl, texture_inactive, texture_inactive_hl, size_x, size_y)),
        m_gl(false),
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
        m_gl(false),
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
        m_gl(false),
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

    SSVEPStimulusImpl(int frequency, int screenFrequency) : 
        m_graph(0),
        m_gl(true),
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

    int GetFrequency()
    {
        return m_frequency;
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
        if(m_graph)
        {
            m_graph->Highlight();
        }
    }

    void Unhighlight()
    {
        if(m_graph)
        {
            m_graph->Unhighlight();
        }
    }

    void Move(float diffX, float diffY)
    {
        if(m_graph)
        {
            m_graph->Move(diffX, diffY);
        }
    }

    void SetPosition(float X, float Y)
    {
        if(m_graph)
        {
            m_graph->SetPosition(X, Y);
        }
    }

    void SetRotation(float deg_angle)
    {
        if(m_graph)
        {
            m_graph->SetRotation(deg_angle);
        }
    }

    void SetScale(float sX, float sY)
    {
        if(m_graph)
        {
            m_graph->SetScale(sX, sY);
        }
    }

    void Resize(float sx, float sy)
    {
        if(m_graph)
        {
            m_graph->Resize(sx, sy);
        }
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
    {
        if(m_graph && m_frameSeq[frameCount % m_screenFrequency])
        {
            m_graph->Draw(app);
        }
        else if(m_graph)
        {
            m_graph->DrawInactive(app);
        }
    }

    bool DrawWithGL()
    {
        return m_gl;
    }

    bool DisplayActive(unsigned int frameCount)
    {
        return m_frameSeq[frameCount % m_screenFrequency];
    }

}; //class SSVEPStimulusImpl

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency, x, y, tx, tx_hl, size_x, size_y))
{
}

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl, const std::string & tx2, const std::string & tx2_hl) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency, x, y, tx, tx_hl, tx2, tx2_hl, size_x, size_y))
{
}

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency, float x, float y, const std::string & tx, const std::string & tx_hl) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency, x, y, tx, tx_hl))
{
}

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency, float x, float y, const std::string & tx, const std::string & tx_hl, const std::string & tx2, const std::string & tx2_hl) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency, x, y, tx, tx_hl, tx2, tx2_hl))
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

SSVEPStimulus::SSVEPStimulus(int frequency, int screenFrequency) :
    m_impl(new SSVEPStimulusImpl(frequency, screenFrequency))
{
}

int SSVEPStimulus::GetFrequency()
{
    return m_impl->GetFrequency();
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

void SSVEPStimulus::SetRotation(float deg_angle)
{
    m_impl->SetRotation(deg_angle);
}

void SSVEPStimulus::SetScale(float sX, float sY)
{
    m_impl->SetScale(sX, sY);
}

void SSVEPStimulus::Resize(float size_x, float size_y)
{
    m_impl->Resize(size_x, size_y);
}

void SSVEPStimulus::Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
{
    m_impl->Display(app, frameCount, clock);
}

bool SSVEPStimulus::DrawWithGL()
{
    return m_impl->DrawWithGL();
}

bool SSVEPStimulus::DisplayActive(unsigned int frameCount)
{
    return m_impl->DisplayActive(frameCount);
}

} // namespace bciinterface

