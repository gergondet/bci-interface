#include <bci-interface/CommandReceiver/TCPReceiver.h>

#include <boost/thread.hpp>

#include <iostream>
#include <sstream>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <cerrno>


namespace bciinterface
{

struct TCPReceiverImpl
{
private:
    int sSocket;
    int cSocket;
    struct sockaddr_in addr;
    bool has_client;
    int m_command;
    bool m_close;
    boost::thread * m_th;
public:
    TCPReceiverImpl(unsigned short port) : has_client(false), m_command(0), m_close(false), m_th(0)
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl (INADDR_ANY);
        sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        bind(sSocket, (struct sockaddr *)&addr, sizeof(addr));
        listen(sSocket, 12);
        m_th = new boost::thread(boost::bind(&TCPReceiverImpl::GetClient, this));
    }
    
    ~TCPReceiverImpl()
    {
        if(cSocket != -1)
        {
            close(cSocket);
        }
        if(sSocket != -1)
        {
            close(sSocket);
        }
        if(m_th)
        {
            m_th->interrupt();
            delete m_th;
        }
    }

    void CommandLoop()
    {
        while(!m_close)
        {
            if(has_client)
            {
                char buffer[256];
                int err = recv(cSocket, buffer, 256, 0);
                if(err > 0)
                {
                    std::stringstream ss;
                    ss << buffer;
                    ss >> m_command;
                    std::cout << "Got command " << m_command << std::endl;
                    sleep(2);
                }
                else
                {
                    m_command = 0;
                }
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
        int command = m_command;
        m_command = 0;
        return command;
    }

    void GetClient()
    {
        while(!m_close)
        {
            cSocket = accept(sSocket, 0, 0);
            has_client = true;
        }
    }
};

TCPReceiver::TCPReceiver(unsigned short port) : m_impl(new TCPReceiverImpl(port))
{}

void TCPReceiver::CommandLoop()
{
    m_impl->CommandLoop();
}

void TCPReceiver::Close()
{
    m_impl->Close();
}

int TCPReceiver::GetCommand()
{
    return m_impl->GetCommand();
}

}

