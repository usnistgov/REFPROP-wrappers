# REFPROP wrapper for C/C++

Installation
------------
**NOTE**: for Linux and macOS, you must compile REFPROP yourself, see: [REFPROP-cmake](https://github.com/usnistgov/REFPROP-cmake)

1. Follow the conventional installation instructions for REFPROP.
2. For Linux and macOS, compile REFPROP via [REFPROP-cmake](https://github.com/usnistgov/REFPROP-cmake).
3. Ensure that the system environment variable `RPprefix` is set to the location of the REFPROP installation (e.g., `C:\Program Files (x86)\REFPROP`).
4. Install CMake, version 3.26 or newer. (https://cmake.org/download/).
5. Install Git, version 2.0 or newer. (https://git-scm.com/downloads).
6. For Windows, install the Microsoft Visual C++ compiler (MSVC). <br>(**Note:** This compiler is included with the Visual Studio IDE and this step can be skipped if that is already installed.)
   1. Go to the Visual Studio download page (https://visualstudio.microsoft.com/downloads/).
   2. Under "Tools for Visual Studio", download "Build Tools for Visual Studio 20XX".
   3. Launch the installer and select the "C++ build tools" workload, ensuring that the MSVC compiler, Windows SDK and C++ Standard Library are included.
   4. Complete the installation.
7. Download the top-level `CMakeLists.txt` file into a directory that will be used for building the wrapper and associated code.
8. Run CMake from this directory to generate the build files.
   1. For Visual Studio on Windows, this can be accomplished by creating and running the following batch file (e.g., `build.bat`).
      ```
      rmdir /Q/S build
      mkdir build
      cd build
   
      cmake -G "Visual Studio 17 2022" -DCMAKE_CONFIGURATION_TYPES="Debug;Release" ..
   
      pause
      ```
   2. This will download the files in `/refprop` as part of its build process.
9. Open the `main.cpp` file in the `src` directory for example code on how to call various REFPROP functions.


Alternative
---
REFPROP can also be called using the header defining the library at: [REFPROP-headers](https://github.com/CoolProp/REFPROP-headers). This header includes functions for runtime loading of the DLL (shared library). Present there are also examples of how to use it and a CMake-based build system. This is an older approach that is not recommended for new projects.

Help
-----
Please file an issue at https://github.com/usnistgov/REFPROP-issues and we will get back to you as quickly as possible. Please don't email NIST staff directly, the only exception being if you have proprietary code that cannot be shared publicly.