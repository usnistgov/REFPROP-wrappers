# Wrapper of REFPROP for C&#35;

The files in this directory demonstrate how to call REFPROP routines from a C# program. It demonstrates calling the RefProp unmanaged native C run-time methods from the managed C#/.Net run-time including marshalling data between the two environments.  This example can be used to easily and quickly create your own C# program that uses RefProp.

Thanks to Warren Newhauser (support@millcreeksystems.com) of Mill Creek Systems (http://www.millcreeksystems.com/) for making these files freely available. 

## License

These files fall under a MIT-style license.

## To Use

1. Do a git clone of the entire repository (or alternatively download a zip of this entire repository from https://github.com/usnistgov/REFPROP-wrappers/archive/master.zip)
2. Go into ``wrappers/Csharp`` directory
3. Open the Visual Studio 2015 solution.

## Debugging

* The visual studio project will open in Visual Studio 2015 Express, and likely in other newer versions of Visual Studio, but not in older versions of Visual Studio
* To use older versions of Microsoft Visual Studio, first create a new solution, then create two projects (a C# class library for IRefProp64 and a C# Windows project for RefProp_CS_Examples) and add finally the source code to your newly created C# projects, respectively.
* You must already have REFPROP installed in the default location
* Contact Warren Newhauser (support@millcreeksystems.com) with other questions, or file an issue: https://github.com/usnistgov/REFPROP-wrappers/issues/new
