// Refprop : Defines the entry point for the DLL application.
//
//
//  3/05/07    JPH    Version 1.0 Release (Refprop 7.1)
//  5/29/07    JPH    Version 1.1 - Use Refprop 8.0
//                    Included derived quantity functions:
//                    rho(P,H), rho(P,S), rho(T,H), rho(T,S),
//                    H(P,S), S(P,H), P(T,H), P(T,S)
//  1/22/18    JPH    This is a re-write of the RefProp add-in interface using
//                    the installed REFPROP application DLL and the header file
//                    from https://Github.com/CoolProp/REFPROP-Headers
//                    This will be version 2.0 of the RefProp Add-in DLL for Mathcad
//                    This version has several advantages:
//                    1) NIST REFPROP can be updated without having to update the
//                       Mathcad Add-in or recompile it.
//                    2) This version does not require the Intel Composer (Fortran) Run-time Libraries
//                    3) Version 2.0 currently uses NIST REFPROP 9.1.1.11 and should
//                       support version 10.0 when it is made available
//                    4) Version 2.0 is implemented in both Mathcad 15 and Mathcad Prime 4.0
//                    5) Version 2.0 is verified on Windows 10.
//

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef NOMINMAX         // Kill Windows' min() and max() macros in windows.h
#define NOMINMAX
#endif
#include "MCADINCL.H"    // Required to link with the Mathcad library
#undef NOMINMAX

#define REFPROP_IMPLEMENTATION
#define REFPROP_CSTYLE_REFERENCES
#include "REFPROP_lib.h"
#undef REFPROP_CSTYLE_REFERENCES
#undef REFPROP_IMPLEMENTATION

// Some additional constants for REFPROP to extend the list in REFPROP_lib.h...
// These are used to set string lengths for the Mathcad functions that
// build mixture strings and return fluid names and CASN strings
const static RP_SIZE_T namelengthshort = 12;
const static RP_SIZE_T namelengthlong = 80;
const static RP_SIZE_T casnlength = 12;

enum { MC_STRING = STRING }; // Substitute enumeration variable MC_STRING for STRING, use MC_STRING below and in the stub headers
#undef STRING                // STRING conflicts with a variable defined in RPstrings.cpp and is just generally a bad idea

#define NO_CPPFORMAT
#include <stdlib.h>
#include <stdio.h>
#include "RPstrings.h"       // Utility string functions for splitting and joining long strings at delimiters
#include <iostream>
#include <fstream>

// RefProp Mathcad Add-in Version
std::string rpVersion = "2.0.2";       // Mathcad Add-in version number

// Setup Dialog Window for debugging
HWND hwndDlg;  // Dialog handle for pop-up message boxes

enum EC {MUST_BE_REAL = 1, INSUFFICIENT_MEMORY, INTERRUPTED,                  // Mathcad Error Codes
         T_OUT_OF_RANGE, P_OUT_OF_RANGE, SATURATED,                           // Refprop Error Codes
         FLUID_NOT_FOUND, NO_MIX, UNCONVERGED,INVALID_MODEL,
         BAD_COMPONENT, NO_TRANSPORT, INFINITE_K,
         NO_SURFTEN, H_OUT_OF_RANGE, S_OUT_OF_RANGE,
         D_OUT_OF_RANGE, BAD_INPUT, INVALID_FLAG, X_SUM_NONUNITY,
         NO_UPPER_ROOT, TOO_MANY_COMPONENTS, BAD_MIX_STRING,
         BAD_MOLE_FRACTION, INDIV_COMPONENT, SATSPLN_FAILED, UNKNOWN,
         NUMBER_OF_ERRORS};                                                   // Dummy code for Error Count

