#include <bci-interface/Background/VisionServerBG.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <cerrno>
#include <cstring>

namespace bciinterface
{

struct VisionServerBGImpl
{
private:
    unsigned int m_width;
    unsigned int m_height;

    int m_sockfd;
    struct sockaddr_in m_bindaddr;
    struct sockaddr_in m_serveraddr;
    unsigned char * m_dataFromSocket;
    sf::Uint8 * m_datatexture;
    sf::Texture * m_texture;
    sf::Sprite * m_sprite;

    bool m_close;

public:
    VisionServerBGImpl(const std::string & vision_name, unsigned short vision_port, unsigned int width, unsigned int height)
       :    m_width(width), m_height(height),
            m_dataFromSocket(new unsigned char[16385]), m_datatexture(new sf::Uint8[width*height*4]),
            m_texture(new sf::Texture), m_sprite(new sf::Sprite), m_close(false)
    {
        struct hostent * hent;
        hent = gethostbyname(vision_name.c_str());
        if(hent)
        {
            m_serveraddr.sin_family = AF_INET;
            memcpy(&m_serveraddr.sin_addr, hent->h_addr_list[0], hent->h_length);
            m_serveraddr.sin_port = htons(vision_port);
        }
        else
        {
            std::cerr << strerror(errno) << std::endl;
        }
        memset(m_datatexture, 0, width*height*4);
        m_texture->Create(width, height);
        m_texture->Update(m_datatexture);
        m_sprite->SetTexture(*m_texture);

        m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(m_sockfd < 0)
        {
                std::cerr << strerror(m_sockfd) << std::endl;
        }

        m_bindaddr.sin_family = AF_INET;
        m_bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        m_bindaddr.sin_port = htons(0);
        int err = bind(m_sockfd, (struct sockaddr *)&m_bindaddr, sizeof(m_bindaddr));
        if(err < 0)
        {
                std::cerr << strerror(err) << std::endl;
                close(m_sockfd);
        }
    }

    ~VisionServerBGImpl()
    {
        delete[] m_dataFromSocket;
        delete[] m_datatexture;
        delete m_texture;
        delete m_sprite;
        close(m_sockfd);
    }

    void UpdateLoop()
    {
        while(!m_close)
        {
            sendto(m_sockfd, "get", 4, 0, (struct sockaddr *)&m_serveraddr, sizeof(m_serveraddr));

            int receivedData = 0;
            char packetId = -1;
            memset(m_datatexture, 0, m_width*m_height*4);
            while(receivedData < m_width*m_height*4 && receivedData != -1)
            {
                memset(m_dataFromSocket, '\0', 16385);
                int n = recvfrom(m_sockfd, m_dataFromSocket, 16385, 0, 0, 0);
                if(n <= 0)
                {
                    receivedData = -1;
                    break;
                }
                unsigned char newPacketId = m_dataFromSocket[0];
                if(newPacketId != packetId + 1)
                {
                    /* Lost part of the frame on the road, reset */
                    receivedData = -1;
                }
                else
                {
                    packetId = newPacketId;
                    receivedData += n;
                    /* Copy new data in m_datatexture */
                    for(int i = 0; i < n - 1; ++i)
                    {
                        m_datatexture[16384*packetId + i] = m_dataFromSocket[i+1];
                        if( (16384*packetId + i) % 4  == 3 ) { m_datatexture[16384*packetId + i] = 255; }
                    }
                }
                if(receivedData < m_width*m_height*4 && receivedData != -1)
                {
                    sendto(m_sockfd, "more", 5, 0, (struct sockaddr *)&m_serveraddr, sizeof(m_serveraddr));
                }
            }
            usleep(50000);
            if(receivedData != -1)
            {
                /* m_datatexture has a full texture worth of data, update the sprite */
                m_texture->Update(m_datatexture);
            }
        }
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
};

VisionServerBG::VisionServerBG(const std::string & vs_name, unsigned short vs_port, unsigned int width, unsigned int height)
    : m_impl(new VisionServerBGImpl(vs_name, vs_port, width, height))
{
}

void VisionServerBG::UpdateLoop()
{
    m_impl->UpdateLoop();
}

void VisionServerBG::Close()
{
    m_impl->Close();
}

void VisionServerBG::Draw(sf::RenderWindow * app)
{
    return m_impl->Draw(app);
}

} // namespace bciinterface

