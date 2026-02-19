# HTTP Server

A lightweight HTTP/1.1 server written in C.

## Features

- **GET and HEAD requests** - Serves static files from a specified directory
- **MIME type detection** - Automatic content-type based on file extension
- **URL decoding** - Handles percent-encoded URLs
- **HTTP header parsing** - Parses Host, User-Agent, Accept, and more
- **Path traversal protection** - Blocks unsafe path access
- **Request size limits** - Prevents DoS attacks
- **Keep-Alive support** - Reuses connections for multiple requests
- **Concurrent connections** - Handles multiple clients simultaneously
- **Command-line arguments** - Configure port and root directory
- **Signal handling** - Graceful shutdown on SIGINT (Ctrl+C)
- **Structured logging** - Configurable log levels

## Building

```bash
mkdir build && cd build
cmake ..
make
```

The executable will be at `./build/app/http-server`.

## Docker

```bash
# Build image
docker build -t http-server .

# Run container
docker run -p 8080:8080 http-server
```

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
- **src/server.c** - Socket handling, fork-based concurrency
- **src/request.c** - HTTP request parsing
- **src/response.c** - Response generation and file serving
- **src/progargs.c** - Command-line argument handling
- **src/util.c** - Utilities (MIME types, URL decoding, path safety)
- **src/logging.c** - Structured logging
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

## Security Features

- Path traversal protection (`..` blocked)
- Request size limits (8KB max)
- Hidden file protection
- Absolute path blocking

## HTTP Compliance

- HTTP/1.1 support
- Keep-Alive connections (5s timeout, 10 requests max)
- HEAD method support
- Proper status codes: 200, 400, 403, 404, 500

## Project Structure

```
http-server/
├── app/
│   └── main.c           # Entry point
├── src/
│   ├── server.c/h       # Socket handling
│   ├── request.c/h      # Request parsing
│   ├── response.c/h     # Response generation
│   ├── progargs.c/h    # CLI arguments
│   ├── util.c/h        # Utilities
│   └── logging.c/h     # Logging
├── include/
│   ├── http_types.h    # HTTP types
│   ├── request.h
│   ├── response.h
│   ├── server.h
│   ├── progargs.h
│   ├── util.h
│   └── logging.h
├── tests/
│   └── unit/           # Unit tests
├── .github/workflows/  # CI/CD
├── Dockerfile
└── .clang-format
```
