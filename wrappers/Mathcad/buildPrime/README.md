# Mathcad Prime REFPROP Wrapper

This directory contains the Microsoft Visual Studio 2022 solution files for building the **NIST RefProp** add-in for **Mathcad Prime**.  It is targeted for **Prime 10.0.0.0**, but has been compiled and tested on versions 3 through 9.

## Building the Mathcad Prime Custom Function DLL

To build the Mathcad Prime DLL,

1. Make sure that Mathcad Prime is not running  
2. Make sure that the Mathcad Prime Custom Functions directory is user modifiable and not write protected.  
3. Go to the /buildPrime directory in this repository  
4. Use the `RefPropPrimeWrapper.sln` file to open the solution in VisualStudio 2022 or later.  This will open the PrimeREFPROPwrapper project.  

    > NOTE: You can get the latest Community Edition of Visual Studio at: https://visualstudio.microsoft.com/vs/community/ and signing up for free in the Dev Essentials community.
5. Make sure that the build is set to **"Prime10"** (or your version 6 through 10) and target is **"x64"** for a 64-bit build.  This will target your active version of Mathcad Prime and specify where the installation directory and build files are located.  
6. Select **Build | Rebuild PrimeREFPROPwrapper**  
7. This will build the add-in DLL and it will be placed in the configuration specified Mathcad Prime installation directory under **/Custom Functions**  

## Units File
The `Units/RefProp_units.mcdx` include file provides units wrappers for each of the REFPROP functions and makes the add-in much easier to use, prividing  
- Function names using standard thermodynamic nomenclature (i.e. h, s, Cp, ρ, σ, etc.).  
- Handling of input parameters with any appropriate Mathcad units applied.  
- Output of results with Mathcad units applied.
- Better error handling.

## Interface File
Mathcad Prime does not natively include Custom Functions in its Functions look-up menu.  However, this legacy feature can be implemented by installing the [CustFunc](https://github.com/henningjp/CustFunc) add-in and companion Refprop interface file.  This makes inserting raw and unit-wrapped funcitons infinitely easier through an `<F3>` pop-up dialog box that lists all of the functions, their syntax, and a description of each function as well as an insert button to drop the desired function right on the open Mathcad Prime worksheet.  

Go to the [CustFunc](https://github.com/henningjp/CustFunc) Github repository for complete instructions on obtaining and installing the latest release of this add-in. 

## Other/Newer Versions of Mathcad Prime
Build configurations are provided for Mathcad Prime 6 through 10.  If you have a different version than **Mathcad Prime 6.0.0.0** through **10.0.0.0**, you will need to go to the **Release | x64 ** configuration and modify the VS project settings under:  
   - C++ > General > Additional Include Directories
   - Linker > General > Additional Library Directories
   - Build Events > Post-Build Events  

to change the applicable `C:\Program Files\PTC\Mathcad Prime 10.0.0.0\Custom Functions` path to somthing like `C:\Program Files\PTC\Mathcad Prime X.0.0.0\Custom Functions`, replacing `X.0.0.0` with the appropriate version for the target system.



