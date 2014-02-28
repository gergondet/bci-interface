#ifndef _H_UDPSERVER_H_
#define _H_UDPSERVER_H_

/* UDPServer provides a server virtual class for UDP
 * Only the request handling function has to be written
 * It operates asynchronously and is based on boost::asio */

#include <boost/asio.hpp>

using boost::asio::ip::udp;

#include <bci-interface/BCIAPI.h>

namespace bciinterface
{

class UDPServer
{
public:
    /* Rely on furnished io_service */
    BCIINTERFACE_API UDPServer(boost::asio::io_service & io_service, short port);

    /* Function to be implemented by the user */
    BCIINTERFACE_API virtual std::string handle_request(const std::string & request_in) = 0;

    BCIINTERFACE_API const boost::asio::io_service & get_io_service();

private:
    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);

    void handle_send_to(const boost::system::error_code& error, size_t bytes_sent);

private:
    boost::asio::io_service& io_service_;
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    enum { max_length = 1024 };
    char data_[max_length];
};

} // namespace bciinterface

#endif
