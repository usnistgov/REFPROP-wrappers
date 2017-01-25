//
// IRefProp64 - Builds the IRefProp64.dll .Net assembly DLL that allows C# programs to use the native RefProp RefPrp64.dll. 
//              C#/.Net programs must add the MCS namesspace and Reference IRefProp64.dll to the Visual Studio Project in order
//              to use IRefProp64.dll.
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

/*! \mainpage IRefProp64.dll
 *
 * \section intro_sec Introduction
 *
 * IRefProp is an effective C# Interface for REFPROP. It’s an open source project (is/will be hosted on github) that contains a static wrapper class (IRefProp64) allowing access to the REFPROP native-code methods such as SETPATHdll(…), SETUPdll(…), SATSPLNdll(…),………etc.
 *
 * \section install_sec Installation
 * Make sure IRefProp64.dll is installed on your machine.
 * \subsection step1 Remarks
 *  
 * Using IRefProp requires adding the IRefProp64.dll reference to the project. Once the reference is added, the wrapped REFPROP methods can be accessed as follows: MCS.iRefProp64.method_name() where MCS is the namespace and iRefProp64 is the name of the main static class. Ex: MCS.IRefProp64.SETPATHdll(hpath, ref size);
 *
 */
 
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace MCS
{
    /// <summary>
    ///  This is C# Wrapper Class for REFPROP.
    /// </summary>
    /// 
    public static class IRefProp64
    {

        /*! \brief Brief description.
         *         Brief description continued.
         *
         *  Detailed description starts here.
         */
        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll")]
        public static extern void SETPATHdll(string htype, ref long ln);


        /*! \brief Brief description.
        *         Brief description continued.
        *
        *  Detailed description starts here.
        */

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        /*! \brief Brief description.
        *         Brief description continued.
        *
        *  Detailed description starts here.
        */
        public static extern void SETUPdll
             (
             ref long nComps,
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string hfld,
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string hfmix,
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string hrf,
             ref long ierr,
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
             ref long hfldLen,
             ref long hfmixLen,
             ref long hrfLen,
             ref long herrLen);

         /*! \brief Brief description.
       *         Brief description continued.
       *
       *  Detailed description starts here.
       */
        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void TPFLSHdll //!< a member function.
        (
            ref double t,
            ref double p,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,
            ref double d,
            ref double Dl,
            ref double Dv,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xliq,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xvap,
            ref double q,
            ref double ee,
            ref double h,
            ref double ss,
            ref double Cv,
            ref double Cp,
            ref double w,
            ref long ierr,
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
            ref long ln
            );

        /*! \brief Brief description.
        *         Brief description continued.
        *
        *  Detailed description starts here.
        */
        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void SETMIXdll 
        (
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string hmxnme,
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string hfmix,
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string hrf,
             ref long nccp, // number of fluids
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string hfiles,
             [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,
             ref long ierr,
             [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
             ref long hfldLen,
             ref long hfmixLen,
             ref long hrfLen,
             ref long hfilesLen,
             ref long herrLen);

        /*! \brief Brief description.
        *
        *  Detailed description starts here.
        */
        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void SATTdll //!< a member function.
        (
            ref double t,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,
            ref long kph,
            ref double p,
            ref double Dl,
            ref double Dv,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xliq,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xvap,
            ref long ierr,
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
            ref long herrleng
        );

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void SATSPLNdll
        (
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,
            ref long ierr,
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
            ref long ln
         );

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void QMASSdll
        (
            ref double qmol,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xl,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xv,
            ref double qkg,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xlkg,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xvkg,
            ref double wliq,
            ref double wvap,
            ref long ierr,
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
            ref long ln
        );

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void THERMdll
        (
            ref double t,
            ref double Dl,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,
            ref double p,
            ref double el,
            ref double hl,
            ref double sl,
            ref double cvl,
            ref double cpl,
            ref double wl,
            ref double hjt
        );

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void TRNPRPdll
        (
            ref double t,
            ref double d,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,
            ref double eta,
            ref double tcx,
            ref long ierr,
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
            ref long ln
        );

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void CRITPdll
        (
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,
            ref double tc,
            ref double pc,
            ref double Dc,
            ref long ierr,
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,
            ref long herrLen
         );

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void SATPdll
        (
            ref double p,   // => pressure
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,  // => composition array
            ref long kph,   // => phase flag 
            ref double t,   // <= temp
            ref double Dl,  //  => density of liquid
            ref double Dv,  //  => density of vapor
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xliq,  // <= liquid composition
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] xvap,  // <= vapor composition
            ref long ierr,  // <=>
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string herr,  // <=> error string
            ref long ln     // => length of error string
        );

        [DllImport(@"C:\Program Files (x86)\REFPROP\REFPRP64.dll", CharSet = CharSet.Ansi)]
        public static extern void WMOLdll
        (
           [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] double[] x,  // => composition array
           ref double wm     //
        );
    }
}
