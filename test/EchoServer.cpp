#include <bci-interface/Utils/UDPServer.h>

class EchoServer : public bciinterface::UDPServer
{
public:
    EchoServer(boost::asio::io_service & io_service, short port) : UDPServer(io_service, port) {}

    std::string handle_request(const std::string & request_in)
    {
        return request_in;
    }
};

int main(int argc, char * argv[])
{
    short port = 4242;
    boost::asio::io_service io_service;
    EchoServer server(io_service, port);
    io_service.run();
}
