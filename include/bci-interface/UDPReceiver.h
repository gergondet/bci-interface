/* Receive commands through UDP connection*/

#ifndef _H_UDPRECEIVER_H_
#define _H_UDPRECEIVER_H_

#include <bci-interface/CommandReceiver.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct UDPReceiverImpl;

class UDPReceiver : public CommandReceiver
{
public:
    UDPReceiver(unsigned short port);

    virtual ~UDPReceiver() {}

    /* Loop to receive command */
    virtual void CommandLoop();

    /* Close command receiver */
    virtual void Close();

    /* Returns the current command output from the BCI */
    virtual int GetCommand();
private:
    boost::shared_ptr<UDPReceiverImpl> m_impl;
};

} // namespace bciinterface

#endif

