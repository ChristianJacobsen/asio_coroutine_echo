# ASIO-Standalone Echo Client/Server using C++20 Coroutines

## Libraries used

* [ASIO-Standalone](https://github.com/chriskohlhoff/asio/)
* [fmt](https://github.com/fmtlib/fmt)

## Build

Requires Meson and Ninja to build, as well as a C++20-compliant compiler.

Uses Meson's WrapDB to download missing libraries in case you don't have them on your system.

I've personally tested with GCC 12 and Clang 14, but your milage may wary.

```sh
# Skip this section if you already have Meson and Ninja installed.
$ python -m virtualenv .venv
$ source .venv/bin/activate
$ pip install -r requirements.txt

# Build the executables
$ meson setup build
$ meson compile -C build
```

## Run

### Regular

```sh
# Run the server
$ ./build/asio_server &

# Run the client with your desired message
$ ./build/asio_client "Hello world!"
```

### With custom port

```sh
$ ./build/asio_server 5000 &
$ ./build/asio_client "How are you?" 5000
```
