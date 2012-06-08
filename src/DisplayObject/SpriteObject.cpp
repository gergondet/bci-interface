#include <bci-interface/DisplayObject/SpriteObject.h>

#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct SpriteObjectImpl
{
private:
    sf::Texture * m_texture;
    sf::Sprite * m_sprite;
    float m_x;
    float m_y;
    float m_size_x;
    float m_size_y;
public:
    SpriteObjectImpl(const std::string & texture_src)
        : m_texture(0), m_sprite(0)
    {
        m_texture = new sf::Texture();
        if(! m_texture->LoadFromFile(texture_src) )
        {
            std::cerr << "Could not load texture from: " << texture_src << std::endl;
        }
        m_sprite = new sf::Sprite(*m_texture);
        m_sprite->setPosition(0,0);
        m_x = 0;
        m_y = 0;
        m_size_x = m_texture->getSize().x;
        m_size_y = m_texture->getSize().y;
    }

    ~SpriteObjectImpl()
    {
        delete m_sprite;
        delete m_texture;
    }

    void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
    {
        app->Draw(*m_sprite);
    } 

    void SetSubRect(int left, int top, int width, int height)
    {
        m_sprite->SetSubRect(sf::IntRect(left, top, width, height));
    }

    void SetPosition(float x, float y)
    {
        m_sprite->setPosition(x - m_size_x/2, y - m_size_y/2);
        m_x = x;
        m_y = y;
    }

    void Resize(float size_x, float size_y)
    {
        m_sprite->setScale((float)size_x/(float)m_texture->getSize().x, (float)size_y/(float)m_texture->getSize().y);
        m_size_x = size_x;
        m_size_y = size_y;
        SetPosition(m_x, m_y);
    }
};

SpriteObject::SpriteObject(const std::string & texture_src) : m_impl(new SpriteObjectImpl(texture_src))
{}

void SpriteObject::Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
{
    m_impl->Display(app, frameCount, clock);
}

void SpriteObject::SetSubRect(int left, int top, int width, int height)
{
    m_impl->SetSubRect(left, top, width, height);
}

void SpriteObject::SetPosition(float x, float y)
{
    m_impl->SetPosition(x, y);
}

void SpriteObject::Resize(float x, float y)
{
    m_impl->Resize(x,y);
}

} // namespace bciinterface

