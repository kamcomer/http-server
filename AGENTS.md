# AGENTS.md

Guidelines for AI agents working on this HTTP server codebase.

## Project Overview

A lightweight HTTP/1.1 server written in C. Uses CMake build system and the `check` testing framework.

## Build Commands

```bash
# Standard build
mkdir -p build && cd build
cmake ..
make

# Clean build
rm -rf build && mkdir build && cd build && cmake .. && make
```

The executable will be at `./build/app/http-server`.

## Test Commands

```bash
# Build and run all tests
cd build && make && ./tests/unit/tests
```

## Code Style Guidelines

### C Standards
- Use C11 standard
- Compile with Clang (set in CMakeLists.txt)
- Generate compile commands for clangd: `CMAKE_EXPORT_COMPILE_COMMANDS=ON`

### Formatting
- 2 spaces for indentation (not tabs)
- Opening braces on same line for functions and control structures
- Maximum line length: ~100 characters
- No trailing whitespace

### Naming Conventions
- Functions: `snake_case` (e.g., `process_request`, `header_parse_method`)
- Types/Structs: `PascalCase` (e.g., `RequestMethod`, `ProtocolType`)
- Enums: `SCREAMING_SNAKE_CASE` for values (e.g., `METHOD_GET`, `SUCCESS_OK`)
- Constants/Macros: `SCREAMING_SNAKE_CASE` (e.g., `BUFFER_SIZE`, `MAX_URI_LENGTH`)
- Private functions: `static` keyword, still use `snake_case`

### File Organization
- Entry point: `app/main.c`
- Headers: `include/*.h`
- Source: `src/*.c`
- Tests: `tests/unit/*.c`

### Include Order
1. Corresponding header (for .c files)
2. Other project headers
3. System headers (use `< >`)

Example:
```c
#include "request.h"      // Corresponding header first
#include "util.h"         // Other project headers
#include <stdio.h>        // System headers
#include <stdlib.h>
```

### Error Handling
- Return negative integers or named constants for errors
- Use `perror()` for system call failures
- Use `fprintf(stderr, ...)` for other errors
- Check NULL pointers before dereferencing
- Free allocated memory on error paths
- Exit codes: Use `EXIT_SUCCESS` and `EXIT_FAILURE`

### Memory Management
- Always check malloc/calloc/realloc return values
- Free what you allocate (match every malloc with a free)
- Use `strdup()` for string duplication
- Close file descriptors and sockets properly

### Comments
- Use `//` for single-line comments
- Use `/* */` for multi-line or function documentation
- Mark TODOs clearly: `// TODO: description`
- Keep comments concise and relevant

### Type Safety
- Use explicit struct tags
- Prefer `typedef struct` for cleaner usage
- Use `size_t` for sizes, `ssize_t` for signed sizes
- Use `bool` from `<stdbool.h>` for boolean values

### Functions
- Keep functions focused and small
- Early returns for error conditions
- Pass structs by pointer, not by value (for efficiency)
- Mark file-scope functions as `static`

## Architecture

- **app/main.c**: Entry point
- **src/server.c**: Socket handling, fork-based concurrency
- **src/request.c**: HTTP request parsing
- **src/response.c**: HTTP response generation
- **src/progargs.c**: Command-line argument parsing
- **src/util.c**: Utilities (MIME types, URL decoding, path safety)
- **src/logging.c**: Structured logging

## Common Constants

- `BUFFER_SIZE`: 1024 bytes for read buffers
- `MAX_URI_LENGTH`: 255 characters
- `MAX_METHOD_LENGTH`: 10 characters
- `MAX_PROTOCOL_LENGTH`: 10 characters
- `MAX_REQUEST_SIZE`: 8192 bytes
- Default port: 8080

## Git Ignore

Already configured in `.gitignore`:
- `**DS_Store`
- `build/`
- `.cache/`
- `test_site/`
- `.vscode`
- `RFC*`
- `*.log`

## Testing

- Uses the `check` framework
- Unit tests in `tests/unit/`
- Run tests with: `./tests/unit/tests`

## Security Considerations

- Path traversal protection is implemented
- Request size limits enforced
- Hidden file protection
- Absolute path blocking
