#include "bci-interface/CommandReceiver/TCPClientReceiver.h"

#include "bci-interface/Utils/TCPClient.h"
#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct TCPClientReceiverImpl : public TCPClient
{
private:
    int m_command;
    bool m_close;
public:
    TCPClientReceiverImpl(const std::string & srv_name, unsigned short port)
        : TCPClient(srv_name, port), m_command(0), m_close(false)
    {}

    ~TCPClientReceiverImpl()
    {}

    void CommandLoop()
    {
        while(!m_close)
        {
            std::string buffer;
            ReceiveMessage(buffer);
            std::cout << "Received " << buffer << std::endl;
            if(buffer.size() > 0)
            {
                std::stringstream ss;
                ss << buffer;
                ss >> m_command;
                std::cout << "m_command is now " << m_command << std::endl;
            }
        }
    }

    void Close()
    {
        m_close = true;
    }

    int GetCommand()
    {
        return m_command;
    }
};

    TCPClientReceiver::TCPClientReceiver(const std::string & srv_name, unsigned short port)
    : m_impl(new TCPClientReceiverImpl(srv_name, port))
    {}

    void TCPClientReceiver::CommandLoop()
    {
        m_impl->CommandLoop();
    }

    void TCPClientReceiver::Close()
    {
        m_impl->Close();
    }

    int TCPClientReceiver::GetCommand()
    {
        return m_impl->GetCommand();
    }

    void TCPClientReceiver::Send(const std::string & message)
    {
        m_impl->SendMessage(message);
    }

} // namespace bciinterface

