# Mathematica REFPROP Wrapper
Initial wrapper package was created by Wolfram Research under contract to the U.S. Naval Nuclear Laboratory (NNL). Wolfram Research and NNL agreed to provide the wrapper package to the REFPROP community. Patrick Fourspring facilated the agreement and the transfer. Jeff Henning facilated the transfer of the wrapper package to GitHub.

Overview
The Wolfram Language (WL) wrappers provide a diverse range of example WL functions calling individual subroutines of the REFPROP Fortran code via the REFPROP DLL. The file RefpropLinkInstructions describes and provivdes the required initialization functions and the example WL functions to call individual REFPROP subroutines. Two approaches are provided to intialize the connection between WL and REFPROP subroutines. The programmatic approach is the most useful.

To call any of the REFPROP subroutines, a WL session must be initialized with the following commands as explained in the RefpropLinkInstructions:

Needs["RefpropLink`"]
(*For single fluid set fluidComponents as {"nameoflfuid.fld"}*)
(*For \
mixtures, give the components of the mixture as a list of string. \
Example: {"fluid1.fld","fluid2.fld","fluid3.fld"}*)

dllpath = "C:\\Program Files (x86)\\REFPROP\\REFPRP64.DLL";
fluidpath = "C:\\Program Files (x86)\\REFPROP\\fluids";
fluidComponents = {"nitrogen.fld"};
numberofComp = 1;
refstate = "DEF";
ierr = 0;
herr = StringJoin[Table[" ", {256}]];

(*A return value of 0 indicates successful setup*)

setup[dllpath, fluidpath, fluidComponents, numberofComp, refstate, \
ierr, herr]

After this sequence of commands (functions) the initilization is complete if a '0' is returned.
Then, the wrapper files contain examples of WL functions for the following subroutines:

* **info** - provides fluid constants for specified component
* **satp** - Calculate saturation properties at a given p
* **tprho** - iterate for density as a function of temperature, pressure, and composition for a specified phase
* **tpflsh** - General flash subroutine 
* **therm** - compute thermal quantities as a function of temperature, density, and compositions using core functions (Helmholtz free energy, ideal gas heat capacity and various derivatives and integrals)
* **press** - compute pressure as a function of temperature, density, and composition using core functions
* **fgcty** - compute fugacity for each of the nc components of a mixture by numerical differentiation (using central differences) of the dimensionless residual Helmholtz energy
* **virb** - compute second acoustic virial coefficient as a function of temperature and composition
* **virc** - compute third acoustic virial coefficient as a function of temperature and composition
* **dpdd** - compute partial derivative of pressure w.r.t. density at constant temperature as a function of temperature, density, and composition
* **dpdd2** - compute second partial derivative of pressure w.r.t. density at const temperature as a function of temperature, density, and composition
* **dpdt** - compute partial derivative of pressure w.r.t. temperature at constant density as a function of temperature, density, and composition
* **dhdl** - Compute partial derivatives of enthalpy w.r.t. t, p, or rho at constant t, p, or rho as a function of temperature, density, and composition
* **surft** - Compute surface tension
* **trnprp** - Calculate viscosity (eta) and thermal conductivity (tcx)
* **tdflsh** - General property calculation with inputs of t,d,x
* **pdflsh** - General property calculation with inputs of p,h,x
* **meltt** - Calculate melting pressure
* **dielec** - Calculate dielectric constant
* **RFPI** - Brings up a an interface to run set up
* **checkErrorCodes** - Checks error codes during set up

The `RefpropLinkInstructions.nb` notebook can be run to demonstrate the wrapper functions between REFROP and WL.
