#ifndef CIALLOO_NETWORK_RCON_HPP
#define CIALLOO_NETWORK_RCON_HPP

#include <bits/stdc++.h>
#include <boost/asio.hpp>

namespace {

constexpr int SIZE_FIELD_SIZE = 4;
constexpr int ID_SIZE = 4;
constexpr int TYPE_SIZE = 4;
constexpr int TERMINATOR_SIZE = 1;
constexpr int HEADER_SIZE = ID_SIZE + TYPE_SIZE + TERMINATOR_SIZE;

} // static

namespace cialloo {
namespace network {

class rcon
{
public:
    rcon(std::string hostname, int port, std::string password);
    ~rcon();

    rcon(const rcon&) = delete;
    rcon& operator=(const rcon&) = delete;

public:
enum class packet_type
{
    SERVERDATA_RESPONSE_VALUE = 0,
    SERVERDATA_EXECCOMMAND = 2,
    SERVERDATA_AUTH_RESPONSE = 2,
    SERVERDATA_AUTH = 3
};

struct packet 
{
    uint32_t size_;  // body.size() + HEADER_SIZE
    uint32_t id_;
    packet_type type_;
    std::string body_;
    char terminator_;

    packet() { terminator_ = '\0'; }
};

public:
    std::string send_command(std::string cmd);

private:
    void authorize();
    void send_packet(packet pack);
    unsigned char* packet_to_byte(packet pack);

private:
    uint32_t id_;
    std::string password_;
    boost::asio::io_context io_context_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<boost::asio::ip::tcp::endpoint> endpoint_;
};

inline std::string rcon::send_command(std::string cmd)
{

}

inline unsigned char* rcon::packet_to_byte(packet pack)
{
    unsigned char* byte_array = new unsigned char[pack.size_ + SIZE_FIELD_SIZE];
    byte_array[0] = pack.size_;
    byte_array[4] = pack.id_;
    byte_array[8] = static_cast<int>(pack.type_);
    std::copy(pack.body_.c_str(), pack.body_.c_str() + pack.body_.length(), byte_array + 12);
    byte_array[pack.size_ + SIZE_FIELD_SIZE] = pack.terminator_;
    return byte_array;
}

inline void rcon::send_packet(packet pack)
{
    unsigned char* pack_byte = packet_to_byte(pack);
    socket_->send(boost::asio::buffer(pack_byte, pack.size_ + SIZE_FIELD_SIZE));
    delete pack_byte;
}

inline void rcon::authorize()
{
    packet p;
    p.id_ = id_;
    p.type_ = packet_type::SERVERDATA_AUTH;
    p.body_ = password_;
    p.size_ = password_.length() + HEADER_SIZE;
    send_packet(p);
    char temp[HEADER_SIZE + SIZE_FIELD_SIZE + 1 /* SERVERDATA_RESPONSE_VALUE body is a null teminator */];
    socket_->receive(boost::asio::buffer(temp, sizeof(temp)));
    socket_->receive(boost::asio::buffer(temp, sizeof(temp)));
}

inline rcon::rcon(std::string hostname, int port, std::string password)
    : endpoint_(std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::address_v4::from_string(hostname), port)),
      socket_(std::make_shared<boost::asio::ip::tcp::socket>(io_context_)),
      password_(password),
      id_(1)
{
    socket_->connect(*endpoint_);
    authorize();
}

inline rcon::~rcon()
{
}

} // namespace network
} // namespace cialloo


#endif