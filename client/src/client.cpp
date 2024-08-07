#include <iostream>
#include <optional>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "client.hpp"

// explicit template instantiation
template class Client<NUMBER_OF_PORTS>;

namespace
{
int connect_to_server(const char* address, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket for port " << port << std::endl;
        return -1;
    }

    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address for port " << port << std::endl;
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection to port " << port << " failed" << std::endl;
        close(sockfd);
        return -1;
    }

    return sockfd;
}

// thread receive data function
void read_data(ServerInfo &server, std::mutex& mutex, std::atomic<bool>& running)
{
    char buffer[1024];
    while (running)
    {
        int size = read(server.sockfd, buffer, sizeof(buffer) - 1);
        if (size > 0)
        {
            buffer[size - 1] = '\0'; // last character is LF: 0x0a

            (std::lock_guard<std::mutex>(mutex), server.latest_data) = std::string(buffer);

            if (server.hook) server.hook(buffer);
        } 
        else if (size == 0)
        {
            // connection closed, error handling
            break;
        }
    }
}
}

template<std::size_t N> void Client<N>::print()
{
    auto timestamp = std::chrono::system_clock::now().time_since_epoch();
    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count();

    std::cout << "{\"timestamp\": " << ms;
    for (size_t i = 0; i < N; ++i)
    {
        auto& server = servers[i];

        // fetch and clear latest data
        std::string data = [&]() {
            std::lock_guard<std::mutex> lock(mutex[i]);
            std::string temp = server.latest_data;
            server.latest_data.clear();
            return temp;
        }();

        if (data.empty()) data = "--";

        std::cout << ", \"" << server.name << "\": \"" << data << "\"";
    }
    std::cout << "}" << std::endl;
}

template<std::size_t N> void Client<N>::initialize()
{
    for (auto &server : servers)
    {
        server.sockfd = connect_to_server("127.0.0.1", server.port);
        if (server.sockfd < 0)
        {
            std::cerr << "Failed to connect to server on port " << server.port << std::endl;
        }
    }
}

template<std::size_t N> void Client<N>::start()
{
    initialize();

    run();
}

template<std::size_t N> void Client<N>::run()
{
    std::array<std::optional<std::thread>, N> threads;

    for (size_t i = 0; i < N; ++i)
    {
        threads[i].emplace(read_data, std::ref(servers[i]), std::ref(mutex[i]), std::ref(running));
    }

    while (running)
    {
        // wait for the threads to gather data and print most current values
        std::this_thread::sleep_for(std::chrono::milliseconds(wait));
        print();
    }

    // stop threads
    for (auto &t : threads)
    {
        if (t && t->joinable())
        {
            t->join();
        }
    }
}

template<std::size_t N> void Client<N>::end()
{
    running = false;

    // close sockets
    for (auto &server : servers)
    {
        if (server.sockfd >= 0)
        {
            close(server.sockfd);
        }
    }
}
