#include <string>
#include <netinet/in.h>
#include <limits>
#include <array>

class Control
{
    static constexpr size_t MESSAGE_WRITE_SIZE = 4;
    static constexpr size_t MESSAGE_WRITE_BYTES = MESSAGE_WRITE_SIZE * sizeof(unsigned short);
public:

    /**
     * @brief Constructs a Control object and initializes the UDP socket.
     * 
     * @param port The port number to connect to the server.
     */
    explicit Control(int port);

    /**
     * @brief Runs the control logic based on the input value.
     * 
     * This function converts the input string value to a float and determines the
     * state (HIGH or LOW) based on the value. If the state changes, it updates the
     * server output frequency and amplitude.
     * 
     * @param value The input value as a string, which is converted to a float for processing.
     */
    void run(std::string value);

private:
    
    /**
     * @brief Sends a message to the server.
     * 
     * @param message The message to be sent, represented as an array of big endian unsigned 16bit values.
     */
    void write(const std::array<unsigned short, MESSAGE_WRITE_SIZE> message);

    static constexpr float NAN = std::numeric_limits<float>::quiet_NaN();
    int sockfd{-1};
    struct sockaddr_in server_addr{};
    static constexpr const char* address = "127.0.0.1";

    enum class State { UNINITIALIZED, LOW, HIGH }; // amplitude

    State state{State::UNINITIALIZED};
};
