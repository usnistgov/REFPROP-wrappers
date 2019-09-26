# REFPROP for Excel

Help!
-----

Please file an issue at https://github.com/usnistgov/REFPROP-issues and we will get back to you as quickly as possible.  Please don't email NIST staff directly, the only exception being if you have proprietary code that cannot be shared publicly.

In addition to the information below, please look at https://github.com/usnistgov/REFPROP-wrappers/issues/105

To download the above files (XLS and XLA), click on the file, then the download button (right-ish side).

Installation
------------

The following outlines the procedure for using REFPROP within any spreadsheet in Office 2007 or 2010:

1.  Open REFPROP.xls and save it as an add-in, REFPROP.xla or REFPROP.xlam, in the main REFPROP folder, C:\Program Files\REFPROP.
2.  Go to File/Options/Trust Center/Trust Center Settings (button at bottom right).
3.  Select "Trusted Locations" on the left.  Click "Add new location".  Browse to C:\Program Files\REFPROP, select "Subfolders of this location are also trusted", and click "OK".
4.  Go to File/Options/Add-Ins and select "Excel Add-ins" in the Manage drop-down box at the bottom, and click Go.
5.  Click "Browse", and navigate to C:\Program Files\REFPROP, select REFPROP.xlam and click OK.  IMPORTANT: Do not simply select REFPROP.xlam when it first comes up, as this will be in the wrong folder (C:\Documents and Settings\Username\Application Data\Microsoft\AddIns), which is not trusted and will not work. 
6.  Select the Data tab, and click on Edit Links.  Select REFPROP.xlam.  Click on Change Source and navigate to C:\Program Files\REFPROP.  Select REFPROP.xlam there and click OK.  (This is just to make sure you are connected to the correct Add-in.)  If the Data tab is greyed out, start typing in a Refprop command [such as “=Density("water","TP","SI",300,1) ] and the button should become active.
7.  Once you have the xla or xlam file set up, you can open a brand new work book and the functions should be available to you.  Do not continue working with the Refprop.xls file since it still contains the VB code that is also in the xlam file.  In this manner, future updates from NIST of the Refprop.xls file can be resaved as the xlam file, and all of your work books will have access to the most recent code.

Other tips:

1.  In some cases the macros may not work.  Try saving the file as a macro-enabled workbook (under Save As…).
2.  The xls file distributed with version 9.0 sometimes will give false answers depending on the sequence of calculations if multiple xls files are open.  Switching between the open files may cause the initial setup to be lost.  The updated xls file given above fixes this.
3.  For inputs that do not required a 5th parameter, Excel may require the comma at the end, for example:   =Pressure("water", "TVAP", "SI", 298,)
4.  If Excel cannot find the Refprop fluid files, you can copy the *.FLD and HMX.BNC files into a default directory:  C:\REFPROP\FLUIDS.  When the program fails to find the fluid files, it will look to see if a C:\REFPROP\FLUIDS (or D:) is available, and if so it will use the files from that source.
5.  If you see dual entries for each function in your workbook, then you have either saved two xla files, or you are working with a file that still contains the VB code.  Start with a blank workbook to eliminate the dual entries for the latter case.

## Notes for Windows users

The Refprop.xls file that comes with the program has Visual Basic (VB) code embedded within it to make the connection between the workbook and the Refprop DLL.  However, when working with anything other than temporary calculations, it is best to start with a blank workbook that does not have the VB code.  In this manner, future updates to the Refprop program can be made available to all of your old workbooks through the instructions below.

When making calculations of the critical point for mixtures, or when saturation states fail to converge, see:
https://pages.nist.gov/REFPROP-docs/#calculation-of-the-critical-point-and-saturation-states-in-the-critical-region

If calculations do not appear to work and you are located outside of the U.S., try switching the period and comma, or the comma and semicolon, to enter the numbers in the format required by your version of Microsoft products.  For example, try both of these to see which format is required:

    =Density("argon","TP","SI",325.5,8.4)
    =Density("argon";"TP";"SI";325,5;8,4)

