# Mathematica REFPROP Wrapper

The initial, Mathematica wrapper package was created as an example framework by Wolfram Research under contract to the U.S. Naval Nuclear Laboratory (NNL).  Wolfram Research and NNL agreed to release the wrapper package to the REFPROP community.  Patrick Fourspring facilitated the agreement and the transfer of the package to NIST's wrapper repository. Jeff Henning facilitated the actual transfer of the wrapper package to the `usnistgov/REFPROP-wrappers` repository on GitHub as well as further development to provide compatibility with **REFPROP 10** and conversion to a Mathematica Paclet format that includes full usage documentation through the Mathematica help system.

## What's New

Version 1.1 of RepropLink has been completely restructured for modern Paclet development in Mathematica 13.  While the functionality has not changed (other than some minor bug fixes), there are significant structural changes to this repository from the original version.

1. The finalized directory structure for *Paclets* introduced in Mathematica 13 is used.
2. The **PacletTools** and **DocumentationTools** are used to build the *Paclet*
3. The third-party Eclipse program and Wolfram Workbench extension **_are no longer needed_**.  This means that anyone with Mathematica 13+ can build the paclet, including help documentation and tool palette.
4. Help documentation has been completely reformatted using the v13 documentation templates to provide more consistent look and feel and full with the rest of the Wolfram Documentation Center.
5. With complete integration of the documentation pages into the Wolfram Documentation Center, the hover buttons for usage and help, as well as the ability to press **F1** on any function to bring up help, are now fully functional.

## Overview

The Wolfram Language (WL) Paclet, **_RefpropLink_**, provides a diverse range of WL wrapper functions that call the individual public subroutines in REFPROP64.DLL of the local NIST REFPROP installation.  The file `Installation.nb` is a Wolfram Language Notebook that provides the required installation steps.  Opening this file directly in Mathematica, the user can easily follow along with the steps to:

   - Uninstall any and all existing **_RefpropLink_** paclets if they exist.
   - Install the paclet in the user's local Mathematica environment (only required once for each user)
   - Verify that **_RefpropLink_** is properly installed

This instructions file is extremely useful for initial installation and providing examples of loading and using the RefProp wrapper functions.

