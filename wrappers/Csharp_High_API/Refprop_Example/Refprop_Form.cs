using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using RefpropDLL;
using static System.Net.WebRequestMethods;
using static System.Windows.Forms.LinkLabel;

namespace Refprop_Deneme
{
    public partial class Refprop_Form : Form
    {
        public Refprop_Form()
        {
            InitializeComponent();
        }

        private void calculate_Click(object sender, EventArgs e)
        {
            Refprop refprop = new Refprop(); // creating object 
       //     refprop.SetDLLPathProgramtDirectory();
            string prop_1 = property_1.SelectedItem.ToString();
            double prop_1_value = Convert.ToDouble(property_1_value.Text);
            string prop_2 = property_2.SelectedItem.ToString();
            double prop_2_value = Convert.ToDouble(property_2_value.Text);
            string output_prop = output_property.Text.ToString();
            string refri = refrigerant.SelectedItem.ToString();

            switch (prop_1)
            {
                case "T":
                    prop_1_value= prop_1_value+273.15;
                    break;
                case "P":
                    prop_1_value = prop_1_value *(100000);
                    break;
                case "H":
                    prop_1_value = prop_1_value * (1000);
                    break;
                case "S":
                    prop_1_value = prop_1_value * (1000);
                    break;
            }
            switch (prop_2)
            {
                case "T":
                    prop_2_value = prop_2_value + 273.15;
                    break;
                case "P":
                    prop_2_value = prop_2_value * (100000);
                    break;
                case "H":
                    prop_2_value = prop_2_value * (1000);
                    break;
                case "S":
                    prop_2_value = prop_2_value * (1000);
                    break;
            }

            //  ("P" for Pressure, "T" for Temmperature etc..) You may find a table for input and output parameters below the ALLPROPSdll, link: https://refprop-docs.readthedocs.io/en/latest/DLL/high_level.html#function-listing
            // prop_1 : Input Property 1
            // prop_1_value :Input Property 1 Value
            // prop_2 : Input Property 2
            // prop_2_value :Input Property 2 Value
            // refri : Refrigerant name ("R134A" , "R450A" etc.)
            // output_prop : Output Property

            double output_val = refprop.Calculate(output_prop, prop_1, prop_1_value, prop_2, prop_2_value, refri);
         
         
            switch (output_prop)
            {
                case "T":
                    output_val = output_val - 273.15;
                    break;
                case "P":
                    output_val = output_val / (100000);
                    break;
                case "H":
                    output_val = output_val / (1000);
                    break;
                case "S":
                    output_val = output_val / (1000);
                    break;
            }
           
            output_property_value.Text = output_val.ToString("0.##");
            
            string phase = refprop.Phase(prop_1, prop_1_value, prop_2, prop_2_value, refri);
            phase_textbox.Text = phase;
            double dll = refprop.DLL_VERSION();
            dll = dll / 10000;
            dll_version_label.Text = dll.ToString("0.#"); 

        }

        private void property_1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string prop_1 = property_1.SelectedItem.ToString();

            switch (prop_1)
            {
                case "T":
                    property_1_unit.Text = "°C";
                    break;
                case "P":
                    property_1_unit.Text = "Bara";
                    break;
                case "H":
                    property_1_unit.Text = "kJ/kg";
                    break;
                case "S":
                    property_1_unit.Text = "(kJ/kg)/K";
                    break;
                case "D":
                    property_1_unit.Text = "kg/m^3";
                    break;
                case "Q":
                    property_1_unit.Text = "kg-vap/kg-tot";
                    break;

            }
        }

        private void property_2_SelectedIndexChanged(object sender, EventArgs e)
        {
            string prop_2 = property_2.SelectedItem.ToString();

            switch (prop_2)
            {
                case "T":
                    property_2_unit.Text = "°C";
                    break;
                case "P":
                    property_2_unit.Text = "Bara";
                    break;
                case "H":
                    property_2_unit.Text = "kJ/kg";
                    break;
                case "S":
                    property_2_unit.Text = "(kJ/kg)/K";
                    break;
                case "D":
                    property_2_unit.Text = "kg/m^3";
                    break;
                case "Q":
                    property_2_unit.Text = "kg-vap/kg-tot";
                    break;
            }

        }

        private void output_property_SelectedIndexChanged(object sender, EventArgs e)
        {
            string output_prop = output_property.SelectedItem.ToString();

            switch (output_prop)
            {
                case "T":
                    output_unit.Text = "°C";
                    break;
                case "P":
                    output_unit.Text = "Bara";
                    break;
                case "H":
                    output_unit.Text = "kJ/kg";
                    break;
                case "S":
                    output_unit.Text = "(kJ/kg)/K";
                    break;
                case "D":
                    output_unit.Text = "kg/m^3";
                    break;
                case "Q":
                    output_unit.Text = "kg-vap/kg-tot";
                    break;
            }
        }
    }
}
