#include <bci-interface/Background/BufferBG.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cstring>

namespace bciinterface
{

struct BufferBGImpl
{
private:
    unsigned int m_width;
    unsigned int m_height;

    sf::Uint8 * m_dataImage;
    sf::Texture * m_texture_display;
    sf::Texture * m_texture_load;
    sf::Sprite * m_sprite_display;
    sf::Sprite * m_sprite_load;

    bool m_color_enabled;

    bool m_close;

public:
    BufferBGImpl(unsigned int width, unsigned int height)
       :    m_width(width), m_height(height),
            m_dataImage(new sf::Uint8[width*height*4]),
            m_texture_display(new sf::Texture), m_texture_load(new sf::Texture),
            m_sprite_display(new sf::Sprite), m_sprite_load(new sf::Sprite),
            m_color_enabled(true), m_close(false)
    {
        memset(m_dataImage, 0, width*height*4);
        m_texture_display->Create(width, height);
        m_texture_load->Create(width, height);
        m_sprite_display->SetTexture(*m_texture_display);
        m_sprite_load->SetTexture(*m_texture_load);
    }

    ~BufferBGImpl()
    {
        delete m_sprite_display;
        delete m_sprite_load;
        delete m_texture_display;
        delete m_texture_load;
        delete[] m_dataImage;
    }

    void UpdateLoop()
    {
    }

    void Close()
    {
        m_close = true;
    }

    void Draw(sf::RenderWindow * app)
    {
        m_sprite_display->Resize(app->GetWidth(), app->GetHeight());
        app->Draw(*m_sprite_display);
    }

    void LoadImageFromPixels()
    {
        m_texture_load->Update(m_dataImage, m_width, m_height, 0, 0);
        sf::Sprite * sprite_tmp = m_sprite_display;
        sf::Texture * texture_tmp = m_texture_display;
        m_sprite_display = m_sprite_load;
        m_texture_display = m_texture_load;
        m_sprite_load = sprite_tmp;
        m_texture_load = texture_tmp;
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
                m_dataImage[4*i]   = gray;
                m_dataImage[4*i+1] = gray;
                m_dataImage[4*i+2] = gray;
                m_dataImage[4*i+3] = 255;
            }
        }    
        LoadImageFromPixels();
    }

    void SwitchColorMode()
    {
        m_color_enabled = !m_color_enabled;
    }
};

BufferBG::BufferBG(unsigned int width, unsigned int height)
    : m_impl(new BufferBGImpl(width, height))
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

void BufferBG::SwitchColorMode()
{
    m_impl->SwitchColorMode();
}

} // namespace bciinterface

