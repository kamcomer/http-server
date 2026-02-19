# Core library source files (explicit list instead of GLOB)
set(LIB_SOURCES
    server.c
    request.c
    response.c
    progargs.c
    util.c
)

# Create static library
add_library(httpserver STATIC ${LIB_SOURCES})

# Public include directory
target_include_directories(httpserver PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)
