*************************
REFPROP wrapper for C/C++
*************************

Brought to you by Ian Bell, NIST, 2016

Help!
-----

Please file an issue at https://github.com/usnistgov/REFPROP-issues and we will get back to you as quickly as possible.  Please don't email NIST staff directly, the only exception being if you have proprietary code that cannot be shared publicly.

Installation
------------

Follow the conventional installation instructions for REFPROP.  If you need to compile REFPROP yourself, see: [REFPROP-cmake](https://github.com/usnistgov/REFPROP-cmake)

Use
---

Your best bet for calling REFPROP from C/C++ is to use the header defining the library and how to hook the library hosted at: [REFPROP-headers](https://github.com/CoolProp/REFPROP-headers) .  This header includes functions for runtime loading of the DLL (shared library), and some examples of how to use it, and integration with CMake.