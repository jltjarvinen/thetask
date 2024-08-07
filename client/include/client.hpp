#pragma once

#include <string>
#include <array>
#include <mutex>
#include <atomic>
#include <thread>
#include <functional>

struct ServerInfo
{
    using Hook = std::function<void(std::string)>;

    std::string name{};
    int port{0};
    int sockfd{-1};
    std::string latest_data{};
    Hook hook{};

    ServerInfo() = default;
    ServerInfo(const std::string& n, int p, Hook data_hook = {}) 
        : name(n), port(p), hook(data_hook) {}
};

template<std::size_t N>
class Client
{
public:
    static constexpr size_t DEFAULT_PRINT_PERIOD_MS = 100;
    Client(std::array<ServerInfo, N>& serverInfo, size_t period = DEFAULT_PRINT_PERIOD_MS)
        : wait(period), servers(serverInfo) {}
    ~Client() { end(); }

    /**
     * @brief Starts the client by initializing and running the data collection.
     * 
     */
    void start();

private:

    /**
     * @brief Initializes connections to all servers.
     * 
     * This function iterates through all servers, attempting to connect each one
     * to the server at "127.0.0.1" on their respective ports.
     */
    void initialize();

    /**
     * @brief Runs the data collection and printing loop.
     * 
     * This function creates threads to read data from each server concurrently.
     * It enters a loop where it periodically prints the latest data from all servers.
     */
    void run();

    /**
     * @brief Ends the client by stopping the data collection and closing all server connections.
     * 
     * This function sets the running flag to false to stop the data collection loop
     * and then iterates through all servers, closing any open sockets.
     */
    void end();

    /**
     * @brief Prints the current timestamp and the latest data from all ports.
     * 
     * This function fetches the current timestamp and iterates through all servers,
     * retrieving and clearing the latest data for each server. It then prints the
     * timestamp along with the server names and their corresponding latest data in
     * JSON format.
     */
    void print();

    size_t wait;

    std::array<ServerInfo, N>& servers;
    std::array<std::mutex, N> mutex{};
    
    std::atomic<bool> running{true};
};
