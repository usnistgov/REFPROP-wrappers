//
// RefProp C# Examples Project - demonstrates how to call RefProp functions from a Windows C#/.Net program usingIRefProp64.dll interface. 
//
//
// Copyright 2017 Mill Creek Systems, Inc. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
// (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, 
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do 
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Mill Creek Systems, Inc.
// 3233 N. Arlington Heights Rd., Ste 301B
// Arlington Heights, IL 60004 USA
//
// www.MillCreekSystems.com
// support@MillCreekSystems.com
// 847/590-5686
 
using System;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

using MCS; // required namespace for IRefProp64.dll


namespace RefProp_cs_Examples
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs eventArgs)
        {
             comboBox_hflnme.SelectedIndex = 0;
        }
        
        // called when the Natural Gas Calc button is pressed
        private void Button_TempPro_Calculate_Click(object sender, EventArgs e)
        {
            long iErr, kph = 1;

            double te = 0.0, p = 0.0, d = 0.0, Dl = 0.0, Dv = 0.0, q = 0.0, ee = 0.0, hh = 0.0, ss = 0.0, cp = 0.0, cv = 0.0, w = 0.0;
            double[] x = new double[20], xliq = new double[20], xvap = new double[20];
            double[] xlkg = new double[20], xvkg = new double[20];

            double tk = 0.0, wm = 0.0;

            TextBox_TempPro_BubblePoint.Text = "";
            TextBox_TempPro_DewPoint.Text = "";

            string hpath = @"c:\Program Files (x86)\RefProp\fluids";
            long size = hpath.Length;

            hpath += new String(' ', 255 - (int)size);
            IRefProp64.SETPATHdll(hpath, ref size);

            long numComps = 8;
            string hfld = "methane|ethane|propane|butane|isobutan|pentane|ipentane|nitrogen";
            size = hfld.Length;

            hfld += new String(' ', 10000 - (int)size);

            string hfmix = "hmx.bnc" + new String(' ', 248);
            string hrf = "DEF";
            string herr = new String(' ', 255);

            iErr = 0;
            long hfldLen = hfld.Length, hfmixLen = hfmix.Length, hrfLen = hrf.Length, herrLen = herr.Length;

            numComps = -1;
            IRefProp64.SETUPdll(ref numComps, ref hfld, ref hfmix, ref hrf, ref iErr, ref herr, ref hfldLen, ref hfmixLen, ref hrfLen, ref herrLen);
            double version = iErr / 10000.0;
            TextBox_TempPro_RefPropDLLVersion.Text = Convert.ToString(version);

            numComps = 8;
            IRefProp64.SETUPdll(ref numComps, ref hfld, ref hfmix, ref hrf, ref iErr, ref herr, ref hfldLen, ref hfmixLen, ref hrfLen, ref herrLen);
            if (iErr != 0)
            {
                MessageBox.Show(herr, "RefProp SETUPdll() Error ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Get the individual components of the composition from the text fields
            x[0] = Convert.ToDouble(TextBox_InputSheet_Methane.Text);
            x[1] = Convert.ToDouble(TextBox_InputSheet_Ethane.Text);
            x[2] = Convert.ToDouble(TextBox_InputSheet_Propane.Text);
            x[3] = Convert.ToDouble(TextBox_InputSheet_Butane.Text);
            x[4] = Convert.ToDouble(TextBox_InputSheet_Isobutane.Text);
            x[5] = Convert.ToDouble(TextBox_InputSheet_Pentane.Text);
            x[6] = Convert.ToDouble(TextBox_InputSheet_Isopentane.Text);
            x[7] = Convert.ToDouble(TextBox_InputSheet_Nitrogen.Text);

            tk = Convert.ToDouble(t_textBox_2ndTab.Text); // temp in Kelvin
            p = Convert.ToDouble(p_textBox_2ndTab.Text); // pressure in kPa

            IRefProp64.WMOLdll(x, ref wm); // calculate molecular weight

            IRefProp64.TPFLSHdll(ref tk, ref p, x, ref d, ref Dl, ref Dv, xliq, xvap, ref q, ref ee, ref hh, ref ss, ref cp, ref cv, ref w, ref iErr, ref herr, ref herrLen);

            string[] values = { String.Format("{0,0:0.00}", d), String.Format("{0,0:0.00}", Dl), String.Format("{0,0:0.00}", Dv), String.Format("{0,0:0.00}", q), String.Format("{0,0:0.00}", ee), String.Format("{0,0:0.00}", hh), String.Format("{0,0:0.00}", ss), String.Format("{0,0:0.00}", cp), String.Format("{0,0:0.00}", cv), String.Format("{0,0:0.00}", w), String.Format("{0,0:0}", iErr), herr };

            DataGridView_2ndTab.Rows.Add(values);

            kph = 1;
            IRefProp64.SATPdll(ref p, x, ref kph, ref te, ref Dl, ref Dv, xliq, xvap, ref iErr, ref herr, ref herrLen);

            if (iErr == 0)
                TextBox_TempPro_BubblePoint.Text = String.Format("{0:n1}", te);
            else
                TextBox_TempPro_BubblePoint.Text = "-- ";

            kph = 2;
            IRefProp64.SATPdll(ref p, x, ref kph, ref te, ref Dl, ref Dv, xliq, xvap, ref iErr, ref herr, ref herrLen);
            if (iErr == 0)
                TextBox_TempPro_DewPoint.Text = String.Format("{0:n1}", te);
            else
                TextBox_TempPro_DewPoint.Text = "-- ";

            TextBox_TempPro_MolecularWeight.Text = String.Format("{0:n2}", wm);
    
            double critTemp = 0.0, critPressure = 0.0, critDensity = 0.0;

            IRefProp64.CRITPdll(x, ref critTemp, ref critPressure, ref critDensity, ref iErr, ref herr, ref herrLen);

            if (iErr == 0)
            {
                TextBox_TempPro_CriticalPoint_Temp.Text = String.Format("{0:n1}", critTemp);

                TextBox_TempPro_CriticalPoint_Press.Text = String.Format("{0:n1}", critPressure);

                TextBox_TempPro_CriticalPoint_Density.Text = String.Format("{0:n1}", critDensity);
            }
            else
                TextBox_TempPro_CriticalPoint_Temp.Text = TextBox_TempPro_CriticalPoint_Press.Text = TextBox_TempPro_CriticalPoint_Density.Text = "N/A";
        }

        // called when the R404A Calc button is pressed
        private void button3_Click(object sender, EventArgs eventArgs)
        {
            long iErr, kph = 1;

            double te = 0.0, p = 0.0, d = 0.0, Dl = 0.0, Dv = 0.0, q = 0.0, h = 0.0, ee = 0.0, hh = 0.0, ss = 0.0, cp = 0.0, cv = 0.0, w = 0.0;
            double[] x = new double[20], xliq = new double[20], xvap = new double[20];
            double[] xlkg = new double[20], xvkg = new double[20];

          
            double tk = 0.0, wm = 0.0, prevDeltaH = 0.0;

            TextBox_1stTab_BubblePoint.Text = "";
            TextBox_1stTab_DewPoint.Text = "";

            string hpath = @"c:\Program Files (x86)\RefProp\fluids";
            long size = hpath.Length;

            hpath += new String(' ', 255 - (int)size);
            IRefProp64.SETPATHdll(hpath, ref size);

            long numComps = 3;
            string hfld = "R125.FLD|R134A.FLD|R143A.FLD|";
            size = hfld.Length;

            hfld += new String(' ', 10000 - (int)size);

            string hfmix = "hmx.bnc" + new String(' ', 248);
            string hrf = "DEF";
            string herr = new String(' ', 255);

            iErr = 0;
            long hfldLen = hfld.Length, hfmixLen = hfmix.Length, hrfLen = hrf.Length, herrLen = herr.Length;

            numComps = -1;
            IRefProp64.SETUPdll(ref numComps, ref hfld, ref hfmix, ref hrf, ref iErr, ref herr, ref hfldLen, ref hfmixLen, ref hrfLen, ref herrLen);
            double version = iErr / 10000.0;
            TextBox_1stRab_RefPropDLLVersion.Text = Convert.ToString(version);

            numComps = 3;
            IRefProp64.SETUPdll(ref numComps, ref hfld, ref hfmix, ref hrf, ref iErr, ref herr, ref hfldLen, ref hfmixLen, ref hrfLen, ref herrLen);
            if (iErr != 0)
            {
                MessageBox.Show(herr, "RefProp SETUPdll() Error ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Composition
            x[0] = Convert.ToDouble(TextBox_R125_FLD.Text);
            x[1] = Convert.ToDouble(TextBox_R134A_FLD.Text);
            x[2] = Convert.ToDouble(TextBox_R143A_FLD.Text);

            tk = Convert.ToDouble(textBox_t_1stTab.Text);
            p = Convert.ToDouble(textBox_p_1stTab.Text);

            IRefProp64.SATSPLNdll(x, ref iErr, ref herr, ref herrLen);

            IRefProp64.WMOLdll(x, ref wm);
            TextBox_1stTab_MolecularWeight.Text = String.Format("{0,0:0.0}", wm);
                      
            IRefProp64.TPFLSHdll(ref tk, ref p, x, ref d, ref Dl, ref Dv, xliq, xvap, ref q, ref ee, ref hh, ref ss, ref cp, ref cv, ref w, ref iErr, ref herr, ref herrLen);

            string[] values = { String.Format("{0,0:0.00}", d), String.Format("{0,0:0.00}", Dl), String.Format("{0,0:0.00}", Dv), String.Format("{0,0:0.00}", q), String.Format("{0,0:0.00}", ee), String.Format("{0,0:0.00}", hh), String.Format("{0,0:0.00}", ss), String.Format("{0,0:0.00}", cp), String.Format("{0,0:0.00}", cv), String.Format("{0,0:0.00}", w), String.Format("{0,0:0.00}", iErr), herr };

            DataGridView_1st.Rows.Add(values);

            prevDeltaH = h;
            
            kph = 1;
            IRefProp64.SATPdll(ref p, x, ref kph, ref te, ref Dl, ref Dv, xliq, xvap, ref iErr, ref herr, ref herrLen);

            if (iErr == 0)
                TextBox_1stTab_BubblePoint.Text = String.Format("{0:n1}", te);
            else
                TextBox_1stTab_BubblePoint.Text = "-- ";

            kph = 2;
            IRefProp64.SATPdll(ref p, x, ref kph, ref te, ref Dl, ref Dv, xliq, xvap, ref iErr, ref herr, ref herrLen);
            if (iErr == 0)
            {
                TextBox_1stTab_DewPoint.Text = String.Format("{0:n1}", te);
            }
            else
                TextBox_1stTab_DewPoint.Text = "-- ";

            TextBox_1stTab_MolecularWeight.Text = String.Format("{0:n2}", wm);
         
            double critTemp = 0.0, critPressure = 0.0, critDensity = 0.0;

            IRefProp64.CRITPdll(x, ref critTemp, ref critPressure, ref critDensity, ref iErr, ref herr, ref herrLen);

            if (iErr == 0)
            {
                TextBox_1stRab_CriticalPoint_Temp.Text = String.Format("{0:n1}", critTemp);

                TextBox_1stRab_CriticalPoint_Press.Text = String.Format("{0:n1}", critPressure);

                TextBox_1stRab_CriticalPoint_Density.Text = String.Format("{0:n1}", critDensity);
            }
            else
                TextBox_1stRab_CriticalPoint_Temp.Text = TextBox_1stRab_CriticalPoint_Press.Text = TextBox_1stRab_CriticalPoint_Density.Text = "N/A";
        }

        // called when the EX MIX Fortran Calc button is pressed
        private void button5_Click(object sender, EventArgs e)
        {
            double p = 0.0;
            double[] x = new double[20], xliq = new double[20], xvap = new double[20];
            double[] xlkg = new double[20], xvkg = new double[20];

            long iErr, kph = 1;

            TextBox_1stTab_BubblePoint.Text = "";
            TextBox_1stTab_DewPoint.Text = "";

            string hpath = @"c:\Program Files (x86)\RefProp\fluids"; //mixtures
            long size = hpath.Length;

            hpath += new String(' ', 255 - (int)size);
            IRefProp64.SETPATHdll(hpath, ref size);

            string hflnme = comboBox_hflnme.Text + ".mix"; //R404A.mix, R410A.mix
            size = hflnme.Length;

            hflnme += new String(' ', 255 - (int)size);

            string hfmix = "hmx.bnc";
            size = hflnme.Length;
            hfmix += new String(' ', 255 - (int)size);

            string hrf = "DEF";
            string herr = new String(' ', 255);
            string hf = new String(' ', 10000);

            iErr = 0;
            long hfldLen = hflnme.Length, hfmixLen = hfmix.Length, hrfLen = hrf.Length, herrLen = herr.Length, ncc = 0;

            long l1 = 255, l2 = 255, l3 = 3, l4 = 10000, l5 = 255;
            
            IRefProp64.SETMIXdll(ref hflnme, ref hfmix, ref hrf, ref ncc, ref hf, x, ref iErr, ref herr, ref l1, ref l2, ref l3, ref l4, ref l5);
          
            double dl = 0.0, dv = 0.0;

            double t = Convert.ToDouble(textBox_t_3rdTab.Text);
            double tk = (t - 32) * 5.0 / 9.0 + 273.15;

            // Calculate saturated liquid properties
            kph = 1;
            IRefProp64.SATTdll(ref tk, x, ref kph, ref p, ref dl, ref dv, xliq, xvap, ref iErr, ref herr, ref l1);
            string[] values_liquid = { String.Format("{0,0:0}", kph), String.Format("{0,0:0.0}", p), String.Format("{0,0:0.00}", dl), String.Format("{0,0:0.00}", dv), String.Format("{0,0:0.000000000000}", xliq[0]), String.Format("{0,0:0.000000000000}", xliq[1]), String.Format("{0,0:0.000000000000}", xliq[2]), String.Format("{0,0:0}", iErr), herr };

            dataGridView_liquid_props.Rows.Add(values_liquid);

            // Calculate saturated vapor properties
            kph = 2;
            IRefProp64.SATTdll(ref tk, x, ref kph, ref p, ref dl, ref dv, xliq, xvap, ref iErr, ref herr, ref l1);
            string[] values_vapor = { String.Format("{0,0:0}", kph), String.Format("{0,0:0.0}", p), String.Format("{0,0:0.00}", dl), String.Format("{0,0:0.00}", dv), String.Format("{0,0:0.000000000000}", xvap[0]), String.Format("{0,0:0.000000000000}", xvap[1]), String.Format("{0,0:0.000000000000}", xvap[2]), String.Format("{0,0:0}", iErr), herr };

            // Display results
            dataGridView_vapor_props.Rows.Add(values_vapor);
        }

 
        private void TextBox_InputSheet_Methane_Enter(object sender, EventArgs e)
        {
            TextBox txtBox = sender as TextBox;
            double validNumber;
            bool isValidNumber = Double.TryParse(txtBox.Text.ToString(), out validNumber);
            if (isValidNumber != true)
                return;
            double val = Convert.ToDouble(txtBox.Text.ToString());
            txtBox.Text = string.Format("{0:#0.0000000}", val);

            if (validNumber > 1.0)
            {
                txtBox.Text = "0.0000000";
            }
        }

        private void TextBox_InputSheet_Methane_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (((e.KeyChar < 48 || e.KeyChar > 57) && e.KeyChar != 8 && e.KeyChar != 46))
            {
                if (e.KeyChar == 13) // carriage return 
                {
                    TextBox txtBox = sender as TextBox;
                    double validNumber;
                    bool isValidNumber = Double.TryParse(txtBox.Text.ToString(), out validNumber);
                    if (isValidNumber != true)
                        return;
                    double val = Convert.ToDouble(txtBox.Text.ToString());
                    txtBox.Text = string.Format("{0:#0.0000000}", val);

                    if (validNumber > 1.0)
                    {
                        txtBox.Text = "0.0000000";
                    }
                }

                e.Handled = true;
                return;
            }
        }

        // compute molecular weight
        public double getMolecularWeight()
        {
            double wm = 0.0;
            long iErr;

            double[] x = new double[20];

            string hpath = @"c:\Program Files (x86)\RefProp\fluids";
            long size = hpath.Length;

            hpath += new String(' ', 255 - (int)size);
            long len = hpath.Length;
            IRefProp64.SETPATHdll(hpath, ref size);

            long numComps = 8;
            string hfld = "methane|ethane|propane|butane|isobutan|pentane|ipentane|nitrogen";
            size = hfld.Length;

            hfld += new String(' ', 10000 - (int)size);

            string hfmix = "hmx.bnc" + new String(' ', 248);
            string hrf = "DEF";
            string herr = new String(' ', 255);

            iErr = 0;
            long hfldLen = hfld.Length, hfmixLen = hfmix.Length, hrfLen = hrf.Length, herrLen = herr.Length;

            numComps = 8;
            IRefProp64.SETUPdll(ref numComps, ref hfld, ref hfmix, ref hrf, ref iErr, ref herr, ref hfldLen, ref hfmixLen, ref hrfLen, ref herrLen);
            if (iErr != 0)
            {
                MessageBox.Show(herr, "RefProp SETUPdll() Error ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return wm;
            }

            // Composition
            x[0] = Convert.ToDouble(TextBox_InputSheet_Methane.Text);
            x[1] = Convert.ToDouble(TextBox_InputSheet_Ethane.Text);
            x[2] = Convert.ToDouble(TextBox_InputSheet_Propane.Text);
            x[3] = Convert.ToDouble(TextBox_InputSheet_Butane.Text);
            x[4] = Convert.ToDouble(TextBox_InputSheet_Isobutane.Text);
            x[5] = Convert.ToDouble(TextBox_InputSheet_Pentane.Text);
            x[6] = Convert.ToDouble(TextBox_InputSheet_Isopentane.Text);
            x[7] = Convert.ToDouble(TextBox_InputSheet_Nitrogen.Text);

            // Calculate molecular weight
            IRefProp64.WMOLdll(x, ref wm);

            return wm;
        }

        private void TextBox_InputSheet_Methane_TextChanged(object sender, EventArgs e)
        {
            double methane, ethane, propane, butane, isobutane, pentane, isopentane, nitogen;

            // make sure valid doubles have been entered before calculating thew total
            if (
                Double.TryParse(TextBox_InputSheet_Methane.Text.ToString(), out methane) == false ||
                Double.TryParse(TextBox_InputSheet_Ethane.Text.ToString(), out ethane) == false ||
                Double.TryParse(TextBox_InputSheet_Propane.Text.ToString(), out propane) == false ||
                Double.TryParse(TextBox_InputSheet_Butane.Text.ToString(), out butane) == false ||
                Double.TryParse(TextBox_InputSheet_Isobutane.Text.ToString(), out isobutane) == false ||
                Double.TryParse(TextBox_InputSheet_Pentane.Text.ToString(), out pentane) == false ||
                Double.TryParse(TextBox_InputSheet_Isopentane.Text.ToString(), out isopentane) == false ||
                Double.TryParse(TextBox_InputSheet_Nitrogen.Text.ToString(), out nitogen) == false
               )
                return;

            double total = methane + ethane + propane + butane + isobutane + pentane + isopentane + nitogen;

            total *= 1.0000000;

            string str = string.Format("{0:0.0000000}", total);

            TextBox_InputSheet_Total.Text = str;

            total = Convert.ToDouble(TextBox_InputSheet_Total.Text); // converting from double to string back to double to make sure the total has 7 decimals 
            double MolecularWeight = 0.0;

            if (total != 1.0)
            {
                MolecularWeight = 0;
                TextBox_TempPro_MolecularWeight.Text = "--";
                TextBox_InputSheet_Total.BackColor = Color.LightCoral;
                Label_Total.ForeColor = Color.Red;
                Button_TempPro_Calculate.Enabled = false;
            }
            else
            {
                MolecularWeight = getMolecularWeight();
                TextBox_TempPro_MolecularWeight.Text = string.Format("{0:0.00}", MolecularWeight);
                TextBox_InputSheet_Total.BackColor = Color.GreenYellow;
                Label_Total.ForeColor = SystemColors.WindowText;
                Button_TempPro_Calculate.Enabled = true;
            }
        }

        private void TextBox_InputSheet_Methane_Leave(object sender, EventArgs e)
        {
            TextBox txtBox = sender as TextBox;
            double validNumber;
            bool isValidNumber = Double.TryParse(txtBox.Text.ToString(), out validNumber);
            if (isValidNumber != true)
                return;
            double val = Convert.ToDouble(txtBox.Text.ToString());
            txtBox.Text = string.Format("{0:#0.0000000}", val);

            if (validNumber > 1.0)
            {
                txtBox.Text = "0.0000000";
            }
        }

        private void Button_TempPro_Clear_Click(object sender, EventArgs e)
        {
            DataGridView_2ndTab.Rows.Clear();

            TextBox_TempPro_BubblePoint.Text = "";
            TextBox_TempPro_DewPoint.Text = "";
            TextBox_TempPro_MolecularWeight.Text = "";
            TextBox_TempPro_CriticalPoint_Temp.Text = "";
            TextBox_TempPro_CriticalPoint_Press.Text = "";
            TextBox_TempPro_CriticalPoint_Density.Text = "";
        }

        private void TextBox_R125_FLD_Leave(object sender, EventArgs e)
        {
            TextBox txtBox = sender as TextBox;
            double validNumber;
            bool isValidNumber = Double.TryParse(txtBox.Text.ToString(), out validNumber);
            if (isValidNumber != true)
                return;
            double val = Convert.ToDouble(txtBox.Text.ToString());
            txtBox.Text = string.Format("{0:#0.0000000}", val);

            if (validNumber > 1.0)
            {
                txtBox.Text = "0.0000000";
            }
        }

        private void TextBox_R125_FLD_Enter(object sender, EventArgs e)
        {
            TextBox txtBox = sender as TextBox;
            double validNumber;
            bool isValidNumber = Double.TryParse(txtBox.Text.ToString(), out validNumber);
            if (isValidNumber != true)
                return;
            double val = Convert.ToDouble(txtBox.Text.ToString());
            txtBox.Text = string.Format("{0:#0.0000000}", val);

            if (validNumber > 1.0)
            {
                txtBox.Text = "0.0000000";
            }
        }

        private void TextBox_R125_FLD_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (((e.KeyChar < 48 || e.KeyChar > 57) && e.KeyChar != 8 && e.KeyChar != 46))
            {
                if (e.KeyChar == 13) // carriage return 
                {
                    TextBox txtBox = sender as TextBox;
                    double validNumber;
                    bool isValidNumber = Double.TryParse(txtBox.Text.ToString(), out validNumber);
                    if (isValidNumber != true)
                        return;
                    double val = Convert.ToDouble(txtBox.Text.ToString());
                    txtBox.Text = string.Format("{0:#0.0000000}", val);

                    if (validNumber > 1.0)
                    {
                        txtBox.Text = "0.0000000";
                    }
                }

                e.Handled = true;
                return;
            }
        }

        // called when the text item contents change in the R404A program
        private void TextBox_R125_FLD_TextChanged(object sender, EventArgs e)
        {
            double R125_FLD, R134A_FLD, R143A_FLD;

            // make sure valid double njumbers haven entered before calculating thew total
            if (
                    Double.TryParse(TextBox_R125_FLD.Text.ToString(), out R125_FLD) == false ||
                    Double.TryParse(TextBox_R134A_FLD.Text.ToString(), out R134A_FLD) == false ||
                    Double.TryParse(TextBox_R143A_FLD.Text.ToString(), out R143A_FLD) == false
                    )
                return;

            double total = R125_FLD + R134A_FLD + R143A_FLD;

            total *= 1.0000000;

            string str = string.Format("{0:0.0000000}", total);

            TextBox_total_1stTab.Text = str;

            total = Convert.ToDouble(TextBox_total_1stTab.Text); // converting from double to string back to double to make sure the total has 7 decimals 
            if (total != 1.0)
            {
                TextBox_total_1stTab.BackColor = Color.LightCoral;
                Label_Total_1stTab.ForeColor = Color.Red;
                button3.Enabled = false;
            }
            else
            {
                TextBox_total_1stTab.BackColor = Color.GreenYellow;
                Label_Total_1stTab.ForeColor = SystemColors.WindowText;
                button3.Enabled = true;
            }
        }

        // clear the R404A grid
        private void button1_Click(object sender, EventArgs e)
        {
            DataGridView_1st.Rows.Clear();

            TextBox_1stTab_BubblePoint.Text = "";
            TextBox_1stTab_DewPoint.Text = "";
            TextBox_1stTab_MolecularWeight.Text = "";
            TextBox_1stRab_CriticalPoint_Temp.Text = "";
            TextBox_1stRab_CriticalPoint_Press.Text = "";
            TextBox_1stRab_CriticalPoint_Density.Text = "";
        }

        // clear the EX MIX Fortran data grid
        private void button4_Click(object sender, EventArgs e)
        {
            dataGridView_liquid_props.Rows.Clear();
            dataGridView_vapor_props.Rows.Clear();
        }

        // open the About dialog
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            panel9.Show();
        }

        // web site hyperlink clicked -> view web site
        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            ProcessStartInfo sInfo = new ProcessStartInfo("http://www.millcreeksystems.com/");
            Process.Start(sInfo);
        }

        // email hyperlink clicked -> send email 
        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("mailto:support@millcreeksystems.com");
        }

        // close the About dialog
        private void button2_Click_2(object sender, EventArgs e)
        {
            panel9.Hide();
        }
    }
}
