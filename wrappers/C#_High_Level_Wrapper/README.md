# Wrapper of REFPROP for C&#35;

The files in this directory demonstrate how to call REFPROP routines from a C# program for High level API. This example can be used to easily and quickly create your own C# program that uses RefProp.
Cancel changes
## License

These files fall under a MIT-style license.

## To Use Solution

1. Do a git clone of the entire repository.
2. Go into ``wrappers/Csharp`` directory
3. Open the Visual Studio 2015 or later solution.

## To Use Wraper

1. Donwload the DLL wrapper : [wrappers/C#_High_Level_Wrapper/RefpropDLL/bin/Debug](https://github.com/meyinyil/REFPROP-wrappers/tree/master/wrappers/C%23_High_Level_Wrapper/RefpropDLL/bin/Debug)
2. Insert your project. (Bin/Debug)
3. Add references the DLL
4. Create an object.
5. Here is an example :

             using RefpropDLL;
             public partial class Refrpop
             {
                        Refprop refrpop = new Refprop(); // creating object

                        string prop_1 = "T"; // property 1, T Ç Temperature
                        double prop_1_value = "300"; // property 1 value, for temperature in K
                        string prop_2 = "p"; // property 2, P : Pressure
                        double prop_2_value = "200000; // property 2 value, for pressure in Pa
                        string output_prop = "H"; // output property, H : Enthalpy (J/kg)
                        string refri = "R134A"; // fluid

                        double output_val = refrpop.Calculate(output_prop, prop_1, prop_1_value, prop_2, prop_2_value, refri); 
                        // this method calculates desired property.   output : 424.410 J/kg
                      
                        string phase = refrpop.Phase(prop_1, prop_1_value, prop_2, prop_2_value, refri); 
                        // this method gives the phase state of the fluid.
                        // phase : Superheated gas 
                        
                        double dll = refrpop.DLL_VERSION(); // this method gives DLL version. (version*10000)
                        dll = dll / 10000;
                        // dll : 10
                        
             }

* You must already have REFPROP installed in the default location
