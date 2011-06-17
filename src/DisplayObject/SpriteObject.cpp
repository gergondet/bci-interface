#include <bci-interface/DisplayObject/SpriteObject.h>

#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct SpriteObjectImpl
{
private:
    sf::Image * m_image;
    sf::Sprite * m_sprite;
public:
    SpriteObjectImpl(const std::string & image_src)
        : m_image(0), m_sprite(0)
    {
        m_image = new sf::Image();
        if(! m_image->LoadFromFile(image_src) )
        {
            std::cerr << "Could not load image from: " << image_src << std::endl;
        }
        m_sprite = new sf::Sprite(*m_image);
        m_sprite->SetPosition(0,0);
    }

    ~SpriteObjectImpl()
    {
        delete m_sprite;
        delete m_image;
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
    {
        app->Draw(*m_sprite);
    } 

    void SetSubRect(int left, int top, int right, int bottom)
    {
        m_sprite->SetSubRect(sf::IntRect(left, top, right, bottom));
    }

    void SetPosition(float x, float y)
    {
        m_sprite->SetPosition(x, y);
    }
};

SpriteObject::SpriteObject(const std::string & image_src) : m_impl(new SpriteObjectImpl(image_src))
{}

void SpriteObject::Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
{
    m_impl->Display(app, frameCount, clock);
}

void SpriteObject::SetSubRect(int left, int top, int right, int bottom)
{
    m_impl->SetSubRect(left, top, right, bottom);
}

void SpriteObject::SetPosition(float x, float y)
{
    m_impl->SetPosition(x, y);
}

} // namespace bciinterface

