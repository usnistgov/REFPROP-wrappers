# Building the Legacy Mathcad 15 (unsupported) add-in DLL

One can also use the same code to build a 32-bit add-in for Legacy Mathcad, however, that functionality is no longer tested or supported.  Use at your own risk.

To build the Mathcad 15 add-in DLL, 

1. Make sure that Mathcad is not running
2. Go to the /build15 directory
3. Use the `MathcadREFPROPwrapper.sln` file to open the solution in VisualStudio 2015 or later.  This will open the MathcadREFPROPwrapper project. 
4. Make sure that the **build** is set to "**Release**" and target is "**Win32**" (Mathcad 15 is 32-bit) 
5. Select **Build | Rebuild MathcadREFPROPwrapper**
6. This will build the add-in DLL and it will be placed in the Mathcad 15 installation directory under **/userfi**
7. The function interface doc (RefProp_EN.xml) will be copied to the Mathcad 15 installation directory under **/doc/funcdoc**
8. The context sensitive help files will also be copied to the Mathcad 15 installation directory under **/doc/HELP_EN/Refprop**
9. The RefProp Handbook (eBook) files will be copied to the Mathcad 15 installation directory under **/Handbook/Refprop**

------

## Using the 32-bit REFPROP Add-in in Legacy Mathcad 15

With the Mathcad wrapper DLL compiled and copied to the appropriate Mathcad directories, the RefProp functions, which all begin with the prefix "rp_", will be directly available for use in Mathcad.  In Legacy Mathcad 15, the functions and usage descriptions will be available in the Insert Function panel under the category **NIST RefProp**. A User's Guide
for Mathcad Prime can be found in PDF format in the PrimeDocs folder of this repository.  
  
In addition, the file `Units\RefProp_Units` is available for **Legacy Mathcad**, although it does not include user functions to support REFPROP 10 enhancements.  By including this reference worksheet in your own worksheets, all of the RefProp functions will be available through pre-defined user functions that provide:
- Legacy API function names in standard mathematical, thermodynamic notation, including Greek symbols,
- Unit handling to convert any parameters to the correct units and return values with units already applied.

See the add-in documentation (Mathcad Prime PDF User's Guide and Mathcad 15 Handbook) as well as the [NIST REFPROP DLL documentation](https://refprop-docs.readthedocs.io/en/latest/DLL/index.html) for instructions on using these functions.  For the Legacy Mathcad add-in, functions are only provided for the [Legacy API](LegacyAPI.md) function calls available with REFPROP 9.1.1 and later.

------ 
## Electronic Handbook for the RefProp Add-in for Legacy Mathcad _Only_

The files in the /Handbook directory can be copied into the /Handbook subdirectory in the Mathcad 15 installation path.  This copy procedure happens automatically when you compile and link the Legacy Mathcad Add-in DLL.  These files will allow the electronic handbook for RefProp to be available from the Mathcad **Help | e-Books** menu under **NIST RefProp**.  This electronic handbook will provide usage instruction and Mathcad usage examples that can be copied and pasted into your own worksheets. 
