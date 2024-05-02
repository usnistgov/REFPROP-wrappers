using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace RefpropDLL
{
    public class Refprop_DLL
    { 

        [DllImport(@"REFPRP64.DLL")]
        public static extern void SETPATHdll(string hpath, ref int hpath_Lenght);
        [DllImport(@"REFPRP64.DLL")]

        public static extern void SETFLUIDSdll(string hFld, ref int ierr, ref int hFld_length);

        [DllImport(@"REFPRP64.DLL")]
        /*Calculate the properties identified in the hOut string for the inputs specified in the hIn string for the fluid or mixture given in the hFld string. 
        The unit identifier for the properties should be passed in the iUnits variable (as described below).
        Compositions can be sent as mole fractions or mass fractions in the z array depending on the value of iMass.*/

        public static extern void REFPROPdll(

       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hFld,                   // hFld [char ,in] :: Fluid string.
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hIn,                    // hIn [char ,in] :: Input string of properties being sent to the routine.
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hOut,                   // hOut [char ,in] :: Output string of properties to be calculated. 
       ref int iUnits,                                                          // iUnits [int ,in] :: The unit system to be used for the input and output properties (such as SI, English, etc.). MOLAR BASE SI = 20
       ref int iFlag,                                                           // iFlag [int ,in] :: Flag to specify if the routine SATSPLN should be called (where a value of 1 activates the call).
       ref int iMass,                                                           // iMass [int ,in] :: Specifies if the input composition is mole or mass based
       ref double a,                                                            // a [double ,in] :: First input property as specified in the hIn variable.
       ref double b,                                                            // b [double ,in] :: Second input property as specified in the hIn variable.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] z,       // z (20) [double ,in] :: Molar composition (array of size ncmax=20).
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] Output,  // Output (200) [double ,out] :: Array of properties specified by the hOut string (array of size
                                                                                // 200 dimensioned as double precision). The number -9999970 will be returned when errors occur, and the number
                                                                                // -9999990 will be returned when nothing was calculated. Read the comments in the ALLPROPS routine to fully understand the contents of this array.
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hUnits,                 // hUnits [char ,out] :: The units for the first property in the Output array. Strings such as a fluid name may also be passed back in this position.
                                                                                // To obtain the units for all of the properties sent to the string, call the ALLPROPS routine instead.
       ref int iUCode,                                                          // iUCode [int ,out] :: Unit code that represents the units of the first property in the Output array. See below for further details.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,       // x (20) [double ,out] :: Composition of the liquid phase (array of mole fractions of size 20) for two-phase states on a mole or mass basis depending on the value of iMass.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] y,       // y (20) [double ,out] :: Composition of the vapor phase(array of mole fractions of size 20) for two-phase states on a mole or mass basis depending on the value of iMass.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x3,      // x3 (20) [double ,out] :: Reserved for returning the composition of a second liquid phase for LLE or VLLE.
       ref double q,                                                            // q [double ,out] :: Vapor quality on a mole or mass basis depending on the value of iMass. (See subroutine ABFLSH for the definitions of values. returned for this variable).
                                                                                // To obtain the molar quality regardless of iMass, send “qmole” as an input in hIn, and vice-versa for “qmass”.
       ref int ierr,                                                            // ierr [int ,out] :: Error flag
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,                   // herr [char ,out] :: Error string (character*255)
       ref int hFld_length,                                                     // hFld_length [int ] :: length of variable hFld (default: 10000)
       ref int hIn_length,                                                      // hIn_length [int ] :: length of variable hIn (default: 255)
       ref int hOut_length,                                                     // hOut_length [int ] :: length of variable hOut (default: 255)
       ref int hUnits_length,                                                   // hUnits_length [int ] :: length of variable hUnits (default: 255)
       ref int herr_length);
    }

    public class Refprop_DLL_base
    {

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.DLL")]
        public static extern void SETPATHdll(string hpath, ref int hpath_Lenght);
        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.DLL")]

        public static extern void SETFLUIDSdll(string hFld, ref int ierr, ref int hFld_length);

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.DLL")]
        /*Calculate the properties identified in the hOut string for the inputs specified in the hIn string for the fluid or mixture given in the hFld string. 
        The unit identifier for the properties should be passed in the iUnits variable (as described below).
        Compositions can be sent as mole fractions or mass fractions in the z array depending on the value of iMass.*/

        public static extern void REFPROPdll(

       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hFld,                   // hFld [char ,in] :: Fluid string.
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hIn,                    // hIn [char ,in] :: Input string of properties being sent to the routine.
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hOut,                   // hOut [char ,in] :: Output string of properties to be calculated. 
       ref int iUnits,                                                          // iUnits [int ,in] :: The unit system to be used for the input and output properties (such as SI, English, etc.). MOLAR BASE SI = 20
       ref int iFlag,                                                           // iFlag [int ,in] :: Flag to specify if the routine SATSPLN should be called (where a value of 1 activates the call).
       ref int iMass,                                                           // iMass [int ,in] :: Specifies if the input composition is mole or mass based
       ref double a,                                                            // a [double ,in] :: First input property as specified in the hIn variable.
       ref double b,                                                            // b [double ,in] :: Second input property as specified in the hIn variable.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] z,       // z (20) [double ,in] :: Molar composition (array of size ncmax=20).
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] Output,  // Output (200) [double ,out] :: Array of properties specified by the hOut string (array of size
                                                                                // 200 dimensioned as double precision). The number -9999970 will be returned when errors occur, and the number
                                                                                // -9999990 will be returned when nothing was calculated. Read the comments in the ALLPROPS routine to fully understand the contents of this array.
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string hUnits,                 // hUnits [char ,out] :: The units for the first property in the Output array. Strings such as a fluid name may also be passed back in this position.
                                                                                // To obtain the units for all of the properties sent to the string, call the ALLPROPS routine instead.
       ref int iUCode,                                                          // iUCode [int ,out] :: Unit code that represents the units of the first property in the Output array. See below for further details.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,       // x (20) [double ,out] :: Composition of the liquid phase (array of mole fractions of size 20) for two-phase states on a mole or mass basis depending on the value of iMass.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] y,       // y (20) [double ,out] :: Composition of the vapor phase(array of mole fractions of size 20) for two-phase states on a mole or mass basis depending on the value of iMass.
       [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x3,      // x3 (20) [double ,out] :: Reserved for returning the composition of a second liquid phase for LLE or VLLE.
       ref double q,                                                            // q [double ,out] :: Vapor quality on a mole or mass basis depending on the value of iMass. (See subroutine ABFLSH for the definitions of values. returned for this variable).
                                                                                // To obtain the molar quality regardless of iMass, send “qmole” as an input in hIn, and vice-versa for “qmass”.
       ref int ierr,                                                            // ierr [int ,out] :: Error flag
       [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,                   // herr [char ,out] :: Error string (character*255)
       ref int hFld_length,                                                     // hFld_length [int ] :: length of variable hFld (default: 10000)
       ref int hIn_length,                                                      // hIn_length [int ] :: length of variable hIn (default: 255)
       ref int hOut_length,                                                     // hOut_length [int ] :: length of variable hOut (default: 255)
       ref int hUnits_length,                                                   // hUnits_length [int ] :: length of variable hUnits (default: 255)
       ref int herr_length);
    }

}
