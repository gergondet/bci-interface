#include "bci-interface/CommandReceiver/UDPReceiver.h"

#include "bci-interface/Utils/UDPSocket.h"
#include <iostream>
#include <sstream>

namespace bciinterface
{

struct UDPReceiverImpl
{
private:
    UDPSocket m_socket;
    int m_command;
    bool m_close;
public:
    UDPReceiverImpl(unsigned short port) : m_socket(), m_command(0), m_close(false)
    {
        if(!m_socket.CreateServer(port))
        {
            std::cerr << "Error creating socket in UDPReceiver" << std::endl;
        }
    }
    
    ~UDPReceiverImpl()
    {
        m_socket.Close();
    }

    void CommandLoop()
    {
        int timeout = 1;
        while(!m_close)
        {
            std::string buffer;
            if(m_socket.ReceiveAndACK(buffer, "ACK", timeout))
            {
                std::stringstream ss;
                ss << buffer;
                ss >> m_command;
            }
            else
            {
                m_command = 0;
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

UDPReceiver::UDPReceiver(unsigned short port) : m_impl(new UDPReceiverImpl(port))
{}

void UDPReceiver::CommandLoop()
{
    m_impl->CommandLoop();
}

void UDPReceiver::Close()
{
    m_impl->Close();
}

int UDPReceiver::GetCommand()
{
    return m_impl->GetCommand();
}

}