## Prerequisites (v1.1)

   - **NIST REFPROP** 9.1 or later **_must_** be installed to make use of the wrapper functions provided here.  However, **NIST REFPROP 10.x** is recommended.  See [NIST.gov](https://www.nist.gov/srd/refprop) for licensing.  REFPROP should be installed either:
       1. In the default location `C:\Program Files(x86)\REFPROP`,
       2. In a location specified by the `RPprefix` environment variable, or
       3. In a location selected using the `SetDLL[]` function.

   - **Mathematica 13.x** is required to build and install using the new Paclet and Documentation Tools introduced in Mathematica 13.0.  Doe to some early release issues, **Mathematica 13.2** or greater is recommended for the most complete formatting and indexing of the RefpropLink documentation pages to the Wolfram Documentation Center standards.  

   - **Wolfram Mathematica 12.x** can likely be used to install the .paclet file directly from a paclet server (although not tested); either the Wolfram Paclet Repository (plans in the works) or a private, local paclet.  **Mathematica 12.x**, however, does not have the full paclet build tools necessary to build the paclet from this repository using solely the Mathematica Paclet Tools.  
   
**NOTE:** Previous versions (1.0.x) were built for Mathematica 12.x using the Wolfram Workbench add-in to Eclipse.  However, with the superior build tools in Mathematica 13, this third party method is no longer supported or recommended.

**NOTE:** Development and testing of this wrapper has been entirely on a Microsoft Windows installation using Mathematica 13 and REFPROP 10.  This wrapper **_should_** work on a Linux installation, but will require testing and may require some code modification.  Care has been taken to ensure that the paclet code uses Mathematica's OS-independent file manipulation tools.

------

# Installation from the Git Repository

These installation steps are included in the `Installation.nb` notebook, as noted above, but are also outlined below for completeness (outside of Mathematica), since they only need to be completed once for each user.

## Steps to install the RefpropLink package

1. Download (and unzip if necessary) this repository. 
 
2. Open the Mathematica notebook `wrappers\Mathematica\RefpropLink\Installation.nb` in a fresh instance of Mathematica directly from the downloaded repository.

3. Follow the instructions, evaluating each installation step in the file by pressing `<Shift><Enter>`. 
    
That's it! The **_RefpropLink_** paclet will be built and installed on the local user account and can be used from any Mathematica notebook or script.  In addition, the installation steps automatically create a paclet file (`/build/RefpropLink-x.x.x.paclet`) that can be used directly by other users to install **_RefpropLink_**, or it can be placed on a local paclet server for remote installation by users in the organization or community.

Efforts are underway to provide the latest Paclet file on the public Wolfram Paclet Repository.  This will allow users to install **_RefpropLink_** directly from the Wolfram Paclet Repository, without the need to download these files from GitHub and rebuild the repository; greatly facilitating user installation for the entire Wolfram Community.

------

# Usage Help

Extensive documentation is provided with the paclet on usage of the **_RefpropLink_** functions.  Simply open the Mathematica help system and search for Refprop to open the Overview, Function Guide pages, Tutorials, and Symbol pages for each of the **_RefpropLink_** functions.  The majority of the legacy API functions are implemented as well as most of the High-Level API functions introduced in REFPROP 10.

NOTE: The `ALLPROPSdll` routines are not implemented; only `REFPROPdll`.

## Loading RefpropLink from within Mathematica

To call any of the RefProp functions in a Mathematica notebook, **_RefpropLink_** must first be loaded from the paclet directory.  In any Mathematica session or script, the **NIST RefProp** functions are evaluated by pressing `<Shift><Enter>` following the command.  To load **_RefpropLink_**, evaluate the following command.
   ```Mathematica
   Needs["RefpropLink`"]

   Found NIST REFPROP Version: 10.0.0.2
   ```
> _Note the back-quote at the end of the context string._

This command loads the **_RefpropLink_** context if it has not already been loaded and returns the version of NIST REFPROP link found on the machine (which may vary from the example above).

## Fluid Property Calculation Example (High-Level API)

Fluid properties can be calculated using the **_RefProp_[ ]** function (which calls `REFPRRP64dll` in the REFPROP library).  This is a Swiss army knife function that will return one or multiple fluid properties for any of the library fluids at a specified reference state.  The simple example below returns the molar density [D] and enthalpy [H] of carbon dioxide (CO2) at room temperature and pressure (after **_RefpropLink_** is loaded as shown above).

   ```Mathematica
   T = Quantity(72.0,"DegreesFahrenheit");  (* Must use Quantity function for affine temps*)
   P = 1 atm;
   RefProp["CO2", "TP", "D H", T, P]

   {D -> 0.0414741 mol/L, H -> 22158.1 J/mol}
   ```
Note the following:
   - A set of unit symbols is provided for easy unit conversions and function inputs and outputs. However, affine temperature units [°F and °C] must be set with the Quantity function as of Mathematica 13.2 .
   - Outputs are provided as a `List` of Mathematica `Quantities` with units applied.  Individual return values can be extracted using standard `List` indexing.
   - For subsequent calls with the same fluid, the first parameter containing the fluid string can be omitted entirely.

The installed documentation contains extensive tutorials and examples on the usage of the full list of REFPROP functions for pure fluids and mixtures.  The [**NIST REFPROP Online DLL Documentation**](https://refprop-docs.readthedocs.io/en/latest/DLL/index.html) should also be consulted for proper usage of the functions and the many function parameters.

------

# Temperature Units

As of Mathematica 13.2, there has been a fundamental change (correctly so) in the way that affine temperature units are handled.  Values represented in °F or °C can no longer be multiplied by a scalar value.  For example, prior to 13.2:

```WL
In[1]  T = Quantity[20.0,"DegreesFahrenheit"]
Out[1] 20. °F

In[2]  T2 = 2.0*T                             (*Evaluate numerically*)
Out[2] 40. °F
```

Now, however, after version 13.2:

```WL
In[1]  T = Quantity[20.,"DegreesFahrenheit"]
Out[1] 20. °F

In[2]  T2 = 2.0*T                             (*Evaluate numerically*)
Out[2] 532.967 K

In[3]  UnitConvert[ T2, "DegreesFahrenheit"]  (* Convert to °F)
Out[3] 499.67 °F                              (* Not 40 °F          *)
```

This is actually the correct behavior, since the assigned temperature quantity of 20 °F should immediately be converted to an absolute temperature of 266.483 K.  Multiplying this number by 2 yields 532.967 K (or 499.67 °F).

RefpropLink took advantage of the earlier behavior and defined two unit variables, °F and °C, that could be used to set units more simply, like:

```WL
In[1]  T1 = 72 °F;    (* where °F = Quantity[1.0,"DegreesFahrenheit] *)
       T2 = 22 °C;    (* where °C = Quantity[1.0,"DegreesCelsius *)
```
Because of the change in affine unit handling in 13.2, this no longer works.  The first assignment is multiplying (1 °F = 255.928 K) by 72.0.  The second is multiplying (1 °C = 274.15 K) by 22.0.  This gives very unintended results.

```WL
In[1]  T1 = 72. °F
Out[1] 5118.56 K

In[2]  T2 = 22. °C
Out[2] 6031.3 K
```
This is "correct" behavior for unit multiplication, even if unexpected.  So, following Mathematica 13.2, affine units should be set only using the `Quantity[]` function.

```WL
In[1]  T1 = Quantity[72.,"DegreesFahrenheit"]
Out[1] 72. °F

In[1]  T2 = Quantity[22.,"DegreesCelsius"]
Out[1] 22. °C
```
The RefpropLink defined unit variables for all other multiplicative units can still be used as before.

------

# RefpropLink Development

Development of integrated documentation has been problematic since the release of Mathematica 12 and used to require Eclipse and the Wolfram Workbench 3 plug-in. Wolfram has created native Documentation Tools right within Mathematica 13, and updated the Paclet Tools and Documentation Tools in version 13.2.

**_RefpropLink_** and the associated documentation is developed now entirely in Mathematica 13.x ( _see [**Prerequisites**](#prerequisites)_ ).  To contribute to **_RefpropLink_** development, one should clone the entire [**usnistgov/REFPROP-wrappers**](https://github.com/usnistgov/REFPROP-wrappers) repository from GitHub and work directly in the `wrappers\Mathematica` directory under a local development branch.  **_RefpropLink_** is currently developed in Mathematica 13.2, and is expected to be compatible with subsequent versions.

Contributions to **_RefpropLink_** are encouraged through pull requests to the [**usnistgov/REFPROP-wrappers**](https://github.com/usnistgov/REFPROP-wrappers) GitHub repository.  Documentation should be rebuilt and tested for consistency with any changes to the code/documentation as well as updating the version number in the `PacletInfo.m` file.

> **Developer Note:** There are some Git warnings regarding LF and CRLF line endings when placing Mathematica notebooks and scripts in git repos and pulling and pushing them from GitHub.  Limited testing demonstrates that Mathematica is impervious to the line endings (see [Wolfram Notebook Technology](https://www.wolfram.com/technologies/nb/)) and these Git warnings can be ignored.  If issues occur in the Eclipse editor when importing the project, this may be the cause and can easily be corrected. 