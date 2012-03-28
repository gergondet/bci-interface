#include <bci-interface/Background/VisionServerBG.h>

#include <SFML/Graphics.hpp>

#include <iostream>

#ifndef WIN32
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <cerrno>
#include <cstring>
#else

#include <windows.h>
#include <winsock.h>

#endif

#ifndef WIN32
#include <zlib.h>
#endif

void unpack(unsigned char * data_in, unsigned int data_in_size, unsigned char * data_out, unsigned int data_out_size)
{
#ifndef WIN32
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    int ret = inflateInit(&strm);

    strm.avail_in = data_in_size;
    strm.next_in = data_in;
    strm.avail_out = data_out_size;
    strm.next_out = data_out;

    ret = inflate(&strm, Z_NO_FLUSH);

    inflateEnd(&strm);
#endif
}

namespace bciinterface
{

struct VisionServerBGImpl
{
private:
    bool m_rcv_compressed_data;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_wwidth;
    unsigned int m_wheight;
    unsigned int m_iwidth;
    unsigned int m_iheight;

#ifndef WIN32
    int m_sockfd;
#else
	SOCKET m_sockfd;
#endif
    struct sockaddr_in m_bindaddr;
    struct sockaddr_in m_serveraddr;
    unsigned char * m_dataFromSocket;
    sf::Uint8 * m_compressed_data;
    sf::Uint8 * m_datatexture;
    sf::Texture * m_texture;
    sf::Sprite * m_sprite;

    bool m_close;

public:
    VisionServerBGImpl(const std::string & vision_name, unsigned short vision_port, bool compressed_data,
        unsigned int width, unsigned int height, 
        unsigned int wwidth, unsigned wheight, unsigned iwidth, unsigned iheight)
       :    m_rcv_compressed_data(compressed_data), m_width(width), m_height(height),
            m_wwidth(wwidth), m_wheight(wheight), m_iwidth(iwidth), m_iheight(iheight),
            m_dataFromSocket(new unsigned char[32769]), m_datatexture(new sf::Uint8[width*height*4]),
            m_texture(new sf::Texture), m_sprite(new sf::Sprite), m_close(false)
    {
        if(m_rcv_compressed_data)
        {
            m_compressed_data = new sf::Uint8[width*height*4];
        }
        else
        {
            m_compressed_data = m_datatexture;
        }
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
        if(iwidth == 0 || iheight == 0)
        {
            m_iwidth = m_wwidth;
            m_iheight = m_wheight;
        }
        m_sprite->Resize(m_iwidth, m_iheight);
        m_sprite->SetPosition(m_wwidth/2 - m_iwidth/2, m_wheight/2 - m_iheight/2);

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
#ifndef WIN32
                close(m_sockfd);
#else
				closesocket(m_sockfd);
#endif
        }
    }

    ~VisionServerBGImpl()
    {
        if(m_rcv_compressed_data)
        {
            delete[] m_compressed_data;
        }
        delete[] m_dataFromSocket;
        delete[] m_datatexture;
        delete m_texture;
        delete m_sprite;
#ifndef WIN32
        close(m_sockfd);
#else
		closesocket(m_sockfd);
#endif
    }

    void UpdateLoop()
    {
        while(!m_close)
        {
            sendto(m_sockfd, "get", 4, 0, (struct sockaddr *)&m_serveraddr, sizeof(m_serveraddr));

            int receivedData = 0;
            char packetId = -1;
            memset(m_datatexture, 0, m_width*m_height*4);
            int n = 32769;
            while(n == 32769 && receivedData != -1)
            {
                memset(m_dataFromSocket, '\0', 32769);
                n = recvfrom(m_sockfd, (char*)m_dataFromSocket, 32769, 0, 0, 0);
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
                    memcpy(&(m_compressed_data[32768*packetId]), &(m_dataFromSocket[1]), n - 1);
                }
            }
            if(receivedData != -1)
            {
                if(m_rcv_compressed_data)
                {
                    unpack((unsigned char*)m_compressed_data, packetId*32768 + (n-1), m_datatexture, m_width*m_height*4);
                    for(unsigned int i = 0; i < m_width*m_height; ++i)
                    {
                        m_datatexture[4*i+3] = 255;
                    }
                }
                else
                {
                    /* Shift data from RGB24 to RGBA in place */
                    for(unsigned int i = m_width*m_height; i > 0; --i)
                    {
                        m_datatexture[4*(i-1)+3] = 255;
                        m_datatexture[4*(i-1)+2] = m_datatexture[3*(i-1)+2];
                        m_datatexture[4*(i-1)+1] = m_datatexture[3*(i-1)+1];
                        m_datatexture[4*(i-1)]   = m_datatexture[3*(i-1)];
                    }
                }
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
//        m_sprite->Resize(app->GetWidth(), app->GetHeight());
        app->Draw(*m_sprite);
    }
};

VisionServerBG::VisionServerBG(const std::string & vs_name, unsigned short vs_port, unsigned int width, unsigned int height,
    bool compressed_data,
    unsigned int wwidth, unsigned int wheight, unsigned iwidth, unsigned int iheight)
    : m_impl(new VisionServerBGImpl(vs_name, vs_port, compressed_data, width, height, wwidth, wheight, iwidth, iheight))
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

