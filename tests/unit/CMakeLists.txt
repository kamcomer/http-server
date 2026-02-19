# Unit test sources
set(UNIT_TEST_SOURCES
    test_main.c
    test_request.c
    test_response.c
    test_util.c
)

# Get library source files
file(GLOB_RECURSE LIB_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.c
)

# Add test executable
add_executable(tests ${UNIT_TEST_SOURCES} ${LIB_SOURCES})

# Link check library
target_link_libraries(tests ${CHECK_LIBRARIES})
target_compile_options(tests PRIVATE ${CHECK_CFLAGS_OTHER})
target_link_options(tests PRIVATE ${CHECK_LDFLAGS})

# Include directories
target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CHECK_INCLUDE_DIRS}
)
