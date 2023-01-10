#define ASIO_HAS_CO_AWAIT
#define ASIO_HAS_STD_COROUTINES
#include <asio/buffer.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>

#include <fmt/core.h>

#include <array>
#include <charconv>
#include <cstdint>
#include <cstdlib>

namespace {
asio::awaitable<void> echo_once(asio::ip::tcp::socket &socket) {
    std::array<char, 128> data;
    const auto num_read = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
    fmt::print("[S]: Echoing '{}'\n", std::string_view{data.data(), num_read});
    co_await socket.async_write_some(asio::buffer(data), asio::use_awaitable);
}

asio::awaitable<void> echo(asio::ip::tcp::socket socket) {
    try {
        while (true) {
            co_await echo_once(socket);
        }
    } catch (const std::exception &e) {
        fmt::print(stderr, "[S]: Echo error '{}'\n", e.what());
    }
}

asio::awaitable<void> listener(uint16_t port) {
    auto executor = co_await asio::this_coro::executor;
    asio::ip::tcp::acceptor acceptor{executor, {asio::ip::tcp::v4(), port}};
    while (true) {
        auto socket = co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(executor, echo(std::move(socket)), asio::detached);
    }
}
} // namespace

int main(int argc, const char *argv[]) {
    uint16_t port = 4000;

    if (argc > 1) {
        const auto res = std::from_chars(argv[1], argv[2], port);
        if (res.ec != std::errc()) {
            fmt::print(stderr, "[S]: Could not parse port\n");
            return EXIT_FAILURE;
        }
    }

    try {
        asio::io_context context;
        asio::signal_set signals{context, SIGINT, SIGTERM};
        signals.async_wait([&](auto, auto) {
            fmt::print("[S]: Shutting down...\n");
            context.stop();
        });

        asio::co_spawn(context, listener(port), asio::detached);

        context.run();
    } catch (const std::exception &e) {
        fmt::print(stderr, "[S]: Error '{}'\n", e.what());
    }

    return EXIT_SUCCESS;
}
