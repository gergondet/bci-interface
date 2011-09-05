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
        m_sprite->SetPosition(0,0);
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

    void SetSubRect(int left, int top, int right, int bottom)
    {
        m_sprite->SetSubRect(sf::IntRect(left, top, right, bottom));
    }

    void SetPosition(float x, float y)
    {
        m_sprite->SetPosition(x, y);
    }
};

SpriteObject::SpriteObject(const std::string & texture_src) : m_impl(new SpriteObjectImpl(texture_src))
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

