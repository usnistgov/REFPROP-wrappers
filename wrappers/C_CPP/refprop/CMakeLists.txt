cmake_minimum_required(VERSION 3.26)
project(Refprop)

# Set C++ standard version
set(CMAKE_CXX_STANDARD 11)
set(MAKE_CXX_STANDARD_REQUIRED ON)

# ---- Add manyso ----------------
message(STATUS "Adding manyso")
set(MANYSO_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/manyso")
include(FetchContent)
if(EXISTS ${MANYSO_SOURCE_DIR})
  # Use previously downloaded repo
  FetchContent_Declare(
    manyso
	SOURCE_DIR ${MANYSO_SOURCE_DIR}
  )
else()
  # Download repo
  message(STATUS "Fetching manyso library")
  FetchContent_Declare(
    manyso
    GIT_REPOSITORY https://github.com/usnistgov/manyso
    GIT_TAG 8ad7331582143923ee2c04d522d3bc0c1005794d
    GIT_SHALLOW TRUE                    # TRUE=retrieve only latest commit in tree
    SOURCE_DIR ${MANYSO_SOURCE_DIR}     # local destination turned source directory
  )
endif()
FetchContent_Populate(manyso)
include_directories("${MANYSO_SOURCE_DIR}/include")

# ---- Add MemoryModule ----------------
set(MEMORYMODULE_DIR "${MANYSO_SOURCE_DIR}/externals/MemoryModule")
add_library (MemoryModule STATIC
  ${MEMORYMODULE_DIR}/MemoryModule.c
  ${MEMORYMODULE_DIR}/MemoryModule.h
)
set_target_properties(MemoryModule PROPERTIES LINKER_LANGUAGE C)
target_include_directories(MemoryModule PUBLIC
  ${MEMORYMODULE_DIR}
  "${MANYSO_SOURCE_DIR}/include"
)
target_compile_options(MemoryModule PUBLIC
  $<$<CONFIG:Debug>:/MDd>
  $<$<CONFIG:Release>:/MD>
)

# ---- Add REFPROP v10 ----------------
message(STATUS "Adding REFPROP v10")
string(REPLACE "\\" "/" REFPROP10_DIR $ENV{RPprefix})   # get REFPROP path but replace any backslashes with forward slashes
set(REFPROP10_DLL "REFPRP64.DLL")
set(REFPROP10_LIB "REFPRP64.LIB")
add_library(refprop10_lib SHARED IMPORTED GLOBAL)
set_target_properties(refprop10_lib PROPERTIES
  IMPORTED_LOCATION_DEBUG ${REFPROP10_DIR}/${REFPROP10_DLL}
  IMPORTED_LOCATION_RELEASE ${REFPROP10_DIR}/${REFPROP10_DLL}
  IMPORTED_IMPLIB_DEBUG ${REFPROP10_DIR}/${REFPROP10_LIB}
  IMPORTED_IMPLIB_RELEASE ${REFPROP10_DIR}/${REFPROP10_LIB}
)
add_compile_definitions(REFPROP10_PATH="${REFPROP10_DIR}/${REFPROP10_DLL}")	# pass value to source code

# Specify header and source directories
set(HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Specify library
file(GLOB_RECURSE HEADER_FILES
  "${HEADER_DIR}/*.h"
  "${HEADER_DIR}/*.hpp")
file(GLOB SOURCE_FILES
  "${SOURCE_DIR}/*.cpp")
add_library(refprop ${HEADER_FILES} ${SOURCE_FILES})

# Add subdirectories
target_include_directories(refprop PUBLIC
  ${HEADER_DIR}
  ${HEADER_DIR}/${PROJECT_NAME}
)

# Pass root directory to C++ source code
add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")

target_link_libraries(refprop PUBLIC
  MemoryModule
  refprop10_lib
)
