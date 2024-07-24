# Wrapper of REFPROP for C&#35;

The files in this directory demonstrate how to call REFPROP routines from a C# program for High level API. This example can be used to easily and quickly create your own C# program that uses RefProp.
Cancel changes


## To Use Wraper

1. Clone the repository.
2. Insert your project.
3. Add references the DLL
4. Create an object.
5. Here is an example :

             using RefpropDLL;
             public partial class Refprop
             {
                        Refprop refprop = new Refprop(); // creating object

                        string prop_1 = "T"; // property 1, T Ç Temperature
                        double prop_1_value = "300"; // property 1 value, for temperature in K
                        string prop_2 = "p"; // property 2, P : Pressure
                        double prop_2_value = "200000; // property 2 value, for pressure in Pa
                        string output_prop = "H"; // output property, H : Enthalpy (J/kg)
                        string fluid = "R134A"; // fluid
                        double output_val = refprop.Calculate(output_prop, prop_1, prop_1_value, prop_2, prop_2_value, fluid); 
                        // this method calculates desired property.   output : 424.410 J/kg
                      
                        string phase = refprop.Phase(prop_1, prop_1_value, prop_2, prop_2_value, fluid); 
                        // this method gives the phase state of the fluid.
                        // phase : Superheated gas 
                        
                        double dll = refprop.DLL_VERSION(); // this method gives DLL version. (version*10000)
                        dll = dll / 10000;
                        // dll : 10
                        
             }

* You must already have REFPROP installed in the default location
* refprop.SetDLLPathProgramtDirectory(); this method can be used if dll will be used in the project. Sets the path : appdomain.currentdomain.basedirectory
