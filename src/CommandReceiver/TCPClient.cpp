#include <bci-interface/CommandReceiver/TCPClient.h>

#include <bci-middleware/TCPClient.h>
#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct TCPClientImpl : public bcimw::TCPClient
{
private:
    int m_command;
    bool m_close;
public:
    TCPClientImpl(const std::string & srv_name, unsigned short port)
        : bcimw::TCPClient(srv_name, port), m_command(0), m_close(false)
    {}

    ~TCPClientImpl()
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

    TCPClient::TCPClient(const std::string & srv_name, unsigned short port)
    : m_impl(new TCPClientImpl(srv_name, port))
    {}

    void TCPClient::CommandLoop()
    {
        m_impl->CommandLoop();
    }

    void TCPClient::Close()
    {
        m_impl->Close();
    }

    int TCPClient::GetCommand()
    {
        return m_impl->GetCommand();
    }

    void TCPClient::Send(const std::string & message)
    {
        m_impl->SendMessage(message);
    }

} // namespace bciinterface

