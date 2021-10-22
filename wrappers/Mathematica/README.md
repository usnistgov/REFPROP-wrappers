# Mathematica REFPROP Wrapper

The initial, Mathematica wrapper package was created as an example framework by Wolfram Research under contract to the U.S. Naval Nuclear Laboratory (NNL).  Wolfram Research and NNL agreed to release the wrapper package to the REFPROP community.  Patrick Fourspring facilitated the agreement and the transfer of the package to NIST's wrapper repository. Jeff Henning facilitated the actual transfer of the wrapper package to the `usnistgov/REFPROP-wrappers` repository on GitHub as well as further development to provide compatibility with **REFPROP 10** and conversion to a Mathematica Paclet format that includes full usage documentation through the Mathematica help system.

## Overview

The Wolfram Language (WL) Paclet, **RefpropLink**, provides a diverse range of WL wrapper functions that call the individual public subroutines in REFPROP64.DLL of the local NIST REFPROP installation.  The file `Installation.nb` is a Wolfram Language Notebook that provides the required installation steps.  Opening this file directly in Mathematica, the user can easily follow along with the steps to:

   - Uninstall any and all existing **RefpropLink** paclets if they exist.
   - Install the paclet in the user's local Mathematica environment (only required once for each user)
   - Verify that RefpropLink is properly installed

This instructions file is extremely useful for initial installation and providing examples of loading and using the RefProp wrapper functions.

## Pre-Requisites

   - **NIST REFPROP** 9.1 or later **_must_** be installed to make use of the wrapper functions provided here.  However, **NIST REFPROP 10.x** is recommended.  See [NIST.gov](https://www.nist.gov/srd/refprop) for licensing.
   - **Wolfram Mathematica 12.1** or later (paclet will not install in earlier versions)

      *For **developers**, the following are also required (for now):*

   - The [Eclipse IDE](https://www.eclipse.org/eclipseide/) (any flavor) from Eclipse Foundation (www.eclipse.org)
   - The latest Wolfram Workbench plugin for Eclipse ([instructions here](https://support.wolfram.com/27221))

**NOTE:** Development and testing of this wrapper has been entirely on a Microsoft Windows installation.  This wrapper **_may_** work on a Linux installation, but may require significant modification and testing.  Care has been taken to ensure that the paclet code uses Mathematica's OS-independent file manipulation tools.

------

# Installation from the Git Repository

These installation steps are included in the instructions file, noted above, but are also outlined below for completeness (outside of Mathematica), since the only need to be completed once for each user.

## Steps to install the RefpropLink package

1. Download (and unzip if necessary) this repository. 
 
2. Open the Mathematica notebook `wrappers\Mathematica\Installation.nb` in a fresh instance of Mathematica directly from the downloaded repository.

3. Follow the instructions, evaluating each installation step in the file by pressing `<Shift><Enter>`. 
    
That's it! The `RefpropLink` paclet is now installed and can be used from any Mathematica notebook or script.  In addition, the installation steps automatically create a paclet archive file (.paclet) that can be used directly by other users to install RefpropLink, or it can be placed on a local paclet server for remote installation by users in the organization or community.

Wolfram (both the man and the company) have been hinting of a Public Wolfram Paclet Repository since 2019.  This has not yet come to fruition.  When and if it does, RefpropLink developers can place a .paclet file there to greatly facilitate User Installation for the entire Wolfram Community.

------

# Usage Help

Extensive documentation is provided with the paclet on usage of the RefpropLink functions.  Simply open the Mathematica help system and search for Refprop to open the Overview, Function Guide pages, Tutorials, and Symbol pages for each of the RefpropLink functions.  The majority of the legacy API functions are implemented as well as most of the High-Level API functions introduced in REFPROP 10.

NOTE: The `ALLPROPSdll` routines are not implemented; only `REFPROPdll`.

## Loading RefpropLink from within Mathematica

To call any of the RefProp functions in a Mathematica notebook, RefpropLink must first be loaded from the paclet directory.  In any Mathematica session or script, the **NIST RefProp** functions are evaluated by pressing `<Shift><Enter>` following the command.  To load RefpropLink, evaluate the following command.
   ```Mathematica
   Needs["RefpropLink`"]

   Found NIST REFPROP Version: 10.0.0.2
   ```
This command loads the RefpropLink context if it has not already been loaded and returns the version of NIST REFPROP link found on the machine. Note the back-quote at the end of the context string.

## Fluid Property Calculation Example (High-Level API)

Fluid properties can be calculated using the **_RefProp_[ ]** function (which calls `REFPRRP64dll` in the REFPROP library).  This is a Swiss army knife function that will return one or multiple fluid properties for any of the library fluids at a specified reference state.  The simple example below returns the molar density [D] and enthalpy [H] of carbon dioxide (CO2) at room temperature and pressure (after RefpropLink is loaded as shown above).

   ```Mathematica
   T = 72 °F;
   P = 1 atm;
   RefProp["CO2", "TP", "D H", T, P]

   {D -> 0.0414741 mol/L, H -> 22158.1 J/mol}
   ```
Note the following:
   - A set of unit symbols is provided for easy unit conversions and function inputs and outputs
   - Outputs are provided as a `List` of Mathematica `Quantities` with units applied.  Individual return values can be extracted using standard `List` indexing.
   - For subsequent calls with the same fluid, the first parameter containing the fluid string can be omitted entirely.

The installed documentation contains extensive tutorials and examples on the usage of the full list of REFPROP functions for pure fluids and mixtures.  The [**NIST REFPROP Online DLL Documentation**](https://refprop-docs.readthedocs.io/en/latest/DLL/index.html) should also be consulted for proper usage of the functions and the many function parameters.


------

# RefpropLink Development

Development of integrated documentation has been problematic since the release of Mathematica 12.  Wolfram has provided mature paclet management functions, but has not provided native tools for creating Mathematica 12-styled documentation pages and assembling them.  The existing tool, Wolfram Workbench 3, has not kept up with the newer releases of Mathematica, but does still function and generates passable documentation pages that can be indexed into the help system.  Wolfram has been hinting at creating native Documentation Tools right within Mathematica, but these tools have not yet been released as of Mathematica 12.3.  Until they do, we're stuck with Wolfram Workbench, which is a free plug-in for the Eclipse IDE.

RefpropLink and the associated documentation is developed entirely in Wolfram Workbench.  To contribute to RefpropLink development, one should download the entire [**usnistgov/REFPROP-wrappers**](https://github.com/usnistgov/REFPROP-wrappers) repository from GitHub and import the `wrappers\Mathematica` directory into a project in an installation of Eclipse that includes the Wolfram Workbench 3 plug-in.  RefpropLink is currently developed in Eclipse 2021-09 with Wolfram Workbench version 10.1.1087 and Mathematica 12.1 for current working compatibility.

Contributions to RefpropLink are encouraged through pull requests to the [**usnistgov/REFPROP-wrappers**](https://github.com/usnistgov/REFPROP-wrappers) GitHub repository.  Documentation should be rebuilt consistent with any changes to the code and updating the version number in the `PacletInfo.m` file.

> **Developer Note:** There is some Git warnings regarding LF and CRLF line endings when placing Mathematica notebooks and scripts in git repos and pulling and pushing them from GitHub.  Limited testing demonstrates that Mathematica is impervious to the line endings (see [Wolfram Notebook Technology](https://www.wolfram.com/technologies/nb/)) and these Git warnings can be ignored.  If issues occure in the Eclipse editor when importing the project, this may be the cause and can easily be corrected. 