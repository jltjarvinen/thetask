#include "client.hpp"
#include "control.hpp"

int main()
{
    Control control(4000);
    std::array<ServerInfo, NUMBER_OF_PORTS> servers {
        ServerInfo{"out1", 4001},
        ServerInfo{"out2", 4002},
        ServerInfo{"out3", 4003, [&control](const std::string& s) { control.run(s); }}
    };
    Client client(servers, 20);
    client.start();

    return 0;
}
