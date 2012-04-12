/* Receive commands through UDP connection*/

#ifndef _H_TCPCLIENT_H_
#define _H_TCPCLIENT_H_

#include <bci-interface/CommandReceiver.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct TCPClientImpl;

class TCPClient : public CommandReceiver
{
public:
    TCPClient(const std::string & srv_name, unsigned short port);

    virtual ~TCPClient() {}

    /* Loop to receive command */
    virtual void CommandLoop();

    /* Close command Client */
    virtual void Close();

    /* Returns the current command output from the BCI */
    virtual int GetCommand();

    /* Sends a message to the server */
    void Send(const std::string & message);

private:
    boost::shared_ptr<TCPClientImpl> m_impl;
};

} // namespace bciinterface

#endif

