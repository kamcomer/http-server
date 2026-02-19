# Executable target
add_executable(http-server main.c)

# Link against the core library
target_link_libraries(http-server httpserver)

# Include directories
target_include_directories(http-server PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)
