# HTTP Server

A simple, lightweight HTTP/1.1 server written in C.

## Features

- **GET requests** - Serves static files from a specified directory
- **Command-line arguments** - Configure port and root directory
- **Signal handling** - Graceful shutdown on SIGINT (Ctrl+C)
- **Socket options** - SO_REUSEADDR and SO_REUSEPORT enabled for quick restarts

## Building

```bash
mkdir build && cd build
cmake ..
make
```

The executable will be at `./build/app/http-server`.

## Usage

```bash
./build/app/http-server [--port|-p <port>] [--root|-r <directory>]
```

### Arguments

| Argument | Short | Description | Default |
|----------|-------|-------------|---------|
| `--port` | `-p` | Port to listen on | `8080` |
| `--root` | `-r` | Root directory to serve files from | Current directory |

### Examples

```bash
# Start server on port 8080 serving current directory
./build/app/http-server

# Start server on port 3000
./build/app/http-server --port 3000

# Start server on port 8080 serving /var/www
./build/app/http-server --root /var/www

# Combined options
./build/app/http-server -p 8080 -r ./public
```

## Architecture

- **app/main.c** - Entry point
- **src/server.c** - Server socket handling and main loop
- **src/request.c** - HTTP request parsing (method, URI, protocol)
- **src/response.c** - HTTP response generation and file serving
- **src/progargs.c** - Command-line argument handling
- **src/util.c** - Utility functions
- **include/http_types.h** - Shared HTTP types and enums
- **include/server.h** - Server module interface

## Testing

```bash
# Build and run tests
cd build
cmake ..
make
./tests/unit/tests
```

## Status

Currently supports:
- HTTP/1.1 GET requests
- Static file serving
- Error responses (404 Not Found, 500 Internal Server Error)

Not yet implemented:
- Other HTTP methods (POST, PUT, DELETE, etc.)
- Content-Type detection
- Keep-Alive connections
- HTTP headers parsing
- Concurrent connections
