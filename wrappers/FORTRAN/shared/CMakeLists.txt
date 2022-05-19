cmake_minimum_required (VERSION 3.2)

project (main)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../externals/REFPROP-cmake" "REFPROP")

# Define the executable
add_executable (main "${CMAKE_CURRENT_SOURCE_DIR}/TEST.FOR")

# Link the bits together
target_link_libraries (main REFPRP64)