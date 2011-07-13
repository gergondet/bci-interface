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
    sf::Image * m_image;
    sf::Sprite * m_sprite;

    bool m_close;

public:
    BufferBGImpl(unsigned int width, unsigned int height)
       :    m_width(width), m_height(height),
            m_dataImage(new sf::Uint8[width*height*4]),
            m_image(new sf::Image), m_sprite(new sf::Sprite), m_close(false)
    {
        memset(m_dataImage, 0, width*height*4);
        m_image->LoadFromPixels(width, height, m_dataImage);
        m_sprite->SetImage(*m_image);
    }

    ~BufferBGImpl()
    {
        delete[] m_dataImage;
        delete m_image;
        delete m_sprite;
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
        m_sprite->Resize(app->GetWidth(), app->GetHeight());
        app->Draw(*m_sprite);
    }

    void LoadImageFromPixels()
    {
        m_image->LoadFromPixels(m_width, m_height, m_dataImage);
        m_sprite->SetImage(*m_image);
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
        for(size_t i = 0; i < m_width*m_height; ++i)
        {
            m_dataImage[4*i] = img[4*i];
            m_dataImage[4*i+1] = img[4*i+1];
            m_dataImage[4*i+2] = img[4*i+2];
            m_dataImage[4*i+3] = 255;
        }
        LoadImageFromPixels();
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

} // namespace bciinterface

