cmake_minimum_required (VERSION 2.8)
project (REFPROP-wrappers)
find_package (PythonInterp 2.7 REQUIRED)
                         
# Make sure that the default is a RELEASE
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE RELEASE CACHE STRING
      "Choose the type of build, options are: None Debug Release."
      FORCE)
endif ()

if (MSVC)
  # Force visual studio to statically link the c runtime to avoid dependency on MSVCRXXX.dll
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
else()
  # Same story for gcc, clang, etc.
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/externals/REFPROP-headers")

if (REFPROP_EXAMPLE)
    add_executable(Main ${CMAKE_SOURCE_DIR}/test_main.cpp  "${CMAKE_SOURCE_DIR}/REFPROP.cpp")
elseif (REFPROP_SWIG_WRAPPER)
    
    # Must have SWIG
    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})
    
    # Changes in these files should trigger swig to rebuild
    set(SWIG_DEPENDENCIES "${CMAKE_SOURCE_DIR}/REFPROP.h" 
                          "${CMAKE_SOURCE_DIR}/REFPROP.cpp")
    # Default flags needed for all swig builds
    set(DEFAULT_SWIG_FLAGS -I"${CMAKE_CURRENT_BINARY_DIR}" -DCONVENTION "-DEXTERN=extern \"C\"")
    # This is the interface fie that CMake will consume
    set(I_FILE "${CMAKE_SOURCE_DIR}/REFPROP.i")
    # Define which headers the swig wrapper is dependent on
    SET(SWIG_MODULE_REFPROP_EXTRA_DEPS ${SWIG_DEPENDENCIES})
    
    if (SWIG_TARGET_LANG MATCHES "csharp")
        # Nothing to be done here
    elseif(SWIG_TARGET_LANG MATCHES "java")
        find_package(Java REQUIRED)
        find_package(JNI)
        include_directories(${JAVA_INCLUDE_PATH})
        include_directories(${JAVA_INCLUDE_PATH}/win32)
        include_directories(${JAVA_INCLUDE_PATH}/linux)
    elseif(SWIG_TARGET_LANG MATCHES "python")
        FIND_PACKAGE(PythonLibsNew REQUIRED)
        INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
        if (MINGW)
          add_definitions(-DMS_WIN64)
        endif()
        set(MORE_SWIG_LINK_LIBRARIES ${PYTHON_LIBRARIES})
        set(SWIG_OPTIONS "${{SWIG_OPTIONS}}" -module "REFPROP")
    elseif(SWIG_TARGET_LANG MATCHES "perl")
        IF (WIN32 AND MSVC)
            MESSAGE(FATAL_ERROR "Must use MinGW Makefiles generator on windows")
        ENDIF()
        FIND_PACKAGE(PerlLibs REQUIRED)
        INCLUDE_DIRECTORIES(${PERL_INCLUDE_PATH})
        set(MORE_SWIG_LINK_LIBRARIES ${PERL_LIBRARY})
    elseif(SWIG_TARGET_LANG MATCHES "matlab")
        # Nothing to be done here
    elseif(SWIG_TARGET_LANG MATCHES "scilab")
        include_directories("C:\\Program Files\\scilab-5.5.1\\modules\\core\\includes")
        include_directories("C:\\Program Files\\scilab-5.5.1\\modules\\api_scilab\\includes")
        include_directories("C:\\Program Files\\scilab-5.5.1\\modules\\output_stream\\includes")
        include_directories("C:\\Program Files\\scilab-5.5.1\\modules\\mexlib\\includes")
        include_directories("C:\\Program Files\\scilab-5.5.1\\modules\\localization\\includes")
        include_directories("C:\\Program Files\\scilab-5.5.1\\libs\\intl")
        add_definitions(-fpermissive)
    elseif(SWIG_TARGET_LANG MATCHES "octave")
        # N
    elseif(SWIG_TARGET_LANG MATCHES "r")
        IF (WIN32 AND MSVC)
            MESSAGE(FATAL_ERROR "Must use MinGW Makefiles generator on windows")
        ENDIF()
        FIND_PACKAGE(R REQUIRED)
        include_directories(${R_INCLUDE_DIRS})
        link_directories(${R_BIN_OUT})
        set(MORE_SWIG_LINK_LIBRARIES "${R_LIBRARY}")
        if (NOT MSVC)
          set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -m64")
          set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -m64")
        endif()
    elseif(SWIG_TARGET_LANG MATCHES "php")
        execute_process(
            COMMAND php-config --includes
            OUTPUT_VARIABLE php_config_includes
            RESULT_VARIABLE php_config_failed
        )
        string(STRIP ${php_config_includes} php_config_includes)
        string(REPLACE "-I" "" PHP_INCLUDES ${php_config_includes})
        SEPARATE_ARGUMENTS(PHP_INCLUDES)
        include_directories(${PHP_INCLUDES})
    else()
        message(FATAL_ERROR "SWIG_TARGET_LANG [${SWIG_TARGET_LANG}] not set/matched")
    endif()
    
    # Set properties before adding module
    SET_SOURCE_FILES_PROPERTIES(${I_FILE} PROPERTIES SWIG_FLAGS "${SWIG_OPTIONS}" CPLUSPLUS ON)
    
    SWIG_ADD_MODULE(REFPROP ${SWIG_TARGET_LANG} ${I_FILE} "${CMAKE_SOURCE_DIR}/REFPROP.cpp")
    SWIG_LINK_LIBRARIES(REFPROP ${MORE_SWIG_LINK_LIBRARIES})
    set(OPTION_SELECTED ON)
    
    if (SWIG_TARGET_LANG MATCHES "r")
        # No lib prefix for the shared library
        set_target_properties(REFPROP PROPERTIES PREFIX "")
    endif()
else()
    message(FATAL_ERROR "An option was not selected (REFPROP_EXAMPLE, REFPROP_SHARED_LIBRARY, etc..)")
endif()