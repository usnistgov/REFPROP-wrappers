# Welcome to the information about the wrappers of NIST REFPROP

## Why do we need wrappers?

* NIST REFPROP is FORTRAN code compiled into a DLL (dynamically linked library).
    * Precompiled DLL (dynamically linked library) are available on windows (``REFPROP.DLL`` for 32-bit applications, and ``REFPRP64.DLL`` for 64-bit applications), and on OSX as well.  
    * The user is invited to compile their own shared library from source using the CMake-based build system at https://github.com/usnistgov/REFPROP-cmake
* No universal simple way to call a dynamic library in other environments like Excel, MATLAB, LabView, python, etc.

## What wrappers are available?

Hint: click on the name to be taken to the appropriate wrapper code

* [Python](wrappers/python)
* [MATLAB](wrappers/MATLAB)
* [Excel](wrappers/Excel)
* Labview (work in progress, for more information, please email damian.lauria@nist.gov)
* [C/C++](wrappers/C_CPP)
* [C#](wrappers/Csharp)
* [VB, VB.NET](http://trc.nist.gov/refprop/LINKING/Linking.htm#NETApplications)

## Contributing

Users are welcome to contribute wrappers to other languages and environments, and efforts are underway to make the full REFPROP API accessible to many more languages through the use of SWIG wrappers

Pull requests welcome!  You will get credit for your contribution, and the thanks of the REFPROP user community.

