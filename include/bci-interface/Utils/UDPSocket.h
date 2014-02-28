/*! \brief UDPSocket class with utils
 * \author Pierre Gergondet
 * Copyright: JRL, CNRS-AIST
 * Date: 11/11/2010
 */

#ifndef _BCIMW_UDPSOCKET_H_H
#define _BCIMW_UDPSOCKET_H_H

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <string>

#include <bci-interface/BCIAPI.h>

namespace bciinterface
{

struct UDPSocketImpl;

class UDPSocket
{

public:
    /*! \brief Default constructor */
    BCIINTERFACE_API UDPSocket();

    /*! \brief Open a client socket */
    BCIINTERFACE_API bool CreateClient(const std::string & hostName, unsigned short hostPort);

    /*! \brief Open a server socket */
    BCIINTERFACE_API bool CreateServer(unsigned short serverPort);

    /*! \brief Close a socket */
    BCIINTERFACE_API bool Close();

    /*! \brief Send data through a client socket */
    /*! Returns false if send failed */
    /*! Beware, since it's UDP, true doesn't mean it was succesfully received */
    BCIINTERFACE_API bool Send(const std::string & data);

    /*! \brief Send data through a client socket and get an answer */
    /*! Returns false either if send failed or no answer was received before timeout */ 
    BCIINTERFACE_API bool SendAndReceive(const std::string & data, std::string & buffer, int timeout = 1);

    /*! \brief Receive data from a socket and store it in given buffer */
    /*! Return false and empty buffer if receive failed or timeout */
    BCIINTERFACE_API bool Receive(std::string & buffer, int timeout = 0);

    /*! \brief Receive data from a socket, store it in a buffer and send back
        * an answer, user should provide a callback function to generate the answer */
    BCIINTERFACE_API bool ReceiveAndSend(std::string & buffer, boost::function<std::string (const std::string &)> callback, int timeout = 0);

    /*! \brief ReceiveAndReply with simple ackString as the answer */ 
    BCIINTERFACE_API bool ReceiveAndACK(std::string & buffer, const std::string & ackString, int timeout = 0);

private:
    boost::shared_ptr<UDPSocketImpl> m_udpsckimpl;

};

}

#endif
