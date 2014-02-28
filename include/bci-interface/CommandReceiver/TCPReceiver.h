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
    BCIINTERFACE_API TCPReceiver(unsigned short port);

    BCIINTERFACE_API virtual ~TCPReceiver() {}

    /* Loop to receive command */
    BCIINTERFACE_API virtual void CommandLoop();

    /* Close command receiver */
    BCIINTERFACE_API virtual void Close();

    /* Returns the current command output from the BCI */
    BCIINTERFACE_API virtual int GetCommand();
private:
    boost::shared_ptr<TCPReceiverImpl> m_impl;
};

} // namespace bciinterface

#endif

