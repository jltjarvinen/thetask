#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "control.hpp"

Control::Control(int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(sockfd);
        return;
    }
}

namespace
{
unsigned short big_endian(unsigned short value) {
    return (value >> 8) | (value << 8);
}
}

void Control::write(const std::array<unsigned short, MESSAGE_WRITE_SIZE> message)
{
    if (sendto(sockfd, message.data(), MESSAGE_WRITE_BYTES, 0, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error sending message" << std::endl;
        close(sockfd);
        return ;
    }
}

void Control::run(std::string value)
{
    float number = std::stof(value); // exception handling

    const auto received = (number >= 3.0) ? State::HIGH : State::LOW;

    if (state == received) return;

    state = received;

    constexpr unsigned short READ = 1;
    constexpr unsigned short WRITE = 2;

    constexpr unsigned short OUT1 = 1;
    constexpr unsigned short OUT2 = 2;
    constexpr unsigned short OUT3 = 3;

    constexpr unsigned short ENABLED = 14;
    constexpr unsigned short AMPLITUDE = 170;
    constexpr unsigned short FREQUENCY = 255;

#if 0
* When the value on the output 3 of the server becomes greater than or equal 3.0:
    * Set the frequency of server output 1 to 1Hz.
    * Set the amplitude of server output 1 to 8000.

* When the value on the output 3 becomes lower than 3.0:
    * Set the frequency of server output 1 to 2Hz.
    * Set the amplitude of server output 1 to 4000.
#endif

    const unsigned short frequency = (state == State::HIGH) ? 1000 : 2000;
    const unsigned short amplitude = (state == State::HIGH) ? 8000 : 4000;

    write({big_endian(WRITE), big_endian(1), big_endian(AMPLITUDE), big_endian(amplitude)});
    write({big_endian(WRITE), big_endian(1), big_endian(FREQUENCY), big_endian(frequency)});
}
