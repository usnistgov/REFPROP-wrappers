# Mathcad Prime REFPROP Wrapper

This directory contains the Microsoft Visual Studio 2015 solution files for building the **NIST RefProp** add-in for **Mathcad Prime**.  It is targeted for **Prime 6.0.0.0**, but has been compiled and tested on versions 3 through 6.

## Building the Mathcad Prime Custom Function DLL

To build the Mathcad Prime DLL,

1. Make sure that Mathcad Prime is not running
2. Go to the /buildPrime directory
3. Use the `RefPropPrimeWrapper.sln` file to open the solution in VisualStudio 2015 or later.  This will open the PrimeREFPROPwrapper project. 
4. Make sure that the build is set to "Release" and target is "x64" for a 64-bit build.
5. Select **Build | Rebuild PrimeREFPROPwrapper**
6. This will build the add-in DLL and it will be placed in the Mathcad Prime installation directory under **/Custom Functions**
7. There is no interface file facility for Custom Functions in Prime at this time.

If you have a different version than **Mathcad Prime 6.0.0.0**, you will need to modify the VS project settings under:
    - C++ > General > Additional Include Directories
    - Linker > General > Additional Library Directories
    - Build Events > Post-Build Events
to change the applicable `PTC\Mathcad Prime 6.0.0.0` path strings to the appropriate version for your system.



