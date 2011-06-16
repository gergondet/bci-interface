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
    }

    ~SpriteObjectImpl()
    {
        delete m_sprite;
        delete m_image;
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount)
    {
        app->Draw(*m_sprite);
    } 

    void SetSubRect(int left, int top, int right, int bottom)
    {
        m_sprite->SetSubRect(sf::IntRect(left, top, right, bottom));
    }
};

SpriteObject::SpriteObject(const std::string & image_src) : m_impl(new SpriteObjectImpl(image_src))
{}

void SpriteObject::Display(sf::RenderWindow * app, unsigned int frameCount)
{
    m_impl->Display(app, frameCount);
}

void SpriteObject::SetSubRect(int left, int top, int right, int bottom)
{
    m_impl->SetSubRect(left, top, right, bottom);
}

} // namespace bciinterface

