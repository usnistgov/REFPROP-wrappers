# Welcome to the information about the wrappers of NIST REFPROP

## Why do we need wrappers?

* NIST REFPROP is FORTRAN code compiled into a DLL (dynamically linked library).
    * Precompiled DLL (dynamically linked library) are available on windows (``REFPROP.DLL`` for 32-bit applications, and ``REFPRP64.DLL`` for 64-bit applications).  
    * For linux, Mac OSX, and other platforms, the user is invited to compile their own shared library from source using the CMake-based build system at https://github.com/usnistgov/REFPROP-cmake
* No universal simple way to call a dynamic library in other environments like Excel, MATLAB, LabView, python, etc.

## What wrappers are available?

Hint: click on the name to be taken to the appropriate wrapper code

* [Python](wrappers/python)
* [MATLAB](wrappers/MATLAB)
* [Excel](wrappers/Excel)
* [Labview](wrappers/Labview)
* [C/C++](wrappers/C_CPP)
* [C#](wrappers/Csharp)
* [VB, VB.NET](http://trc.nist.gov/refprop/LINKING/Linking.htm#NETApplications)
* [Mathcad](wrappers/Mathcad)

## Contributing

Pull requests welcome!  You will get credit for your contribution, and the thanks of the REFPROP user community.

