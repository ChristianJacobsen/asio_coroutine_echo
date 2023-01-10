#include <asio/buffer.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/ip/tcp.hpp>

#include <fmt/core.h>

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>

int main(int argc, const char *argv[]) {
    std::string message;
    uint16_t port = 4000;

    if (argc == 1) {
        fmt::print(stderr, "[C]: You need to specify a message and optionally a port!\n");
        return EXIT_FAILURE;
    }

    if (argc > 1) {
        message = argv[1];
    }

    if (argc > 2) {
        const auto res = std::from_chars(argv[2], argv[3], port);
        if (res.ec != std::errc{}) {
            fmt::print(stderr, "[C]: Failed to parse port!\n");
            return EXIT_FAILURE;
        }
    }

    asio::io_context context;
    asio::ip::tcp::endpoint endpoint{asio::ip::make_address_v4("127.0.0.1"), port};
    asio::ip::tcp::socket socket{context};
    socket.connect(endpoint);

    fmt::print("[C]: Sending '{}' to port '{}'\n", message, port);

    socket.write_some(asio::buffer(message));

    std::array<char, 128> buffer;
    const auto num_read = socket.read_some(asio::buffer(buffer));

    const std::string_view received_message{buffer.data(), num_read};

    fmt::print("[C]: Received '{}'\n", received_message);

    return EXIT_SUCCESS;
}
