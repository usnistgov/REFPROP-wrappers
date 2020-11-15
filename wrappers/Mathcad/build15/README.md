# Mathcad 15 REFPROP Wrapper

This directory contains the Microsoft Visual Studio 2015 solution files for building the **NIST RefProp** add-in for **Mathcad 15**.  While targeted for Mathcad 15, it should also work on earlier versions with appropriate modifications to the paths in the solution file to target the correct installation directory for earlier versions of Mathcad.

## Building the Mathcad 15 add-in DLL

To build the Mathcad 15 add-in DLL, 

1. Make sure that Mathcad is not running
2. Go to the /build15 directory
3. Use the `MathcadREFPROPwrapper.sln` file to open the solution in VisualStudio 2015 or later.  This will open the MathcadREFPROPwrapper project. 
4. Make sure that the build is set to "**Release**" and target is "**Win32**" (Mathcad 15 is 32-bit) 
5. Select **Build | Rebuild MathcadREFPROPwrapper**
6. This will build the add-in DLL and it will be placed in the Mathcad 15 installation directory under **/userfi**
7. The function interface doc (RefProp_EN.xml) will be copied to the Mathcad 15 installation directory under **/doc/funcdoc**
8. The context sensitive help files will also be copied to the Mathcad 15 installation directory under **/doc/HELP_EN/Refprop**
9. The RefProp Handbook (eBook) files will be copied to the Mathcad 15 installation directory under **/Handbook/Refprop**

## Electronic Handbook for the RefProp Add-in for Mathcad 15

The files in the /Handbook directory can be copied into the /Handbook subdirectory in the Mathcad 15 installation path.  **This copy procedure happens automatically when you compile and link the Mathcad Add-in DLL using the provided VS solution.**  These files will allow the electronic handbook for RefProp to be available from the Mathcad **Help | e-Books** menu under **NIST RefProp**.  This electronic handbook will provide usage instruction and Mathcad usage examples that can be copied and pasted into your own worksheets.
  
## Insert Function Assistant

The files in the /doc directory The RefProp functions will be copied into the /doc subdirectory in the Mathcad 15 installation path using the CopyDocs15.bat file in the Mathcad wrapper directory.  **This copy procedure happens automatically when you compile and link the Mathcad Add-in DLL using the provided VS solution.**  This will insert the **NIST RefProp** functions into the Insert | Function panel in the Mathcad 15 user interface.  Brief context sensitive help will also be available by pressing **F1** when the cursor is on any NIST RefProp function.

