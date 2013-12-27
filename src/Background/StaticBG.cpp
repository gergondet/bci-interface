#include "bci-interface/Background/StaticBG.h"

#include <iostream>
#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct StaticBGImpl
{
private:
    sf::Texture * m_texture;
    sf::Sprite * m_sprite;
    unsigned int m_wwidth;
    unsigned int m_wheight;
    unsigned int m_iwidth;
    unsigned int m_iheight;
public:
    StaticBGImpl(const std::string & src_file, 
        unsigned int wwidth, unsigned int wheight, unsigned int iwidth, unsigned int iheight)
    : m_texture(new sf::Texture), m_sprite(0),
      m_wwidth(wwidth), m_wheight(wheight), m_iwidth(iwidth), m_iheight(iheight)
    {
        if(!m_texture->loadFromFile(src_file))
        {
            std::cerr << "StaticBG, could not load texture from: " << src_file << std::endl;
        }
        m_sprite = new sf::Sprite(*m_texture);
        if(iwidth == 0 || iheight == 0)
        {
            m_iwidth  = m_wwidth;
            m_iheight = m_wheight;
        }
        m_sprite->setScale((float)m_iwidth/(float)m_wwidth, (float)m_iheight/(float)m_wheight);
        m_sprite->setPosition((float)(m_wwidth/2 - m_iwidth/2), (float)(m_wheight/2 - m_iheight/2));
    }

    ~StaticBGImpl()
    {
        delete m_texture;
        delete m_sprite;
    }

    void UpdateLoop()
    {
    }

    void Close()
    {
    }

    void Draw(sf::RenderWindow * app)
    {
        app->draw(*m_sprite);
    }

    void ChangeImage(const std::string & new_src)
    {
        sf::Texture * new_texture = new sf::Texture();
        if(!new_texture->loadFromFile(new_src))
        {
            std::cerr << "StaticBG::ChangeImage, cannot load texture from: " << new_src << std::endl;
            delete new_texture;
            return;
        }
        sf::Sprite * new_sprite  = new sf::Sprite(*new_texture);
        new_sprite->setScale((float)m_iwidth/(float)m_wwidth, (float)m_iheight/(float)m_wheight);
        new_sprite->setPosition((float)(m_wwidth/2 - m_iwidth/2), (float)(m_wheight/2 - m_iheight/2));

        sf::Texture * old_texture = m_texture;
        sf::Sprite * old_sprite = m_sprite;
        m_sprite = new_sprite;
        m_texture = new_texture;
        delete old_sprite;
        delete old_texture;
    }
};

StaticBG::StaticBG(const std::string & src_file, 
    unsigned int wwidth, unsigned int wheight, unsigned int iwidth, unsigned int iheight)
: m_impl(new StaticBGImpl(src_file, wwidth, wheight, iwidth, iheight))
{}

void StaticBG::UpdateLoop()
{
    m_impl->UpdateLoop();
}

void StaticBG::Close()
{
    m_impl->Close();
}

void StaticBG::Draw(sf::RenderWindow * app)
{
    m_impl->Draw(app);
}

void StaticBG::ChangeImage(const std::string & new_src)
{
    m_impl->ChangeImage(new_src);
}

} //namespace bciinterface

