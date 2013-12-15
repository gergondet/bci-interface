#include "bci-interface/Utils/TCPClient.h"

#include <iostream>
#include <cstring>

namespace bciinterface
{

TCPClient::TCPClient(const std::string & serverName, unsigned short serverPort) : m_error(false)
{
#ifdef WIN32
    WSAStartup(0x0202, &w);
#endif
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 1;
#ifndef WIN32
    setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
#else
    /*FIXME implement for WIN32 socket */
#endif

    m_srvaddr.sin_family = AF_INET;
    m_srvaddr.sin_port = htons(serverPort);

    struct hostent * hent;
    hent = gethostbyname(serverName.c_str());
    if(hent)
    {
        memcpy(&m_srvaddr.sin_addr, hent->h_addr_list[0], hent->h_length);
    }

    int err = connect(m_sockfd, (struct sockaddr *)&m_srvaddr, sizeof(m_srvaddr));
    if(err)
    {
        std::cerr << "Error connecting the TCP socket to " << serverName << ":" << serverPort << std::endl;
        m_error = true;
    }
}

TCPClient::~TCPClient()
{
#ifdef WIN32
	closesocket(m_sockfd);
    WSACleanup();
#else
	close(m_sockfd);
#endif
}

void TCPClient::SendMessage(const std::string & message)
{
    if(!m_error)
    {
        send(m_sockfd, message.c_str(), message.size() + 1, 0);
    }
}

void TCPClient::ReceiveMessage(std::string & message)
{
    if(!m_error)
    {
        char * buffer = new char[1024];
        memset(buffer, 0, 1024);
        recv(m_sockfd, buffer, 1024, 0);
        message.append(buffer);
    }
}

}

