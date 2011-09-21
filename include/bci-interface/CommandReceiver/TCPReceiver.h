/* Receive commands through TCP connection*/

#ifndef _H_TCPRECEIVER_H_
#define _H_TCPRECEIVER_H_

#include <bci-interface/CommandReceiver.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct TCPReceiverImpl;

class TCPReceiver : public CommandReceiver
{
public:
    TCPReceiver(unsigned short port);

    virtual ~TCPReceiver() {}

    /* Loop to receive command */
    virtual void CommandLoop();

    /* Close command receiver */
    virtual void Close();

    /* Returns the current command output from the BCI */
    virtual int GetCommand();
private:
    boost::shared_ptr<TCPReceiverImpl> m_impl;
};

} // namespace bciinterface

#endif

