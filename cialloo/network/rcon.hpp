#ifndef CIALLOO_NETWORK_RCON_HPP
#define CIALLOO_NETWORK_RCON_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <bitset>
#include <memory>
#include <cialloo/network/tcp_client.hpp>

namespace cialloo {
namespace network {

class rcon
{
public:
    rcon();
    ~rcon();

private:
    std::shared_ptr<cialloo::network::tcp_client> tcp_client_;
};

inline rcon::rcon()
{
}

inline rcon::~rcon()
{
}

}
}

#endif