// This is the list of errors that can be output if any of the functions fails.
// This table is registered below through the DLLEntryPoint
char * RPErrorMessageTable[NUMBER_OF_ERRORS] =
{
    (char *)("Argument must be real"),                      //  MUST_BE_REAL
    (char *)("Insufficient memory"),                        //  INSUFFICIENT_MEMORY
    (char *)("Interrupted"),                                //  INTERRUPTED
    (char *)("Temperature out of Range"),                   //  T_OUT_OF_RANGE
    (char *)("Pressure out of Range"),                      //  P_OUT_OF_RANGE
    (char *)("Saturated Conditions"),                       //  SATURATED
    (char *)("Fluid/Mixture not found"),                    //  FLUID_NOT_FOUND
    (char *)("Pure Fluids or Predefined Mixtures Only!"),   //  NO_MIX
    (char *)("Algorithm did not converge"),                 //  UNCONVERGED
    (char *)("Invalid model; must be \"EOS\". \"ETA\", or \"TCX\""),  //  INVALID_MODEL
    (char *)("Undefined Component Number"),                 //  BAD_COMPONENT
    (char *)("No transport equations for this fluid"),      //  NO_TRANSPORTPORT
    (char *)("At critical point; k is infinite"),           //  INFINITE_K
    (char *)("Unknown Surface Tension model"),              //  NO_SURFTEN
    (char *)("Enthalpy out of range"),                      //  H_OUT_OF_RANGE
    (char *)("Entropy out of range"),                       //  S_OUT_OF_RANGE
    (char *)("Density out of range"),                       //  D_OUT_OF_RANGE
    (char *)("Invalid Input; Must be 0 or 1"),              //  BAD_INPUT
    (char *)("Invalid Phase Flag; Must be 1 or 2"),         //  INVALID_FLAG
    (char *)("Mixture fractions don't sum to unity"),       //  X_SUM_NONUNITY
    (char *)("Upper root not supported when T < Tc"),       //  NO_UPPER_ROOT - no longer used
    (char *)("Too many components. Max is 20."),            //  TOO_MANY_COMPONENTS
    (char *)("Bad mixture string format: c1[mf1]&c2[mf2]...&cX[mfX]"),   // BAD_MIX_STRING
    (char *)("Mole fraction can't be converted"),           //  BAD_MOLE_FRACTION
    (char *)("Must specify individual component number"),   //  INDIV_COMPONENT
    (char *)("Saturation routine failed for mixture"),      //  SATSPLN_FAILED
    (char *)("Unknown Error"),                              //  UNKNOWN
    (char *)("Error Count - Not Used")                      //  NUMBER_OF_ERRORS
};

double x[20] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // initialize x[] for pure fluid
int ncomp = 1;                        // initialize ncomp to 1 for pure fluid
int extr  = 0;                        // initially disallow extrapolation to higher temps
// These parameters are set with a call to LIMITS when calling SETUP and can be used in any other calls
double Tmin;                          // Triple point temperature
double Tmax;                          // Max Temperature
double Dmax;                          // Max Density
double Pmax;                          // Max Pressure
double wmm;                           // Molar Mass
std::string err;                      // error string from REFPROP load DLL
std::string MixFileName;              // Mixture File Name Storage
std::string LastFluid = "";           // Mixture File Name Storage
std::string fluidPath;
char MixName[namelengthlong+1];       // Mixture Name from loaded mixture file.


// Include function stubs here.
// Each function is in its own file (ie. rp_setup.h)
//
// Utility Function headers
#include "setup.h"
#include "rp_getpath.h"
#include "rp_getNIST.h"
#include "rp_getvers.h"
#include "rp_getname.h"
#include "rp_getcasn.h"
#include "rp_extrapolate.h"
// Fluid Constant Function headers
#include "rp_wmol.h"
#include "rp_rgas.h"
#include "rp_tmax.h"
#include "rp_tmin.h"
#include "rp_pmax.h"
#include "rp_rhomax.h"
#include "rp_ttrip.h"
#include "rp_tcrit.h"
#include "rp_rhocrit.h"
#include "rp_ptrip.h"
#include "rp_pcrit.h"
#include "rp_LIMITS.h"
// Saturation Curve Function headers
#include "rp_tsatp.h"
#include "rp_psatt.h"
// Thermodynamic Property Function headers
#include "rp_rhofp.h"
#include "rp_rhoft.h"
#include "rp_rhogp.h"
#include "rp_rhogt.h"
#include "rp_rhotp.h"
#include "rp_hfp.h"
#include "rp_hft.h"
#include "rp_hgp.h"
#include "rp_hgt.h"
#include "rp_htp.h"
#include "rp_ufp.h"
#include "rp_uft.h"
#include "rp_ugp.h"
#include "rp_ugt.h"
#include "rp_utp.h"
#include "rp_sfp.h"
#include "rp_sft.h"
#include "rp_sgp.h"
#include "rp_sgt.h"
#include "rp_stp.h"
#include "rp_cvfp.h"
#include "rp_cvft.h"
#include "rp_cvgp.h"
#include "rp_cvgt.h"
#include "rp_cvtp.h"
#include "rp_cpfp.h"
#include "rp_cpft.h"
#include "rp_cpgp.h"
#include "rp_cpgt.h"
#include "rp_cptp.h"
#include "rp_wfp.h"
#include "rp_wft.h"
#include "rp_wgp.h"
#include "rp_wgt.h"
#include "rp_wtp.h"
// Transport Property Function headers
#include "rp_kfp.h"
#include "rp_kft.h"
#include "rp_kgp.h"
#include "rp_kgt.h"
#include "rp_ktp.h"
#include "rp_mufp.h"
#include "rp_muft.h"
#include "rp_mugp.h"
#include "rp_mugt.h"
#include "rp_mutp.h"
#include "rp_surften.h"

