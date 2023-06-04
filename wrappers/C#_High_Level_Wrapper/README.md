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

                        string prop_1 = property_1.SelectedItem.ToString(); // property 1
                        ouble prop_1_value = Convert.ToDouble(property_1_value.Text); // property 1 value
                        string prop_2 = property_2.SelectedItem.ToString(); // property 2
                        double prop_2_value = Convert.ToDouble(property_2_value.Text); // property 2 value
                        string output_prop = output_property.Text.ToString(); // output property
                        string refri = refrigerant.SelectedItem.ToString(); // fluid

                       output_property_value.Text = output_val.ToString("0.##");

                        double output_val = refrpop.Calculate(output_prop, prop_1, prop_1_value, prop_2, prop_2_value, refri); 
                        // this method calculates desired property.

                        string phase = refrpop.Phase(prop_1, prop_1_value, prop_2, prop_2_value, refri); // this method gives the phase satae pf the fluid.
                        phase_textbox.Text = phase;
                        double dll = refrpop.DLL_VERSION(prop_1, prop_1_value, prop_2, prop_2_value, refri); // this method gives DLL version. (version*10000)
                        dll = dll / 10000;
                        dll_version_label.Text = dll.ToString("0.#"); 

             }

* You must already have REFPROP installed in the default location
