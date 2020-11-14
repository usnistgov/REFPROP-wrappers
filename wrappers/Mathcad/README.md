#Mathcad REFPROP Wrapper

This directory contains the source for building the Mathcad 15 and Mathcad Prime wrappers for the NIST REFPROP materials library.   This is version 2.0 of the code which makes calls to the REFPROP.dll (and REFPROP64.dll) when NIST Refprop 9.1 or later is installed on the Windows 10 computer.

#Using This Directory as a Git Repository

In order to properly transfer the Mathcad 15 Handbook directory, which is made up of many Mathcad 15 worksheets with the .xmcd file extension, you will need a .gitattributes file with the following line in it:  

    *.xmcd binary

While the Mathcad 15 Handbook worksheets are XML and not binary, this attribute will keep git from messing with the CRLF on the line endings in the XML worksheets, corrupting them and making them unusable when committed or transferred to and from a Git remote repository.	

#Building the Mathcad 15 add-in DLL

To build the Mathcad 15 add-in DLL, 

1. Make sure that Mathcad is not running
2. Go to the /build15 directory
3. Use the MathcadREFPROPwrapper.sln file to open the solution in VisualStudio 2015 or later.  This will openthe MathcadREFPROPwrapper project. 
4. Make sure that the build is set to "Release" and target is "Win32"
5. Select **Build | Rebuild MathcadREFPROPwrapper**
6. This will build the add-in DLL and it will be placed in the Mathcad 15 installation diretory under **/userfi**
7. The function interface doc (Refprop_EN.xml) will be copied to the Mathcad 15 installation directory under **/doc/funcdoc**
8. The context sensitive help files will also be copied to the Mathcad 15 installation directory under **/doc/HELP_EN/Refprop**
9. The Refprop Handbook (eBook) files will be copied to the Mathcad 15 installation directory under **/Handbook/Refprop**

#Building the Mathad Prime Custom Function DLL

To build the Mathad Prime DLL,

1. Make sure that Mathcad Prime is not running
2. Go to the /buildPrime directory
3. Use the MathcadREFPROPwrapper.sln file to open the solution in VisualStudio 2015 or later.  This will open the MathcadREFPROPwrapper project. 
4. Make sure that the build is set to "Release" and target is "x64"
5. Select **Build | Rebuild MathcadREFPROPwrapper**
6. This will build the add-in DLL and it will be placed in the Mathcad 15 installation diretory under **/userfi**
7. There is no interface file facility for Custom Functions in Prime at this time.

If you have a different version than **Mathad Prime 6.0.0.0**, you will need to modify the VS project settings under:
    - C++ > General > Additional Include Direcotries
    - Linker > General > Additional Library Directories
    - Build Events > Post-Build Events
to change the applicable "PTC\Mathcad Prime 6.0.0.0" path strings to the appropriate version.

#Using the Refprop Add-in in either Mathcad 15 or Mathcad Prime

Whith the Mathcad wrapper DLL compiled and copied to the appropriate Mathcad directories, the Refprop functions,
which all begin with the prefix "rp_", will be directly available for use in Mathcad.  In Mathcad 15, the functions
and usage descriptions will be available in the Insert Function panel under the category **NIST Refprop**.
Unfortunately, Mathcad Prime does not yet have the facility to list custom functions as of Mathcad Prime 6.0.  A User's Guide
for Mathcad Prime can be found in PDF format in the PrimeDocs folder of this repository.

#Electronic Handbook for the Refprop Add-in

The files in the /Handbook directory can be copied to the /Handbook subdirectory in the Mahtcad 15 installation path.  This copy procedure happens automatically when you compile and link the Mathcad Add-in DLL.  These files will allow the electonic handbook for Refprop to be available from the Mathcad **Help | e-Books** menu under **NIST Refprop**.  This electronic handbook will provide usage instruction and Mathcad usage examples that can be copied and pasted into your own worksheets.
  
For more information on how to use the Refprop functions in either flavor of Mathcad, see [Refprop Help](./doc/HELP_EN/Refprop/RefpropHelp.htm)  


