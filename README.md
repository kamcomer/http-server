# HTTP Server

A lightweight HTTP/1.1 server written in C.

## Features

- **GET and HEAD requests** - Serves static files from a specified directory
- **MIME type detection** - Automatic content-type based on file extension (20+ types supported)
- **URL decoding** - Handles percent-encoded URLs and `+` for spaces
- **HTTP header parsing** - Parses Host, User-Agent, Accept, and more
- **Path traversal protection** - Blocks unsafe path access (`..`, absolute paths, hidden files)
- **Request size limits** - Prevents DoS attacks (8KB max request size)
- **Keep-Alive support** - Reuses connections for multiple requests (client-initiated)
- **Concurrent connections** - Handles multiple clients simultaneously via fork-based concurrency
- **Command-line arguments** - Configure port and root directory
- **Signal handling** - Graceful shutdown on SIGINT (Ctrl+C)
- **Structured logging** - Timestamped logs with DEBUG/INFO/WARN/ERROR levels

## Supported HTTP Methods

- **GET** - Fully supported, serves static files with proper MIME types
- **HEAD** - Supported (response includes headers only, no body)
- **POST, PUT, DELETE, OPTIONS, TRACE, CONNECT** - Parsed but not implemented (returns error)

## MIME Types Supported

The server automatically detects content-type for the following file extensions:

| Extension | MIME Type |
|-----------|-----------|
| html, htm | text/html |
| css | text/css |
| js | application/javascript |
| json | application/json |
| xml | application/xml |
| txt | text/plain |
| png, jpg, jpeg, gif, svg, ico, webp | image/* |
| pdf | application/pdf |
| zip | application/zip |
| woff, woff2, ttf, otf | font/* |
| mp4, webm | video/* |
| mp3, wav, ogg | audio/* |

Default: `application/octet-stream` for unknown types.

## Logging

The server includes a structured logging system with timestamped output:

- **DEBUG** - Detailed debugging information
- **INFO** - General informational messages
- **WARN** - Warning messages for potential issues
- **ERROR** - Error messages for failures

Note: The log level is currently hardcoded to INFO. Future versions may expose configuration via CLI.

## Error Responses

All error responses (400, 403, 404, 500) return an HTML page with the appropriate HTTP status code and a human-readable message in the response body.

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

- HTTP/1.1 and HTTP/1.0 support
- Keep-Alive connections (client-initiated via `Connection: keep-alive` header, 5s timeout, 10 requests max)
- HEAD method support (headers only, no body in response)
- Proper status codes: 200, 400, 403, 404, 500

## Known Limitations

- **Zombie processes** - The server uses fork-based concurrency but does not have a SIGCHLD handler or `waitpid()` call, which may create zombie processes.
- **HEAD method** - While supported, the response always sends `Connection: close` header instead of respecting Keep-Alive.
- **Hardcoded configurations** - Keep-Alive timeout (5s), max Keep-Alive requests (10), and socket backlog (3) are not configurable.
- **Hidden file handling** - Inconsistent behavior: `.hidden` files in the root directory are served, but `foo/.secret` paths return 403.
- **Method rejection** - Unsupported methods (POST, PUT, etc.) print to stdout instead of returning a proper HTTP 405 response.

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
