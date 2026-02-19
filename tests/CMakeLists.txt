set(TEST_SOURCES
    test_main.c
    test_request.c
    test_response.c
    test_util.c
)

# Get all source files except main.c
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

# Find check library
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)

# Add library directories
link_directories(${CHECK_LIBRARY_DIRS})

add_executable(tests ${TEST_SOURCES} ${SOURCES})

target_link_libraries(tests ${CHECK_LIBRARIES})
target_compile_options(tests PRIVATE ${CHECK_CFLAGS_OTHER})
target_link_options(tests PRIVATE ${CHECK_LDFLAGS})

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CHECK_INCLUDE_DIRS}
)
