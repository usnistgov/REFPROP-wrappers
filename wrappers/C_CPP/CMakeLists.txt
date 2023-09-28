cmake_minimum_required(VERSION 3.26)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)
set(MAKE_CXX_STANDARD_REQUIRED ON)

# ---- Download REFPROP C/C++ wrappers ----------------
set(REPO_URL "https://github.com/usnistgov/REFPROP-wrappers.git")
set(REPO_BRANCH "master")
set(REPO_SUBFOLDER "wrappers/C_CPP/refprop")
set(LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/refprop")
include(FetchContent)
if(EXISTS ${LOCAL_DIR})
  # Use previously downloaded repo
  FetchContent_Declare(
    refprop
	SOURCE_DIR ${LOCAL_DIR}
  )
else()
  # Download repo
  message(STATUS "Downloading REFPROP C++ wrapper")
  FetchContent_Declare(
    refprop
    GIT_REPOSITORY ${REPO_URL}
	GIT_TAG ${REPO_BRANCH}
    GIT_SHALLOW TRUE                    # TRUE=retrieve only latest commit in tree
    SOURCE_DIR ${LOCAL_DIR}             # local destination turned source directory
  )
endif()
FetchContent_Populate(refprop)
add_subdirectory(${LOCAL_DIR}/${REPO_SUBFOLDER})

# Copy example include and source directories to top-level
file(COPY "${LOCAL_DIR}/${REPO_SUBFOLDER}/../include" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
file(COPY "${LOCAL_DIR}/${REPO_SUBFOLDER}/../src" DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

# Add executable
set(HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${HEADER_DIR}/${PROJECT_NAME})
file(GLOB_RECURSE HEADER_FILES "${HEADER_DIR}/*.h")
set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_executable(myproject ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(myproject refprop)

# Set output and build directories
set_target_properties(myproject PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

# ---- Configure Microsoft Visual Studio ----------------
if(MSVC)
  # Choose appropriate version
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
  
  # Set the startup project for Visual Studio
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT myproject)
endif()
