#include <bci-interface/DisplayObject/P300Object.h>

#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct P300ObjectImpl
{
private:
    sf::Color m_active_color;
    sf::Color m_inactive_color;
    sf::Shape * m_shape;
public:
    P300ObjectImpl(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y, unsigned char r, unsigned char g, unsigned char b)
        : m_active_color(sf::Color(r,g,b,255)), m_inactive_color(sf::Color(r,g,b,60)),
          m_shape(new sf::Shape())
    {
        m_shape->AddPoint(x-size_x/2, y-size_y/2, m_inactive_color);
        m_shape->AddPoint(x+size_x/2, y-size_y/2, m_inactive_color);
        m_shape->AddPoint(x+size_x/2, y+size_y/2, m_inactive_color);
        m_shape->AddPoint(x-size_x/2, y+size_y/2, m_inactive_color);
    }

    ~P300ObjectImpl()
    {
        delete m_shape;
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount)
    {
        app->Draw(*m_shape);
    }

    void Highlight()
    {
        m_shape->SetColor(m_active_color);
    }

    void Unhighlight()
    {
        m_shape->SetColor(m_inactive_color);
    }
};

P300Object::P300Object(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y, unsigned char r, unsigned char g, unsigned char b)
    : m_impl(new P300ObjectImpl(x, y, size_x, size_y, r, g, b))
{}

void P300Object::Display(sf::RenderWindow * app, unsigned int frameCount)
{
    m_impl->Display(app, frameCount);
}

void P300Object::Highlight()
{
    m_impl->Highlight();
}

void P300Object::Unhighlight()
{
    m_impl->Unhighlight();
}

} // namespace bciinterface

