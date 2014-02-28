/*! \brief TCPClient class with utils
 * \author Pierre Gergondet
 * Copyright: JRL, CNRS-AIST
 * Date: 11/11/2010
 */

#ifndef _BCIMW_TCPCLIENT_H_H
#define _BCIMW_TCPCLIENT_H_H

#ifndef WIN32

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>

#else

#include <windows.h>
#include <winsock.h>

#endif

#include <string>

#include <bci-interface/BCIAPI.h>

namespace bciinterface
{

class TCPClient 
{

public:
    /*! \brief Default constructor */
    BCIINTERFACE_API TCPClient(const std::string & serverName, unsigned short serverPort);

    BCIINTERFACE_API virtual ~TCPClient();

    BCIINTERFACE_API void SendMessage(const std::string & message);

    BCIINTERFACE_API void ReceiveMessage(std::string & message);

private:
#ifndef WIN32
    int m_sockfd;
#else
    SOCKET m_sockfd;
    WSADATA w;
#endif
    struct sockaddr_in m_srvaddr;
    bool m_error;

};

}

#endif

