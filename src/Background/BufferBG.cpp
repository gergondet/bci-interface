#include "bci-interface/Background/BufferBG.h"

#include <SFML/Graphics.hpp>

#include <boost/thread/mutex.hpp>

#include <iostream>
#include <cstring>

namespace bciinterface
{

struct BufferBGImpl
{
private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_curr_width;
    unsigned int m_curr_height;
    unsigned int m_wwidth;
    unsigned int m_wheight;
    unsigned int m_iwidth;
    unsigned int m_iheight;
    sf::IntRect m_subrect;
    float m_scale_x;
    float m_scale_y;

    sf::Uint8 * m_dataImage;
    sf::Texture * m_texture_display;
    sf::Sprite * m_sprite_display;

    bool m_color_enabled;

    boost::mutex bg_mutex;

public:
    BufferBGImpl(unsigned int width, unsigned int height,
                    unsigned int wwidth, unsigned wheight, unsigned iwidth, unsigned iheight)
       :    m_width(width), m_height(height),
            m_curr_width(width), m_curr_height(height),
            m_wwidth(wwidth), m_wheight(wheight), m_iwidth(iwidth), m_iheight(iheight),
            m_subrect(0, 0, width, height),
            m_dataImage(new sf::Uint8[width*height*4]),
            m_texture_display(new sf::Texture),
            m_sprite_display(new sf::Sprite),
            m_color_enabled(true)
    {
        m_scale_x = (float)m_iwidth/(float)m_width;
        m_scale_y = (float)m_iheight/(float)m_height;
        memset(m_dataImage, 0, width*height*4);
        m_texture_display->create(width, height);
        m_sprite_display->setTexture(*m_texture_display);
        if(iwidth == 0 || iheight == 0)
        {
            m_iwidth = m_wwidth;
            m_iheight = m_wheight;
        }
        m_sprite_display->setScale(m_scale_x, m_scale_y);
        m_sprite_display->setPosition((float)(m_wwidth/2 - m_iwidth/2), (float)(m_wheight/2 - m_iheight/2));
        m_sprite_display->setTextureRect(m_subrect);
    }

    ~BufferBGImpl()
    {
        delete m_sprite_display;
        delete m_texture_display;
        delete[] m_dataImage;
    }

    void UpdateLoop()
    {
    }

    void Close()
    {
    }

    void Draw(sf::RenderWindow * app)
    {
        boost::mutex::scoped_lock lock(bg_mutex);
        app->draw(*m_sprite_display);
    }

    void LoadImageFromPixels()
    {
        boost::mutex::scoped_lock lock(bg_mutex);
        m_texture_display->update(m_dataImage, m_width, m_height, 0, 0);
    }

    void UpdateFromBuffer_MONO(unsigned char * img)
    {
        for(size_t i = 0; i < m_width*m_height; ++i)
        {
            m_dataImage[4*i] = img[i];
            m_dataImage[4*i+1] = img[i];
            m_dataImage[4*i+2] = img[i];
            m_dataImage[4*i+3] = 255;
        }
        LoadImageFromPixels();
    }

    void UpdateFromBuffer_RGB(unsigned char * img)
    {
        if(m_color_enabled)
        {
            for(size_t i = 0; i < m_width*m_height; ++i)
            {
                m_dataImage[4*i] = img[4*i];
                m_dataImage[4*i+1] = img[4*i+1];
                m_dataImage[4*i+2] = img[4*i+2];
                m_dataImage[4*i+3] = 255;
            }
        }    
        else
        {
            sf::Uint32 gray;
            for(size_t i = 0; i < m_width*m_height; ++i)
            {
                gray = (img[4*i] + img[4*i+1] + img[4*i+2])/3;
                m_dataImage[4*i]   = (sf::Uint8)gray;
                m_dataImage[4*i+1] = (sf::Uint8)gray;
                m_dataImage[4*i+2] = (sf::Uint8)gray;
                m_dataImage[4*i+3] = 255;
            }
        }    
        LoadImageFromPixels();
    }

    void UpdateFromBuffer_BGR8(unsigned char * img)
    {
        for(size_t i = 0; i < m_width*m_height; ++i)
        {
            m_dataImage[4*i] = img[3*i+2];
            m_dataImage[4*i+1] = img[3*i+1];
            m_dataImage[4*i+2] = img[3*i];
            m_dataImage[4*i+3] = 255;
        }
        LoadImageFromPixels();
    }

    void SwitchColorMode()
    {
        m_color_enabled = !m_color_enabled;
    }

    void SetSubRect(int left, int top, int width, int height)
    {
        m_subrect = sf::IntRect(left, top, width, height);
        m_scale_x = m_scale_x*(float)(m_curr_width)/(float)width;
        m_curr_width = width;
        m_scale_y = m_scale_y*(float)m_curr_height/(float)height;
        m_curr_height = height;
    }

};

BufferBG::BufferBG(unsigned int width, unsigned int height,
    unsigned int wwidth, unsigned int wheight, unsigned int iwidth, unsigned int iheight)
    : m_impl(new BufferBGImpl(width, height, wwidth, wheight, iwidth, iheight))
{
}

void BufferBG::UpdateLoop()
{
    m_impl->UpdateLoop();
}

void BufferBG::Close()
{
    m_impl->Close();
}

void BufferBG::Draw(sf::RenderWindow * app)
{
    return m_impl->Draw(app);
}

void BufferBG::UpdateFromBuffer_MONO(unsigned char * img)
{
    m_impl->UpdateFromBuffer_MONO(img);
}

void BufferBG::UpdateFromBuffer_RGB(unsigned char * img)
{
    m_impl->UpdateFromBuffer_RGB(img);
}

void BufferBG::UpdateFromBuffer_BGR8(unsigned char * img)
{
    m_impl->UpdateFromBuffer_BGR8(img);
}

void BufferBG::SwitchColorMode()
{
    m_impl->SwitchColorMode();
}

void BufferBG::SetSubRect(int left, int top, int width, int height)
{
    m_impl->SetSubRect(left, top, width, height);
}

} // namespace bciinterface

