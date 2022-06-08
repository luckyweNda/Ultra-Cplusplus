#ifndef CIALLOO_NETWORK_A2S_HPP
#define CIALLOO_NETWORK_A2S_HPP

#include <bits/stdc++.h>
#include <boost/asio.hpp>

namespace {

constexpr unsigned char A2S_INFO_REQUEST[] = {0xFF,0xFF,0xFF,0xFF,0x54,0x53,0x6F,0x75,0x72,0x63,0x65,0x20,0x45,0x6E,0x67,0x69,0x6E,0x65,0x20,0x51,0x75,0x65,0x72,0x79,0x00};
constexpr unsigned char A2S_PLAYER_REQUEST[] = {0xFF,0xFF,0xFF,0xFF,0x55,0xFF,0xFF,0xFF,0xFF};
constexpr unsigned char A2S_RULES_REQUEST[] = {0xFF,0xFF,0xFF,0xFF,0x56,0xFF,0xFF,0xFF,0xFF};

} // static


namespace cialloo {
namespace network {

class a2s
{
public:
    a2s();
    ~a2s();

    a2s(const a2s&) = delete;
    a2s& operator=(const a2s&) = delete;

public:
    struct a2s_info;
    struct a2s_player;
    struct a2s_rules;
    enum class a2s_query_type;

public:


private:
    boost::asio::io_context io_context_;
    boost::asio::ip::udp::socket socket_;
};

inline a2s::a2s() : socket_(io_context_)
{
    socket_.open(boost::asio::ip::udp::v4());
}

inline a2s::~a2s()
{
    if(socket_.is_open())
        socket_.close();
}

enum class a2s::a2s_query_type
{
    A2S_INFO,
    A2S_PLAYER,
    A2S_RULES,
    A2A_PING [[deprecated("no longer support.")]],
    A2S_SERVERQUERY_GETCHALLENGE [[deprecated("no longer support as well.")]]
};


} // namespace network
} // namespace cialloo

#endif