using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RefpropDLL
{
    public class Refprop
    {
        int iUnits = 21;
        int iFlag = 1;
        int iMass = 1;
        double[] z = new double[20];
        double[] Output = new double[200];
        static string hUnits = new string(' ', 255);
        double[] x = new double[20];
        double[] y = new double[20];
        double[] x3 = new double[20];
        static string hpath = "C:\\Program Files (x86)\\REFPROP";
        bool base_path = true;    
        int iUCode = 0;
        double q = 0;
        static string herr = new string(' ', 255);
        int herr_lenght = herr.Length;
        int hUnits_length = hUnits.Length;

        /// <summary>
        /// This method sets the dll program directory.
        /// </summary>
        public void SetDLLPathProgramtDirectory()
        {  
            string path = AppDomain.CurrentDomain.BaseDirectory;
            hpath = path;
            base_path = false;
           
        }

        /// <summary>
        /// This method define fuild phase depends on given two properties like T,P,Q,etc.
        /// </summary>
        /// <param name="a_p">  Input Property 1, Ex.: "T" (K) </param>
        /// <param name="a">  Input Property 1 Value, Ex.: 300 (K) </param>
        /// <param name="b_p">  Input Property 2, Ex.: "P" (Pa) </param>
        /// <param name="b">  Input Property 2 Value, Ex.: 300000 (Pa) </param>
        /// <param name="hFldi">  Fluild Name, Ex.: "R134A" </param>
        /// <returns>Desired Poperty in double type, Ex : 200000 (J/kg) </returns>
        public string Phase(string a_p, double a, string b_p, double b, string hFldi)
        {
            int hpath_Lenght = hpath.Length;
            string hFld = hFldi;
            int ierr = 0;
            int hFld_length = hFld.Length;
            string hIn = a_p + b_p;
            string hOut = "PHASE";
            int hIn_length = hIn.Length;
            int hOut_lenght = hOut.Length;
            if (hOut == "Q") { hOut = "QMASS"; }
 
            if (base_path)
            {
                Refprop_DLL_base.SETPATHdll(hpath, ref hpath_Lenght);
                Refprop_DLL_base.SETFLUIDSdll(hFld, ref ierr, ref hFld_length);
                Refprop_DLL_base.REFPROPdll(ref hFld, ref hIn, ref hOut, ref iUnits, ref iFlag, ref iMass, ref a, ref b, z, Output, ref hUnits, ref iUCode, x, y, x3, ref q, ref ierr, ref herr,
                    ref hFld_length, ref hIn_length, ref hOut_lenght, ref hUnits_length, ref herr_lenght);
            }

            else
            {
                Refprop_DLL.SETPATHdll(hpath, ref hpath_Lenght);
                Refprop_DLL.SETFLUIDSdll(hFld, ref ierr, ref hFld_length);
                Refprop_DLL.REFPROPdll(ref hFld, ref hIn, ref hOut, ref iUnits, ref iFlag, ref iMass, ref a, ref b, z, Output, ref hUnits, ref iUCode, x, y, x3, ref q, ref ierr, ref herr,
                    ref hFld_length, ref hIn_length, ref hOut_lenght, ref hUnits_length, ref herr_lenght);
            }

            string result = hUnits;
            return result;

        }

        /// <summary>
        /// This method calculates propety depends on given two properties like T,P,Q,etc.
        /// </summary>
        /// <param name="hOuti"> Output Property 1, Ex. : "H" (Enthalpy) </param>
        /// <param name="a_p">  Input Property 1, Ex.: "T" (K) </param>
        /// <param name="a">  Input Property 1 Value, Ex.: 300 (K) </param>
        /// <param name="b_p">  Input Property 2, Ex.: "P" (Pa) </param>
        /// <param name="b">  Input Property 2 Value, Ex.: 300000 (Pa) </param>
        /// <param name="hFldi">  Fluild Name, Ex.: "R134A" </param>
        /// <returns>Desired Poperty in double type, Ex : 200000 (J/kg) </returns>
        public double Calculate(string hOuti, string a_p, double a, string b_p, double b, string hFldi)
        {

            int hpath_Lenght = hpath.Length;
            Refprop_DLL.SETPATHdll(hpath, ref hpath_Lenght);
            string hFld = hFldi;
            int ierr = 0;
            int hFld_length = hFld.Length;
            Refprop_DLL.SETFLUIDSdll(hFld, ref ierr, ref hFld_length);
            string hIn = a_p + b_p;
            string hOut = hOuti;
            int hIn_length = hIn.Length;
            int hOut_lenght = hOut.Length;



            if (hOut == "Q") { hOut = "QMASS"; } // For the quality, hOuti must be type as "QMASS"
            if (base_path)
            {
                Refprop_DLL_base.REFPROPdll(ref hFld, ref hIn, ref hOut, ref iUnits, ref iFlag, ref iMass, ref a, ref b, z, Output, ref hUnits, ref iUCode, x, y, x3, ref q, ref ierr, ref herr,
                        ref hFld_length, ref hIn_length, ref hOut_lenght, ref hUnits_length, ref herr_lenght);
            }
            else
            {
                Refprop_DLL.REFPROPdll(ref hFld, ref hIn, ref hOut, ref iUnits, ref iFlag, ref iMass, ref a, ref b, z, Output, ref hUnits, ref iUCode, x, y, x3, ref q, ref ierr, ref herr,
                        ref hFld_length, ref hIn_length, ref hOut_lenght, ref hUnits_length, ref herr_lenght);
            }


            double result = Output[0];

            return result;
        }

        /// <summary>
        /// This method gives DLL version.
        /// </summary>
        /// <returns>Retuns type is int, Ex:10 type</returns>
        public int DLL_VERSION()
        {
            int hpath_Lenght = hpath.Length;
            Refprop_DLL.SETPATHdll(hpath, ref hpath_Lenght);
            string hFld = "WATER";
            int ierr = 0;
            int hFld_length = hFld.Length;
            Refprop_DLL.SETFLUIDSdll(hFld, ref ierr, ref hFld_length);
            string hIn = "TP";
            string hOut = "DLL#";
            int hIn_length = hIn.Length;
            int hOut_lenght = hOut.Length;
            double a = 300;
            double b = 101325;

            if (hOut == "Q") { hOut = "QMASS"; }
            if (base_path)
            {
                Refprop_DLL_base.REFPROPdll(ref hFld, ref hIn, ref hOut, ref iUnits, ref iFlag, ref iMass, ref a, ref b, z, Output, ref hUnits, ref iUCode, x, y, x3, ref q, ref ierr, ref herr,
                    ref hFld_length, ref hIn_length, ref hOut_lenght, ref hUnits_length, ref herr_lenght);
            }
            else
            {
                Refprop_DLL.REFPROPdll(ref hFld, ref hIn, ref hOut, ref iUnits, ref iFlag, ref iMass, ref a, ref b, z, Output, ref hUnits, ref iUCode, x, y, x3, ref q, ref ierr, ref herr,
                     ref hFld_length, ref hIn_length, ref hOut_lenght, ref hUnits_length, ref herr_lenght);
            }
       
            int result = iUCode;
            return result;
        }

    }
}
