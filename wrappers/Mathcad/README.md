# Mathcad REFPROP Wrapper

The initial Mathcad wrapper was created by Jeff Henning of the Naval Nuclear Laboratory (NNL) and provided to the REFPROP community here with agreement from NNL.  Patrick Fourspring (NNL) facilitated the agreement, which also included release of a Mathematica (Wolfram Language) wrapper package, also located in this repository.

## Overview

The Mathcad wrapper provides a diverse range of add-in functions (all beginning with *rp_*) that make subsequent calls to the NIST REFPROP DLL.  These wrapper functions will retrieve the requested fluid properties for a specific fluid material and state point (based on the input parameters).  Utility functions are also provided for retrieving and setting specific NIST REFPROP parameters and version information.

This repository contains the source for building the Mathcad Prime and/or Legacy Mathcad wrappers for the NIST REFPROP materials library using a community or professional version of Microsoft Visual Studio with Visual C++.   This wrapper code, starting with version 2.0, uses [REFPROP-headers](https://github.com/CoolProp/REFPROP-headers) to make calls directly to the Legacy API functions in REFPROP.dll (or REFPRP64.dll) when NIST REFPROP 9.1 or later is installed.  Version 2.1 contains significant enhancements to implement the High-Level API when NIST REFPROP 10.0 or later is installed.  PTC Mathcad only runs on the MS Windows platform. 

> **NOTE:** _The precompiled Custom Function DLL is now available [below](#installing-the-pre-compiled-dll-for-mathcad-prime)_!

------

## Prerequisites

The prerequisites for building and running the REFPROP add-in for Mathcad are:  
  
1. Mathcad Prime (tested up to version 8.0.0.0) (_see NOTE_)
2. NIST REFPROP 9.1 or later ( _available from [NIST](https://www.nist.gov/srd/refprop)_ ), but REFPROP 10 is preferred
3. MS Visual Studio 2015 or later (_Professional or Community editions_)  
    **_<u>Only needed if Compiling (Buiding) the Add-In DLL</u>_**

If downloading the pre-compiled windows DLL, the third requirement above is not needed.

> NOTE: While Legacy Mathcad 15 is no longer supported or distributed by PTC, 32-bit versions of this library can still be built for those who still have perpetual licenses of Legacy Mathcad (15 or earlier). However, compilation for Mathcad 15 is no longer being tested nor supported or  developed.  

------

# Installing the Pre-compiled DLL for Mathcad Prime

If you just want to download and run the latest wrapper version in Mathcad Prime:

1. Download the latest DLL file by clicking on the file name here [PrimeREFPROPWrapper.dll](https://nist-srd.s3.amazonaws.com/SRD23/MathCAD/PrimeREFPROPWrapper.dll).
2. Once you have [PrimeREFPROPWrapper.DLL](https://nist-srd.s3.amazonaws.com/SRD23/MathCAD/PrimeREFPROPWrapper.dll) on your computer, copy it to the Mathcad Prime Custom Functions folder, usually located at: `C:\Program Files\PTC\Mathcad Prime 8.0.0.0\Custom Functions`  
    - Modify `8.0.0.0` if you have a different version, say `7.0.0.0`.  
    - This may require admin access to your C:\ drive on hardened Windows systems.
3. Download the the add-in documentation ([Mathcad Prime PDF User's Guide](./PrimeDocs/PrimeManual.pdf)) for reference; as well as referring to the on-line [NIST REFPROP DLL documentation](https://refprop-docs.readthedocs.io/en/latest/DLL/index.html) or your local PDF version installed with **NIST REFPROP**.

------

# Building the Mathcad Custom Function DLL

## Building the **Mathcad Prime** DLL

1. Make sure that Mathcad Prime is not running
2. Go to the /buildPrime directory
3. Use the `RefPropPrimeWrapper.sln` file to open the solution in VisualStudio 2015 or later.  This will open the PrimeREFPROPwrapper project. 
4. Make sure that the **build** is set to "**Prime7**" or "**Prime8**" and target is "x64" for a 64-bit build.  This will ensure that the correct Mathcad libraries are used and the compiled DLL is placed in the appropriate Mathcad folders. (_There is also a Prime6 configuration, but Mathcad Prime 6.0 is now obsolete._)
5. Select **Build | Rebuild PrimeREFPROPwrapper**
6. This will build the add-in DLL and it will be placed in the Mathcad Prime installation directory under **/Custom Functions**
7. There is no interface file facility for Custom Functions in Prime at this time.

If you have a newer version than **Mathcad Prime 8.0.0.0**, you will need to modify the VS project settings under: 
- C++ > General > Additional Include Directories
- Linker > General > Additional Library Directories
- Build Events > Post-Build Events  

to change the applicable `PTC\Mathcad Prime 8.0.0.0` path strings to the appropriate version for your system.

## Building the **Legacy Mathcad 15** (unsupported) add-in DLL

One can also use the same code to [build a 32-bit add-in for Legacy Mathcad](BuildLegacyMathcadDLL.md), however, that functionality is no longer tested or supported.  Use at your own risk.

------

# Using the RefProp Add-in in Mathcad Prime

With the Mathcad wrapper DLL compiled and copied to the appropriate Mathcad directories, the RefProp functions, which all begin with the prefix "rp_", will be directly available for use in Mathcad.  Unfortunately, Mathcad Prime does not yet have the facility to list custom functions as of Mathcad Prime 8.0.  A User's Guide for Mathcad Prime can be found in PDF format in the PrimeDocs folder of this repository.
  
In addition, the file `Units\RefProp_Units.mcdx` is available in Mathcad Prime format.  By including this reference worksheet in your own worksheets, all of the RefProp functions will be available through pre-defined user functions that provide:
- Function names in standard mathematical, thermodynamic notation, including Greek symbols,
- Unit handling to convert any parameters to the correct units and return values with units already applied, and
- Automatic handling of the Output array, single value returns, and string results from the High-Level API functions available with REFPROP 10.

See the add-in documentation ([Mathcad Prime PDF User's Guide](./PrimeDocs/PrimeManual.pdf)) as well as the [NIST REFPROP DLL documentation](https://refprop-docs.readthedocs.io/en/latest/DLL/index.html) for instructions on using these functions.  As of version 2.1 of this add-in, functions are provided for both the

* [High-Level API](High-LevelAPI.md) function calls from REFPROP 10 or later, and  

* [Legacy API](LegacyAPI.md) function calls available with REFPROP 9.1.1 and later.

------
## Possible Future Enhancements

1. Add a CMake build system to dynamically create the \build15 and \buildPrime directories, taking Mathcad and VS versions as parameters for a simpler build.
2. Use CMake `find_package()` functionality to automatically find the latest version of Mathcad on the user's machine, simplifying path setting to includes/libraries and the target Custom Functions directory.

------

## A Note on Cloning and Transferring this Git Repository

In order to properly transfer the Mathcad Handbook and Units directories, which consists of many Mathcad worksheets with the `.xmcd` and `.mcdx` file extensions, you will need a `.gitattributes` file (provided in this download) with the following lines in it:  

    *.xmcd binary
    *.xmcdz binary
    *.mdcx binary

While the Mathcad 15 Handbook worksheets are XML and not binary, this attribute will keep git from messing with the CRLF on the line endings in the XML worksheets, corrupting them and making them unusable when committed or transferred to and from a Git remote repository.	

