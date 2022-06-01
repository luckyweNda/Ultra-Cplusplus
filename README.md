# Header Only C Plus Plus Library

Here are examples.

**tcp_client**
```cpp
#include <iostream>
#include <string>
#include "cialloo/network/tcp_client.hpp"

int main()
{
    // There is nothing.
}
```

**tcp_server**
```cpp
#include <iostream>
#include <string>
#include "cialloo/network/tcp_server.hpp"

int main()
{
    cialloo::network::tcp_server server(51000);
    server.on_accept([](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if(ec)
            std::cout << "Connection error.\n";
        else
            std::cout << "Get a connection.\n";
    });

    server.run();
}
```

**websocket_client**
```cpp
#include <iostream>
#include <string>
#include "cialloo/network/websocket_client.hpp"


int main()
{
    cialloo::network::websocket_client client("127.0.0.1", 80, "/");
    client.on_received_text([](std::string str) {
        std::cout << "Receive a message: " << str << std::endl;
    });

    client.run();
}
```