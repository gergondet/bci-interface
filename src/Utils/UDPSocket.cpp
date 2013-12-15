#include "bci-interface/Utils/UDPSocket.h"

#include <boost/bind.hpp>
#include <iostream>
#include <cstring>
#include <sstream>

#ifndef WIN32

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <cerrno>

#else

#include <windows.h>
#include <winsock.h>
typedef int socklen_t;
#pragma comment (lib, "ws2_32.lib")

#endif

namespace bciinterface
{

/* UDPSocketImpl */

struct UDPSocketImpl
{
    struct socketImpl
    {
#ifndef WIN32
        int sockfd;
#else
        SOCKET sockfd;
        WSADATA w;
#endif
        struct sockaddr_in bindaddr;
        struct sockaddr_in recvaddr;
        struct sockaddr_in sendaddr;
        bool canSend;

#ifndef WIN32
        socketImpl() : sockfd(-1), canSend(false) {}
#else
        socketImpl() : sockfd(INVALID_SOCKET), canSend(false) {}
#endif

        bool createSocketAndBind(unsigned short port = 0)
        {
#ifndef WIN32
            if(sockfd != -1)
#else
            if(sockfd != INVALID_SOCKET)
#endif
            {
                std::cerr << "Socket already created" << std::endl;
                return false;
            }
#ifdef WIN32
            WSAStartup(0x0202, &w);
#endif

            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if(sockfd < 0)
            {
                std::cerr << "Socket creation error" << std::endl;
#ifndef WIN32
                std::cerr << std::strerror(sockfd) << std::endl;
#endif
                return false;
            }

            bindaddr.sin_family = AF_INET;
            bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            bindaddr.sin_port = htons(port);
            int err = bind(sockfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr));
            if( err < 0)
            {
                std::cerr << "Socket binding error" << std::endl;
#ifndef WIN32
                std::cerr << std::strerror(sockfd) << std::endl;
#endif
                return false;
            }

            return true;
        }


        /* Wrapper for unix recvfrom */
        bool receive(std::string & buffer, int timeout = 0)
        {
            fd_set recvset;
            FD_ZERO(&recvset);
            FD_SET(sockfd, &recvset);
            
            int ret;
            if(timeout == 0) 
            {
                ret = select(sockfd + 1, &recvset, 0, 0, 0);
            }
            else
            {
                struct timeval tv;
                tv.tv_sec = timeout;
                tv.tv_usec = 0;
                ret = select(sockfd + 1, &recvset, 0, 0, &tv);
            }
            if(ret > 0)
            {
                socklen_t addrLength = sizeof(recvaddr);
                char c_buff[1024];
                memset(c_buff, '\0', 1024);
                int n = recvfrom(sockfd, c_buff, 1024, 0, (struct sockaddr *)&recvaddr, &addrLength);
                if(n < 0)
                {
                    std::cerr << "Error in recvfrom" << std::endl;
#ifndef WIN32
                    std::cerr << std::strerror(sockfd) << std::endl;
#endif
                    return false;
                }
                else
                {
                    std::stringstream ss;
                    ss << c_buff;
                    buffer = ss.str();
                    return true;
                }
            }
            else if(ret == 0)
            {
                #ifdef DEBUG
                std::cerr << "Receive timeout after " << timeout << " seconds" << std::endl;
                #endif
                return false;
            }
            else
            {
                std::cerr << "select error!" << std::endl;
#ifndef WIN32
                std::cerr << std::strerror(sockfd) << std::endl;
#endif
            }
        }

        /* Wrapper for unix sendto */
        bool send(const std::string & message)
        {
            if(canSend)
            {
                int n = sendto(sockfd, message.c_str(), message.length()+1, 0, (struct sockaddr *)&sendaddr, sizeof(sendaddr));
                if(n < 0)
                {
                    std::cerr << "sendto failed" << std::endl;
#ifndef WIN32
                    std::cerr << std::strerror(sockfd) << std::endl;
#endif
                    return false;
                }
                return true;
            }
            else
            {
                std::cerr << "sendaddr not set yet" << std::endl;
                return false;
            }
        }

        bool Close()
        {
            if(sockfd != -1)
            {
#ifndef WIN32
                return close(sockfd) == 0;
#else
				return closesocket(sockfd) == 0;
#endif
            }
        };

    };

    struct serverSocket : public socketImpl
    {
        bool receive(std::string & buffer, int timeout = 0)
        {
            if(socketImpl::receive(buffer, timeout))
            {
                sendaddr = recvaddr;
                canSend = true;
                return true;
            }
            return false;
        }
    };

    struct clientSocket : public socketImpl
    {
        void initSendAddr(const std::string & hostName, unsigned short hostPort)
        {
            struct hostent * hent;
            hent = gethostbyname(hostName.c_str());
            if(hent)
            {
                sendaddr.sin_family = AF_INET;
                memcpy(&sendaddr.sin_addr, hent->h_addr_list[0], hent->h_length);
                sendaddr.sin_port = htons(hostPort);
                canSend = true;
            }
            else
            {
                std::cerr << "Host resolution failed" << std::endl;
#ifndef WIN32
                std::cerr << std::strerror(sockfd) << std::endl;
#endif
            }
        }
    };
    
    serverSocket m_server;
    clientSocket m_client;

    UDPSocketImpl() : m_server(), m_client() {}

};

/* UDPSocket */

UDPSocket::UDPSocket() : m_udpsckimpl(new UDPSocketImpl)
{
}

bool UDPSocket::CreateClient(const std::string & hostName, unsigned short hostPort)
{
    if( m_udpsckimpl->m_client.createSocketAndBind() )
    {
        m_udpsckimpl->m_client.initSendAddr(hostName, hostPort);
        return true;
    }
    return false;
}

bool UDPSocket::CreateServer(unsigned short serverPort)
{
    return m_udpsckimpl->m_server.createSocketAndBind(serverPort);
}

bool UDPSocket::Close()
{
    m_udpsckimpl->m_server.Close();
    m_udpsckimpl->m_client.Close();
#ifdef WIN32
    WSACleanup();
#endif
    return true;
}

bool UDPSocket::Send(const std::string  & data)
{
    return m_udpsckimpl->m_client.send(data);
}

bool UDPSocket::SendAndReceive(const std::string & data, std::string & buffer, int timeout)
{
    if(m_udpsckimpl->m_client.send(data))
    {
        return m_udpsckimpl->m_client.receive(buffer, timeout);
    }
    else
    {
        #ifdef DEBUG
        std::cerr << "SendAndReceive failure" << std::endl;
        #endif
        return false;
    }
}

bool UDPSocket::Receive(std::string & buffer, int timeout)
{
    return m_udpsckimpl->m_server.receive(buffer, timeout);
}

bool UDPSocket::ReceiveAndSend(std::string & buffer, boost::function<std::string (const std::string &)> callback, int timeout)
{
    if(m_udpsckimpl->m_server.receive(buffer, timeout))
    {
        return m_udpsckimpl->m_server.send(callback(buffer));
    }
    else
    {
        #ifdef DEBUG
        std::cerr << "ReceiveAndReply failed" << std::endl;
        #endif
        return false;
    }
}

std::string ACKcallback(const std::string & ackString)
{
    return ackString;
}

bool UDPSocket::ReceiveAndACK(std::string & buffer, const std::string & ackString, int timeout)
{
    return ReceiveAndSend(buffer, boost::bind(ACKcallback, ackString), timeout);
}

} //namespace 

