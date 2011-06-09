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
    sf::Uint8 * m_dataImage;
    sf::Image * m_image;
    sf::Sprite * m_sprite;

    bool m_close;

public:
    VisionServerBGImpl(const std::string & vision_name, unsigned short vision_port, unsigned int width, unsigned int height)
       :    m_width(width), m_height(height),
            m_dataFromSocket(new unsigned char[50001]), m_dataImage(new sf::Uint8[width*height*4]),
            m_image(new sf::Image), m_sprite(new sf::Sprite), m_close(false)
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
        memset(m_dataImage, 0, width*height*4);
        m_image->LoadFromPixels(width, height, m_dataImage);
        m_sprite->SetImage(*m_image);

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
        delete[] m_dataImage;
        delete m_image;
        delete m_sprite;
        close(m_sockfd);
    }

    void UpdateLoop()
    {
        while(!m_close)
        {
            sendto(m_sockfd, "get", 4, 0, (struct sockaddr *)&m_serveraddr, sizeof(m_serveraddr));
            fd_set recvset;

            int receivedData = 0;
            unsigned char packetId = '\0';
            memset(m_dataImage, 0, m_width*m_height*4);
            while(receivedData < m_width*m_height && receivedData != -1)
            {
                FD_ZERO(&recvset);
                FD_SET(m_sockfd, &recvset);

                struct timeval tv;
                tv.tv_sec = 0;
                tv.tv_usec = 100000; /* 100 ms wait */
                int ret = select(m_sockfd + 1, &recvset, 0, 0, &tv);
                if(ret > 0)
                {
                    memset(m_dataFromSocket, '\0', 50001);
                    int n = recvfrom(m_sockfd, m_dataFromSocket, 50001, 0, 0, 0);
                    unsigned char newPacketId = m_dataFromSocket[0];
                    if(newPacketId != packetId && newPacketId != packetId + 1)
                    {
                        /* Lost part of the frame on the road, reset */
                        receivedData = -1;
                    }
                    else
                    {
                        packetId = newPacketId;
                        receivedData += n;
                        /* Copy new data in m_dataImage */
                        for(int i = 0; i < n - 1; ++i)
                        {
                            m_dataImage[4*50000*packetId + 4*i]   = m_dataFromSocket[i+1];
                            m_dataImage[4*50000*packetId + 4*i+1] = m_dataFromSocket[i+1];
                            m_dataImage[4*50000*packetId + 4*i+2] = m_dataFromSocket[i+1];
                            m_dataImage[4*50000*packetId + 4*i+3] = 255;
                        }
                    }
                }
                else
                {
                    receivedData = -1;
                }
            }
            if(receivedData != -1)
            {
                /* m_dataImage has a full image worth of data, update the sprite */
                m_image->LoadFromPixels(m_width, m_height, m_dataImage);
                m_sprite->SetImage(*m_image);
            }
        }
    }

    void Close()
    {
        m_close = true;
    }

    void Draw(sf::RenderWindow * app)
    {
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

