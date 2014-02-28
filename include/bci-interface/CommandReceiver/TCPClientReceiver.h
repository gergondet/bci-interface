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
    BCIINTERFACE_API TCPClientReceiver(const std::string & srv_name, unsigned short port);

    BCIINTERFACE_API virtual ~TCPClientReceiver() {}

    /* Loop to receive command */
    BCIINTERFACE_API virtual void CommandLoop();

    /* Close command Client */
    BCIINTERFACE_API virtual void Close();

    /* Returns the current command output from the BCI */
    BCIINTERFACE_API virtual int GetCommand();

    /* Sends a message to the server */
    BCIINTERFACE_API void Send(const std::string & message);

private:
    boost::shared_ptr<TCPClientReceiverImpl> m_impl;
};

} // namespace bciinterface

#endif

