#include <iostream>
#include <string>
#include <string_view>
#include <memory>

#include <boost/asio.hpp>

namespace cialloo {
namespace network {

class tcp_client
{
public:
    tcp_client(std::string hostname, int port);
    ~tcp_client();

public:
    void send(std::string message);
    void send(const char* message, size_t size);

public:
    int get_port() const;
    std::string_view get_hostname();

private:
    int port_;
    std::string hostname_;
    boost::asio::io_context io_context_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<boost::asio::ip::tcp::endpoint> endpoint_;
};

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
    socket_->non_blocking(true);
}

inline tcp_client::~tcp_client()
{
    socket_->close();
}

} // namespace network
} // namespace cialloo