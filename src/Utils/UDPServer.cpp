#include <bci-interface/Utils/UDPServer.h>

#include <boost/bind.hpp>

using boost::asio::ip::udp;

namespace bciinterface
{

UDPServer::UDPServer(boost::asio::io_service & io_service, short port)
: io_service_(io_service), socket_(io_service)
{
    udp::endpoint endpoint(udp::v4(), port);
    socket_.open(udp::v4());
    socket_.bind(endpoint);
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&UDPServer::handle_receive_from, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}

const boost::asio::io_service & UDPServer::get_io_service()
{
    return io_service_;    
}

void UDPServer::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
    if (!error && bytes_recvd > 0)
    {
        std::string request(data_);
        std::string answer = handle_request(request);
        socket_.async_send_to(
          boost::asio::buffer(answer.c_str(), answer.size()+1), sender_endpoint_,
          boost::bind(&UDPServer::handle_send_to, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        socket_.async_receive_from(
          boost::asio::buffer(data_, max_length), sender_endpoint_,
          boost::bind(&UDPServer::handle_receive_from, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
}

void UDPServer::handle_send_to(const boost::system::error_code& error, size_t bytes_sent)
{
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&UDPServer::handle_receive_from, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}

} // namespace bciinterface

