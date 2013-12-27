/* Receive commands through UDP connection*/

#ifndef _H_TCPCLIENTRECEIVER_H_
#define _H_TCPCLIENTRECEIVER_H_

#include <bci-interface/CommandReceiver.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct TCPClientReceiverImpl;

class TCPClientReceiver : public CommandReceiver
{
public:
    TCPClientReceiver(const std::string & srv_name, unsigned short port);

    virtual ~TCPClientReceiver() {}

    /* Loop to receive command */
    virtual void CommandLoop();

    /* Close command Client */
    virtual void Close();

    /* Returns the current command output from the BCI */
    virtual int GetCommand();

    /* Sends a message to the server */
    void Send(const std::string & message);

private:
    boost::shared_ptr<TCPClientReceiverImpl> m_impl;
};

} // namespace bciinterface

#endif

