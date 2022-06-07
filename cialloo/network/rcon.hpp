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

constexpr int PACKET_MAX_SIZE = 4096;

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
    packet pack_packet(std::string cmd, packet_type type);
    uint32_t byte4_to_uint32(unsigned char* byte4);

private:
    uint32_t id_;
    std::string password_;
    boost::asio::io_context io_context_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<boost::asio::ip::tcp::endpoint> endpoint_;
};

inline uint32_t rcon::byte4_to_uint32(unsigned char* byte4)
{
    return static_cast<uint32_t>(byte4[0] | byte4[1] << 8 | byte4[2] << 16 | byte4[3] << 24);
}

inline rcon::packet rcon::pack_packet(std::string cmd, packet_type type)
{
    packet p;
    p.size_ = cmd.length() + HEADER_SIZE;
    p.id_ = id_;
    p.type_ = type;
    p.body_ = cmd;
    return p;
}

inline std::string rcon::send_command(std::string cmd)
{
    packet p1 = pack_packet(cmd, packet_type::SERVERDATA_EXECCOMMAND);
    unsigned char* byte_array = packet_to_byte(p1);
    id_++;
    packet p2 = pack_packet("\0", packet_type::SERVERDATA_RESPONSE_VALUE);
    unsigned char* validator = packet_to_byte(p2);
    socket_->send(boost::asio::buffer(byte_array, p1.size_ + SIZE_FIELD_SIZE));
    socket_->send(boost::asio::buffer(validator, p2.size_ + SIZE_FIELD_SIZE));

    std::string respond;

    while(true)
    {
        unsigned char byte4[4];
        socket_->receive(boost::asio::buffer(byte4, sizeof(byte4)));
        auto packet_size = byte4_to_uint32(byte4);  // size
        unsigned char recv_packet[packet_size];
        socket_->receive(boost::asio::buffer(recv_packet, packet_size));
        std::copy(recv_packet, recv_packet + 4, byte4);
        auto id = byte4_to_uint32(byte4);  // id

        if(id == id_) break;

        char body[packet_size - ID_SIZE - TYPE_SIZE];
        std::copy(recv_packet + ID_SIZE + TYPE_SIZE , recv_packet + packet_size, body);
        respond += body;
    }

    id_++;
    return respond;
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
    send_packet(pack_packet(password_, packet_type::SERVERDATA_AUTH));
    char temp[HEADER_SIZE + SIZE_FIELD_SIZE + 1 /* SERVERDATA_RESPONSE_VALUE body is a null teminator */];
    socket_->receive(boost::asio::buffer(temp, sizeof(temp)));
    socket_->receive(boost::asio::buffer(temp, sizeof(temp)));
    id_++;
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