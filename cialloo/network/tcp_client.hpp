#ifndef CIALLOO_NETWORK_TCP_CLIENT_HPP
#define CIALLOO_NETWORK_TCP_CLIENT_HPP


#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <algorithm>
#include <boost/asio.hpp>

namespace cialloo {
namespace network {

class tcp_client
{
public:
    tcp_client(std::string hostname, int port);
    ~tcp_client();

    tcp_client(const tcp_client&) = delete;
    tcp_client& operator=(const tcp_client&) = delete;

public:
    void send(std::string message);
    void send(const char* message, size_t size);
    std::string receive();

public:
    int get_port() const;
    std::string_view get_hostname();

private:
    std::string buffer_to_string(const boost::asio::mutable_buffer& buffer);

private:
    int port_;
    std::string hostname_;
    boost::asio::io_context io_context_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<boost::asio::ip::tcp::endpoint> endpoint_;
};

inline std::string tcp_client::buffer_to_string(const boost::asio::mutable_buffer& buffer)
{
    char temp[buffer.size() + 1];
    std::copy(buffer.data(), buffer.data() + buffer.size(), temp);
    temp[buffer.size()] = '\0';
    std::string return_value(temp);
    return return_value;
}

inline std::string tcp_client::receive()
{
    boost::asio::mutable_buffer buffer;
    socket_->receive(buffer);
    return buffer_to_string(buffer);
}

inline void tcp_client::send(const char* message, size_t size)
{
    socket_->send(boost::asio::buffer(message, size));
}

inline void tcp_client::send(std::string message)
{
    socket_->send(boost::asio::buffer(message, message.size()));
}

inline std::string_view tcp_client::get_hostname()
{
    return std::string_view(hostname_);
}

inline int tcp_client::get_port() const
{
    return port_;
}

inline tcp_client::tcp_client(std::string hostname, int port)
    : hostname_(hostname),
      port_(port)
{
    socket_ = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    endpoint_ = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address::from_string(hostname_), port_);
    socket_->connect(*endpoint_);
    //socket_->non_blocking(true);
}

inline tcp_client::~tcp_client()
{
    socket_->close();
}

} // namespace network
} // namespace cialloo


#endif