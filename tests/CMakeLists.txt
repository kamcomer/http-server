# Find check library
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)

# Add subdirectories
add_subdirectory(unit)
