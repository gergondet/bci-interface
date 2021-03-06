#include "bci-interface/DisplayObject/P300Object.h"

#include <SFML/Graphics.hpp>

#ifndef WIN32
#include <sys/time.h>
#else
#include "bci-interface/Utils/Win32.h"
#endif

namespace bciinterface
{

struct P300ObjectImpl
{
private:
    unsigned int m_active_time;
    unsigned int m_active_since;
    bool m_active;
    sf::Color m_active_color;
    sf::Color m_inactive_color;
    sf::ConvexShape * m_shape;
public:
    P300ObjectImpl(unsigned int active_time, unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y, unsigned char r, unsigned char g, unsigned char b)
        : m_active_time(active_time*1000), m_active_since(0), m_active(false), 
          m_active_color(sf::Color(r,g,b,255)), m_inactive_color(sf::Color(r,g,b,60)),
          m_shape(new sf::ConvexShape())
    {
        m_shape->setPointCount(4);
        m_shape->setPoint(0, sf::Vector2f((float)(x-size_x)/2, (float)(y-size_y)/2));
        m_shape->setPoint(1, sf::Vector2f((float)(x+size_x)/2, (float)(y-size_y)/2));
        m_shape->setPoint(2, sf::Vector2f((float)(x+size_x)/2, (float)(y+size_y)/2));
        m_shape->setPoint(3, sf::Vector2f((float)(x-size_x)/2, (float)(y+size_y)/2));
        m_shape->setFillColor(m_inactive_color);
    }

    ~P300ObjectImpl()
    {
        delete m_shape;
    }

    void Display(sf::RenderTarget * app, unsigned int, sf::Clock &)
    {
        app->draw(*m_shape);
        if(m_active) 
        { 
            timeval tv_now;
            gettimeofday(&tv_now, 0);
            unsigned int now = tv_now.tv_sec*1000000 + tv_now.tv_usec;
            if(now - m_active_since > m_active_time)
            {
                Unhighlight(); 
            }
        } 
    }

    void Highlight()
    {
        m_shape->setFillColor(m_active_color);
        m_active = true;
        timeval tv_now;
        gettimeofday(&tv_now, 0);
        m_active_since = tv_now.tv_sec*1000000 + tv_now.tv_usec;
    }

    void Unhighlight()
    {
        m_shape->setFillColor(m_inactive_color);
        m_active = false;
    }
};

P300Object::P300Object(unsigned int active_time, unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y, unsigned char r, unsigned char g, unsigned char b)
    : m_impl(new P300ObjectImpl(active_time, x, y, size_x, size_y, r, g, b))
{}

void P300Object::Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock)
{
    m_impl->Display(app, frameCount, clock);
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

