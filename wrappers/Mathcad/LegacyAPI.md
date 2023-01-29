# Legacy API Calls

The Legacy API calls were introduced in NIST REFPROP 9.1.1 through a single DLL and wrappers for these functions are introduced in the RefProp Mathcad Add-in version 2.0.  These property functions follow a standard nomenclature with

* All functions beginning with the characters rp_ to avoid conflict with any other functions
* The prefix is followed by the property being calculated (e.g. rho, h, s, cp, w, etc.)
* The property symbol is followed by a phase character if along the saturated liquid (f) curve or saturated vapor (g) curve.
* The phase character  is followed by a single state point property code (e.g. t or p) or a state point pair if no phase character is present for liquid, vapor, or two-phase conditions (e.g. tp, ts, th, ps, ph, etc.)

For example, the calculation of enthalpy, _h_, as a function of temperature and pressure

Listing of the many individual Legacy API Custom Function wrappers is not provided here, but can be found in the [RefProp Legacy API Help](https://raw.githack.com/usnistgov/REFPROP-wrappers/master/wrappers/Mathcad/doc/HELP_EN/Refprop/RefpropHelp.htm)  

------
## Unit Wrapper Functions

By far, the best way to use the Legacy API functions is through the user functions provided in the `Refprop_units.mcdx` include worksheet.  These functions have the following benefits:
* Function names are more intuitive and follow standard thermodynamic nomenclature, including greek symbols
* These functions handle input parameters with Mathcad units and apply units to the return values automatically.

Users should examine the `Refprop_units.mcdx` include worksheet for the further details of the functions and their use.

------
## Examples

A brief example of the usage of the Legacy API Custom functions through the user functions provided in the `Refprop_units.mcdx` include worksheet are shown below.

![Example Screenshot](img/Screenshot-Units.png "Units Screenshot") 

