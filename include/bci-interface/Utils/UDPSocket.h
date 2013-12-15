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

namespace bciinterface
{

struct UDPSocketImpl;

class UDPSocket
{

public:
    /*! \brief Default constructor */
    UDPSocket();

    /*! \brief Open a client socket */
    bool CreateClient(const std::string & hostName, unsigned short hostPort);

    /*! \brief Open a server socket */
    bool CreateServer(unsigned short serverPort);

    /*! \brief Close a socket */
    bool Close();

    /*! \brief Send data through a client socket */
    /*! Returns false if send failed */
    /*! Beware, since it's UDP, true doesn't mean it was succesfully received */
    bool Send(const std::string & data);

    /*! \brief Send data through a client socket and get an answer */
    /*! Returns false either if send failed or no answer was received before timeout */ 
    bool SendAndReceive(const std::string & data, std::string & buffer, int timeout = 1);

    /*! \brief Receive data from a socket and store it in given buffer */
    /*! Return false and empty buffer if receive failed or timeout */
    bool Receive(std::string & buffer, int timeout = 0);

    /*! \brief Receive data from a socket, store it in a buffer and send back
        * an answer, user should provide a callback function to generate the answer */
    bool ReceiveAndSend(std::string & buffer, boost::function<std::string (const std::string &)> callback, int timeout = 0);

    /*! \brief ReceiveAndReply with simple ackString as the answer */ 
    bool ReceiveAndACK(std::string & buffer, const std::string & ackString, int timeout = 0);

private:
    boost::shared_ptr<UDPSocketImpl> m_udpsckimpl;

};

}

#endif
