#include "client.hpp"

int main()
{
    std::array<ServerInfo, NUMBER_OF_PORTS> servers {
        ServerInfo{"out1", 4001},
        ServerInfo{"out2", 4002},
        ServerInfo{"out3", 4003}
    };
    Client client(servers);
    client.start();

    return 0;
}
