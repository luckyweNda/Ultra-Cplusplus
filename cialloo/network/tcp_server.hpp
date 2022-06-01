#ifndef CIALLOO_NETWORK_TCP_SERVER_HPP
#define CIALLOO_NETWORK_TCP_SERVER_HPP

#include <iostream>
#include <memory>
#include <thread>
#include <functional>
#include <boost/asio.hpp>

namespace cialloo {
namespace network {

class tcp_server
{
public:
    explicit tcp_server(int listen_port);
    ~tcp_server();

    tcp_server(const tcp_server&) = delete;
    tcp_server& operator=(const tcp_server&) = delete;

public:
    void run();
    int get_port() const;
    void on_accept(std::function<void(boost::system::error_code ec, boost::asio::ip::tcp::socket socket)> callback);

private:
    void do_accept();

private:
    std::function<void(boost::system::error_code ec, boost::asio::ip::tcp::socket socket)> accept_callback_;

private:
    int port_;
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::endpoint endpoint_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
};

inline void tcp_server::run()
{
    io_context_.run();
}

inline void tcp_server::on_accept(std::function<void(boost::system::error_code ec, boost::asio::ip::tcp::socket socket)> callback)
{
    accept_callback_ = callback;
}

inline void tcp_server::do_accept()
{
    acceptor_->async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
        if(accept_callback_ != nullptr)
            accept_callback_(ec, std::move(socket));

        do_accept();
    });
}

inline int tcp_server::get_port() const
{
    return port_;
}

inline tcp_server::tcp_server(int listen_port) 
    : port_(listen_port),
      endpoint_(boost::asio::ip::tcp::v4(), listen_port),
      accept_callback_(nullptr)
{
    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_context_, endpoint_);
    do_accept();
}

inline tcp_server::~tcp_server()
{
    io_context_.stop();
}

} // namespace network
} // namespace cialloo

#endif