// Reverse Function headers
#include "rp_tph.h"
#include "rp_tps.h"
#include "rp_ths.h"
#include "rp_phs.h"
#include "rp_hps.h"
#include "rp_sph.h"
#include "rp_hts.h"
#include "rp_sth.h"
#include "rp_pth.h"
#include "rp_pts.h"
#include "rp_rhots.h"
#include "rp_rhoth.h"
#include "rp_rhops.h"
#include "rp_rhoph.h"
// Forward verification Functions
#include "rp_ptrho.h"
#include "rp_cvtrho.h"
#include "rp_wtrho.h"
#include "rp_strho.h"

#ifdef _WIN32
extern "C" BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved);
#endif

// NOTE: The IntelliSense error "Function definition for '_CRT_INIT' not found." above can be ignored.
//       The code will compile file and find _CRT_INIT at compile time with linking with the C Run Time libraries

extern "C" BOOL WINAPI DllEntryPoint (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        // For debugging if needed, but the add-in should load properly.
        // MessageBox(hwndDlg, "Trying to load NIST RefProp Add-In...\n\n", "NIST RefProp Add-In", 0);

        // DLL is attaching to the address space of the current process.
        //
        if (!_CRT_INIT(hDLL, dwReason, lpReserved))
        {
            return FALSE;
        }

        if (load_REFPROP(err, "", ""))
        {
            // Set fluids path to DLL path, one step above the fluids directory.
            size_t index = RPPath_loaded.find_last_of("\\");              // Find the last '\' before the DLL name
            fluidPath = RPPath_loaded.substr(0, index);                   // Get path to DLL without the DLL name
            char cPath[255] = { 0 };                                      // Initialize char array cpath
            std::copy(fluidPath.begin(), fluidPath.end(), cPath);         // Copy string fluidPath into c_str cpath.
            cPath[fluidPath.size()] = '\0';                               // Append with a null character (not sure if needed)
            SETPATHdll(cPath, 255);                                       // Set path to fluids library

            // Look for NIST_PATH environment variable...and set path if found
            // This process occurs once when Mathcad loads the DLL, but only if
            //  the NIST_PATH environment variable exists.

            const char* dbvar = getenv( "NIST_PATH" );                    // Get environment variable NIST_PATH if it exists
            if ( dbvar != NULL )                                          // If NIST_PATH was set (string not null)
            {                                                             //    Use extracted user specified path
                int dblen = static_cast<int>(strlen(dbvar));              //    get length of environment string
                char userPath[255];                                       //    SETPATH wants a char[255]
                strcpy(userPath, dbvar);                                  //    copy string to userPath[255]
                SETPATHdll(userPath, dblen);                              //    Set path to user specified library location
            }                                                             // else leave the path at default location

            // Get the version number of the REFPROP DLL and fix the RPVersion_loaded string to separate Major.Minor.Patch.Rev
            char hmsg[errormessagelength], hfmix[filepathlength], hrf[lengthofreference], hfld[componentstringlength];
            int icompn = -1, ivers;
            SETUPdll(&icompn, hfld, hfmix, hrf, &ivers, hmsg, componentstringlength, filepathlength, lengthofreference, errormessagelength);
            int vMajor = ivers/10000;
            ivers = ivers - vMajor*10000;
            int vMinor = ivers/1000;
            ivers = ivers - vMinor*1000;
            int vPatch = ivers/100;
            int vRev = ivers - vPatch*100;
            RPVersion_loaded = std::to_string(vMajor) + "." + std::to_string(vMinor) + "." + std::to_string(vPatch) + "." + std::to_string(vRev);

            // Use this pop-up window for debugging if needed
            //===============================================================================
            //std::string msg;
            // msg = "The NIST RefProp DLL was found.\n\nVersion: " + RPVersion_loaded;
            // msg.append("\n\nPath = ");
            // msg.append(RPPath_loaded);
            // MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);
        }
        else
        {
            MessageBox(hwndDlg, "Could not find and load REFPROP.DLL.\n\nMake sure that NIST RefProp 9.1.1 or later is installed.","NIST RefProp Add-In", 0);
            break;
        }

        // If all is well, Create the Error Message Table...
        //
        if (!CreateUserErrorMessageTable( hDLL, NUMBER_OF_ERRORS, RPErrorMessageTable ) )
            break;

        // Create the User Functions for Mathcad...
        //
        // Setup and information functions
        // ===============================
        //  if ( CreateUserFunction( hDLL, &rp_setup ) == NULL )    <<<<  This function should not be called directly in Mathcad.  It will get called
        //      break;                                              <<<<  through cSetup as the first step in each function that needs it.

        if ( CreateUserFunction( hDLL, &rp_getvers ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_getNIST ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_getpath ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_getname ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_getcasn ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_extrap ) == NULL )
            break;

        // Gas and Fluid Constants
        // ===============================
        if ( CreateUserFunction( hDLL, &rp_wmol ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rgas ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_ttrip ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_tcrit ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_pcrit ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhocrit ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_ptrip ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_tmax ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_tmin ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhomax ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_pmax ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_limits) == NULL)
            break;

        // Saturation Curve Definition
        // ===============================
        if ( CreateUserFunction( hDLL, &rp_tsatp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_psatt ) == NULL )
            break;

        // Thermodynamic Properties
        // ===============================
        if ( CreateUserFunction( hDLL, &rp_rhofp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhoft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhogp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhogt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhotp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_hfp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_hft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_hgp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_hgt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_htp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_ufp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_uft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_ugp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_ugt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_utp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_sfp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_sft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_sgp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_sgt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_stp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cvfp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cvft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cvgp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cvgt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cvtp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cpfp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cpft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cpgp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cpgt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cptp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_wfp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_wft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_wgp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_wgt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_wtp ) == NULL )
            break;

        // Transport Properties
        // ===============================
        if ( CreateUserFunction( hDLL, &rp_kfp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_kft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_kgp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_kgt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_ktp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_mufp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_muft ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_mugp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_mugt ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_mutp ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_surften ) == NULL )
            break;

        // Reverse Functions
        // ===============================
        if ( CreateUserFunction( hDLL, &rp_tph ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_tps ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_ths ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_phs ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_pth ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_pts ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_hps ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_sph ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_hts ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_sth ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhoth ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhots ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhoph ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_rhops ) == NULL )
            break;

        // Direct functions of T & Density for Verification (add to function reference?)
        // =============================================================================
        if ( CreateUserFunction( hDLL, &rp_ptrho ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_cvtrho ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_wtrho ) == NULL )
            break;

        if ( CreateUserFunction( hDLL, &rp_strho ) == NULL )
            break;

        break;
    }

    case DLL_THREAD_ATTACH:        // A new thread is being created in the current process.
    case DLL_THREAD_DETACH:        // A thread is exiting cleanly.
    case DLL_PROCESS_DETACH:       // The calling process is detaching the DLL from its address space.

        if (!_CRT_INIT(hDLL, dwReason, lpReserved))
        {
            unload_REFPROP(err);
            Sleep(1000);     // Attempt to keep CRT_INIT from detaching before all threads are closed;
                             // otherwise, the Mathcad process can hang and have to be closed manually.
            return FALSE;
        }
        break;
    }
    return TRUE;
}
