
#ifndef REFPROP_LIB_H
#define REFPROP_LIB_H

// The idea here is to have a common header for Windows 
// and Linux systems. The Windows branch should cover the
// functions provided by the .dll and the Linux part covers
// the compiled .so file. Name changes caused by gfortran 
// are respected and should be accounted for. 

// Get the platform identifiers, some overlap with "PlatformDetermination.h" from CoolProp's main repo 
// See also http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
#    define __RPISWINDOWS__
#elif defined(__APPLE__)
#    define __RPISAPPLE__
#elif defined(__linux) || defined(__unix) || defined(__posix)
#    define __RPISLINUX__
#elif defined(__powerpc__)
#    define __RPISPOWERPC__
#else
#    pragma error
#endif

// Define compiler specific calling conventions
// for the shared library.
#if defined(__RPISWINDOWS__)
#    define RPCALLCONV __stdcall
#else
#    define RPCALLCONV
#endif

#if defined(__cplusplus)
// Function prototypes that are implemented below when REFPROP_IMPLEMENTATION macro is defined
// before this header is included.  REFPROP_IMPLEMENTATION shall be implemented in ONLY ONE
// compilation unit
#include <string>
bool load_REFPROP(std::string &err, const std::string &shared_library_path = "", const std::string &shared_library_name = "");
bool unload_REFPROP(std::string &err);
std::size_t REFPROP_address();
#endif

// ******************************************
// ******************************************
// Headers (platform specific and otherwise)
// ******************************************
// ******************************************

#ifdef REFPROP_IMPLEMENTATION

    #ifndef __cplusplus
    #error REFPROP_IMPLEMENTATION can only be used in C++
    #endif

    #if defined(__RPISPOWERPC__)
        #include <cstring>
    #elif defined(__RPISLINUX__) || defined(__RPISAPPLE__)
        #include <cstring>
        #include <dlfcn.h>
    #elif defined(__RPISWINDOWS__)

        #if defined(_MSC_VER)
        #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
        #endif
        #endif

        #ifndef NOMINMAX
            #define NOMINMAX
            #include <windows.h>
            #undef NOMINMAX
        #else 
            #include <windows.h>
        #endif

        #if defined(_MSC_VER)
        #undef _CRT_SECURE_NO_WARNINGS
        #endif

        
    #else
        #pragma error
    #endif

    #include <sstream>
    #include <string>
    #include <algorithm>

#endif

/* See http://stackoverflow.com/a/148610
 * See http://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c#202511
 * This will be used to generate function names, pointers, etc. below
 */
#define LIST_OF_REFPROP_FUNCTION_NAMES \
    X(ABFL1dll) \
    X(ABFL2dll) \
    X(ABFLSHdll) \
    X(ABFLASHdll) \
    X(AGdll) \
    X(ALLPROPS0dll) \
    X(ALLPROPS1dll) \
    X(ALLPROPS20dll) \
    X(ALLPROPSdll) \
    X(B12dll) \
    X(BLCRVdll) \
    X(CCRITdll) \
    X(CHEMPOTdll) \
    X(CP0dll) \
    X(CRITPdll) \
    X(CRTPNTdll) \
    X(CSATKdll) \
    X(CSTARdll) \
    X(CV2PKdll) \
    X(CVCPKdll) \
    X(CVCPdll) \
    X(DBDTdll) \
    X(DBFL1dll) \
    X(DBFL2dll) \
    X(DDDPdll) \
    X(DDDTdll) \
    X(DEFL1dll) \
    X(DEFLSHdll) \
    X(DERVPVTdll) \
    X(DHD1dll) \
    X(DHFL1dll) \
    X(DHFLSHdll) \
    X(DIELECdll) \
    X(DLSATKdll) \
    X(DPDD2dll) \
    X(DPDDdll) \
    X(DPDTdll) \
    X(DPTSATKdll) \
    X(DQFL2dll) \
    X(DSD1dll) \
    X(DSFL1dll) \
    X(DSFLSHdll) \
    X(DVSATKdll) \
    X(ENTHALdll) \
    X(ENTROdll) \
    X(ERRMSGdll) \
    X(ESFLSHdll) \
    X(EXCESSdll) \
    X(FGCTY2dll) \
    X(FGCTYdll) \
    X(FLAGSdll) \
    X(FPVdll) \
    X(FUGCOFdll) \
    X(GERG04dll) \
    X(GERG08dll) \
    X(GETENUMdll) \
    X(GETFIJdll) \
    X(GETKTVdll) \
    X(GETMODdll) \
    X(GETREFDIRdll) \
    X(GIBBSdll) \
    X(HEATFRMdll) \
    X(HEATdll) \
    X(HMXORDERdll) \
    X(HSFL1dll) \
    X(HSFLSHdll) \
    X(IDCRVdll) \
    X(INFOdll) \
    X(JICRVdll) \
    X(JTCRVdll) \
    X(LIMITKdll) \
    X(LIMITSdll) \
    X(LIMITXdll) \
    X(LIQSPNDLdll) \
    X(MASSFLUXdll) \
    X(MAXPdll) \
    X(MAXTdll) \
    X(MELTKdll) \
    X(MELTPdll) \
    X(MELTTdll) \
    X(MLTH2Odll) \
    X(NAMEdll) \
    X(PASSCMNdll) \
    X(PDFL1dll) \
    X(PDFLSHdll) \
    X(PEFL1dll) \
    X(PEFLSHdll) \
    X(PHFL1dll) \
    X(PHFLSHdll) \
    X(PHI0dll) \
    X(PHIDERVdll) \
    X(PHIHMXdll) \
    X(PHIKdll) \
    X(PHIMIXdll) \
    X(PHIXdll) \
    X(PQFLSHdll) \
    X(PREOSdll) \
    X(PRESSdll) \
    X(PSATKdll) \
    X(PSFL1dll) \
    X(PSFLSHdll) \
    X(PUREFLDdll) \
    X(QMASSdll) \
    X(QMOLEdll) \
    X(RDXHMXdll) \
    X(REDXdll) \
    X(REFPROP1dll) \
    X(REFPROP2dll) \
    X(REFPROPdll) \
    X(RESIDUALdll) \
    X(RIEMdll) \
    X(RMIX2dll) \
    X(RPVersion) \
    X(SATDdll) \
    X(SATESTdll) \
    X(SATEdll) \
    X(SATGUESSdll) \
    X(SATGVdll) \
    X(SATHdll) \
    X(SATPESTdll) \
    X(SATPdll) \
    X(SATSPLNdll) \
    X(SATSdll) \
    X(SATTESTdll) \
    X(SATTPdll) \
    X(SATTdll) \
    X(SETAGAdll) \
    X(SETFLUIDSdll) \
    X(SETKTVdll) \
    X(SETMIXTUREdll) \
    X(SETMIXdll) \
    X(SETMODdll) \
    X(SETNCdll) \
    X(SETPATHdll) \
    X(SETREFDIRdll) \
    X(SETREFdll) \
    X(SETUPdll) \
    X(SPLNROOTdll) \
    X(SPLNVALdll) \
    X(STNdll) \
    X(SUBLPdll) \
    X(SUBLTdll) \
    X(SURFTdll) \
    X(SURTENdll) \
    X(TDFLSHdll) \
    X(TEFL1dll) \
    X(TEFLSHdll) \
    X(THERM0dll) \
    X(THERM2dll) \
    X(THERM3dll) \
    X(THERMdll) \
    X(THFL1dll) \
    X(THFLSHdll) \
    X(TPFL2dll) \
    X(TPFLSHdll) \
    X(TPRHOPRdll) \
    X(TPRHOdll) \
    X(TQFLSHdll) \
    X(TRNPRPdll) \
    X(TSATDdll) \
    X(TSATPdll) \
    X(TSFL1dll) \
    X(TSFLSHdll) \
    X(UNSETAGAdll) \
    X(VAPSPNDLdll) \
    X(VIRBAdll) \
    X(VIRBCDdll) \
    X(VIRBCD12dll) \
    X(VIRBdll) \
    X(VIRCAdll) \
    X(VIRCdll) \
    X(VIRTAUdll) \
    X(WMOLIdll) \
    X(WMOLdll) \
    X(XMASSdll) \
    X(XMOLEdll)

// By default the functions are defined to be static and not visible to other compilation units
// You could for instance set this macro to "" which would make the symbols available
#ifndef REFPROP_FUNCTION_MODIFIER
#define REFPROP_FUNCTION_MODIFIER static
#endif

// define new macros for function names
// http://stackoverflow.com/questions/195975/how-to-make-a-char-string-from-a-c-macros-value
#define STR_VALUE(arg)      #arg
#define FUNCTION_NAME(name) STR_VALUE(name)
#define STRINGIFY(name) STR_VALUE(name)

// In C++, references are done using double &, and in C, using double *
// C++ can use C-style references or pointers
// C can only use pointers
#if !defined(__cplusplus) || defined(REFPROP_CSTYLE_REFERENCES)
    // For C compilation, must do double *, int * since C doesn't understand double &, SIZE_T &
    #define DOUBLE_REF double *
    #define INT_REF int *
#else
    #define DOUBLE_REF double &
    #define INT_REF int &
#endif

// We must determine the size of std::size_t because that is the variable type that should be used to define the string lengths
// See also https://software.intel.com/en-us/articles/passing-character-string-in-intel-64-mixed-fortranc-project
#if !defined(__cplusplus)
    #if !defined(SIZE_T_TYPE)
    #error The preprocessor macro SIZE_T_TYPE must be defined and give the unsigned integer variable type that is the same size (in bytes) as std::size_t
    #endif
    #define RP_SIZE_T SIZE_T_TYPE
#else 
    #include <cstdio>
    #define RP_SIZE_T std::size_t
#endif

// Some string length constants for REFPROP...
const static RP_SIZE_T refpropcharlength = 255;
const static RP_SIZE_T filepathlength = 255;
const static RP_SIZE_T lengthofreference = 3;
const static RP_SIZE_T errormessagelength = 255;
const static RP_SIZE_T ncmax = 20;
const static RP_SIZE_T numparams = 72;
const static RP_SIZE_T maxcoefs = 50;
const static RP_SIZE_T componentstringlength = 10000; // Length of component_string (see PASS_FTN.for from REFPROP)
const static RP_SIZE_T versionstringlength = 1000; 

#ifdef REFPROP_LIB_NAMESPACE
namespace REFPROP_LIB_NAMESPACE {
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // For C calling conventions, replaced all "double &" with "double *", and "int &" with "int *"
    
    #define ABFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,char *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define ABFL2dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,INT_REF,char *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define ABFLSHdll_ARGS char *,DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define ABFLASHdll_ARGS char *,DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define AGdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF
    #define ALLPROPS0dll_ARGS INT_REF,int *,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define ALLPROPS1dll_ARGS char *,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define ALLPROPS20dll_ARGS char *,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,char *,int *,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define ALLPROPSdll_ARGS char *,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,char *,int *,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define B12dll_ARGS DOUBLE_REF,double *,DOUBLE_REF
    #define BLCRVdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define CCRITdll_ARGS DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define CHEMPOTdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define CP0dll_ARGS DOUBLE_REF,double *,DOUBLE_REF
    #define CRITPdll_ARGS double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define CRTPNTdll_ARGS double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define CSATKdll_ARGS INT_REF,DOUBLE_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define CSTARdll_ARGS DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define CV2PKdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define CVCPKdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define CVCPdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF
    #define DBDTdll_ARGS DOUBLE_REF,double *,DOUBLE_REF
    #define DBFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,char *,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define DBFL2dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,char *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define DDDPdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define DDDTdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define DEFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DEFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DERVPVTdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define DHD1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define DHFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DHFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DIELECdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define DLSATKdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DPDD2dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define DPDDdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define DPDTdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define DPTSATKdll_ARGS INT_REF,DOUBLE_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DQFL2dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define DSD1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define DSFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DSFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define DVSATKdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define ENTHALdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define ENTROdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define ERRMSGdll_ARGS INT_REF,char *,RP_SIZE_T
    #define ESFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define EXCESSdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define FGCTY2dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define FGCTYdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,double *
    #define FLAGSdll_ARGS char *,INT_REF,INT_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define FPVdll_ARGS DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define FUGCOFdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define GERG04dll_ARGS INT_REF,INT_REF,INT_REF,char *,RP_SIZE_T
    #define GERG08dll_ARGS INT_REF,INT_REF,INT_REF,char *,RP_SIZE_T
    #define GETENUMdll_ARGS INT_REF,char *,INT_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define GETFIJdll_ARGS char *,double *,char *,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define GETKTVdll_ARGS INT_REF,INT_REF,char *,double *,char *,char *,char *,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define GETMODdll_ARGS INT_REF,char *,char *,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define GETREFDIRdll_ARGS char *,RP_SIZE_T
    #define GIBBSdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF
    #define HEATFRMdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define HEATdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define HMXORDERdll_ARGS INT_REF,INT_REF,char *,char *,INT_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define HSFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define HSFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define IDCRVdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define INFOdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define JICRVdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define JTCRVdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define LIMITKdll_ARGS char *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define LIMITSdll_ARGS char *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,RP_SIZE_T
    #define LIMITXdll_ARGS char *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define LIQSPNDLdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define MASSFLUXdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define MAXPdll_ARGS double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define MAXTdll_ARGS double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define MELTKdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define MELTPdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define MELTTdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define MLTH2Odll_ARGS DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define NAMEdll_ARGS INT_REF,char *,char *,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define PASSCMNdll_ARGS char *,INT_REF,INT_REF,INT_REF,char *,INT_REF,DOUBLE_REF,double *,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define PDFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PDFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PEFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PEFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PHFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PHFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PHI0dll_ARGS INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define PHIDERVdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,double *,INT_REF,char *,RP_SIZE_T
    #define PHIHMXdll_ARGS INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define PHIKdll_ARGS INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define PHIMIXdll_ARGS INT_REF,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define PHIXdll_ARGS INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define PQFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PREOSdll_ARGS INT_REF
    #define PRESSdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define PSATKdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PSFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PSFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define PUREFLDdll_ARGS INT_REF
    #define QMASSdll_ARGS DOUBLE_REF,double *,double *,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define QMOLEdll_ARGS DOUBLE_REF,double *,double *,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define RDXHMXdll_ARGS INT_REF,INT_REF,INT_REF,double *,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define REDXdll_ARGS double *,DOUBLE_REF,DOUBLE_REF
    #define REFPROP1dll_ARGS char *,char *,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define REFPROP2dll_ARGS char *,char *,char *,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define REFPROPdll_ARGS char *,char *,char *,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,char *,INT_REF,double *,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define RESIDUALdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define RIEMdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF
    #define RMIX2dll_ARGS double *,DOUBLE_REF
    #define RPVersion_ARGS char *,RP_SIZE_T
    #define SATDdll_ARGS DOUBLE_REF,double *,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define SATESTdll_ARGS INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,double *,INT_REF,char *,RP_SIZE_T
    #define SATEdll_ARGS DOUBLE_REF,double *,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SATGUESSdll_ARGS INT_REF,INT_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,INT_REF,char *,RP_SIZE_T
    #define SATGVdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define SATHdll_ARGS DOUBLE_REF,double *,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SATPESTdll_ARGS DOUBLE_REF,double *,INT_REF,DOUBLE_REF,double *,INT_REF,char *,RP_SIZE_T
    #define SATPdll_ARGS DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define SATSPLNdll_ARGS double *,INT_REF,char *,RP_SIZE_T
    #define SATSdll_ARGS DOUBLE_REF,double *,INT_REF,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SATTESTdll_ARGS DOUBLE_REF,double *,INT_REF,DOUBLE_REF,double *,INT_REF,char *,RP_SIZE_T
    #define SATTPdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SATTdll_ARGS DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,INT_REF,char *,RP_SIZE_T
    #define SETAGAdll_ARGS INT_REF,char *,RP_SIZE_T
    #define SETFLUIDSdll_ARGS char *,INT_REF,RP_SIZE_T
    #define SETKTVdll_ARGS INT_REF,INT_REF,char *,double *,char *,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define SETMIXTUREdll_ARGS char *,double *,INT_REF,RP_SIZE_T
    #define SETMIXdll_ARGS char *,char *,char *,INT_REF,char *,double *,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define SETMODdll_ARGS INT_REF,char *,char *,char *,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define SETNCdll_ARGS INT_REF
    #define SETPATHdll_ARGS char *,RP_SIZE_T
    #define SETREFDIRdll_ARGS char *,RP_SIZE_T
    #define SETREFdll_ARGS char *,INT_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T,RP_SIZE_T
    #define SETUPdll_ARGS INT_REF,char *,char *,char *,INT_REF,char *,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T,RP_SIZE_T
    #define SPLNROOTdll_ARGS INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SPLNVALdll_ARGS INT_REF,INT_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define STNdll_ARGS DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SUBLPdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SUBLTdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SURFTdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define SURTENdll_ARGS DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TDFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TEFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TEFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define THERM0dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define THERM2dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define THERM3dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define THERMdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define THFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define THFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TPFL2dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TPFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TPRHOPRdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF
    #define TPRHOdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,INT_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TQFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TRNPRPdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TSATDdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TSATPdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TSFL1dll_ARGS DOUBLE_REF,DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define TSFLSHdll_ARGS DOUBLE_REF,DOUBLE_REF,double *,INT_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,double *,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define UNSETAGAdll_ARGS 
    #define VAPSPNDLdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,INT_REF,char *,RP_SIZE_T
    #define VIRBAdll_ARGS DOUBLE_REF,double *,DOUBLE_REF
    #define VIRBCD12dll_ARGS DOUBLE_REF,double *,INT_REF,double *,double *,double *,double *
    #define VIRBCDdll_ARGS DOUBLE_REF,double *,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF,DOUBLE_REF
    #define VIRBdll_ARGS DOUBLE_REF,double *,DOUBLE_REF
    #define VIRCAdll_ARGS DOUBLE_REF,double *,DOUBLE_REF
    #define VIRCdll_ARGS DOUBLE_REF,double *,DOUBLE_REF
    #define VIRTAUdll_ARGS DOUBLE_REF,double *,INT_REF,double *,double *,double *,double *,INT_REF,char *,RP_SIZE_T
    #define WMOLIdll_ARGS INT_REF,DOUBLE_REF
    #define WMOLdll_ARGS double *,DOUBLE_REF
    #define XMASSdll_ARGS double *,double *,DOUBLE_REF
    #define XMOLEdll_ARGS double *,double *,DOUBLE_REF

#if !defined(REFPROP_ONLY_MACROS)

    #if !defined(REFPROP_PROTOTYPES)
    
        // ***MAGIC WARNING**!! X Macros in use
        // See http://stackoverflow.com/a/148610
        // See http://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c#202511
        
        // Further information here:
        // http://www.gershnik.com/tips/cpp.asp
    
        // Define type names for each function
        // Each will look something like: typedef void (RPCALLCONV ACTVYdll_TYPE)(ACTVYdll_ARGS);
        // 
        // The ## are needed to escape the _ character in the variable names
        // 
        #define X(name) typedef void (RPCALLCONV name ## _TYPE)(name ## _ARGS);
            LIST_OF_REFPROP_FUNCTION_NAMES
        #undef X
        
        // Define explicit function pointers for each function
        // Each will look something like: typedef RPVersion_TYPE * RPVersion_POINTER;
        // 
        // The ## are needed to escape the _ character in the variable names
        // 
        #define X(name) typedef name ## _TYPE * name ## _POINTER;
            LIST_OF_REFPROP_FUNCTION_NAMES
        #undef X
        
        // Define functions as pointers
        // Each will look something like: SETPATHdll_POINTER SETPATHdll;
        //
        #ifdef REFPROP_IMPLEMENTATION
            // Here we want to define them to be used in this file only
            // since we are adding all the accessor functions
            // Defined as static (by default, but see above) to ensure they are only in this file
            #define X(name) REFPROP_FUNCTION_MODIFIER name ## _POINTER name;
                LIST_OF_REFPROP_FUNCTION_NAMES
            #undef X
        #else
            // Otherwise, for other files adding the header, define the pointers to be extern, 
            // so they can see the pointers in a read-only fashion
            #define X(name)  extern name ## _POINTER name;
                LIST_OF_REFPROP_FUNCTION_NAMES
            #undef X
        #endif
    #else
        // Otherwise this header becomes just a set of prototypes for the functions
        // defining the input parameters
        //
        // The ## are needed to escape the _ character in the macro name in the intermediate expansion
        // 
        #define X(name) extern void name(name ## _ARGS);
            LIST_OF_REFPROP_FUNCTION_NAMES
        #undef X
    #endif

#endif  // (REFPROP_ONLY_MACROS)

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

//******************************************************************************
//******************************************************************************
//*********************  REFPROP IMPLEMENTATION  *******************************
//******************************************************************************
//******************************************************************************

// Define this preprocessor flag to also define the function prototypes and connect function pointers
// N.B. Define this macro in only one location
// N.B. This is C++-only, though with some work it could be made C compatible
#ifdef REFPROP_IMPLEMENTATION

    #if defined(__powerpc__)
        static void *RefpropdllInstance=NULL;
    #elif defined(__RPISLINUX__) || defined(__RPISAPPLE__)
        static void *RefpropdllInstance=NULL;
    #elif defined(__RPISWINDOWS__)
        static HINSTANCE RefpropdllInstance=NULL;
    #else
        #pragma error
    #endif

    // Define the default library names
    const static std::string shared_lib_WIN64 = "REFPRP64.dll";
    const static std::string shared_lib_WIN32 = "REFPROP.dll";
    const static std::string shared_lib_LINUX = "librefprop.so";
    const static std::string shared_lib_APPLE = "librefprop.dylib";

    static std::string RPVersion_loaded = "";
    static std::string RPPath_loaded = "";

    enum DLLNameManglingStyle{ NO_NAME_MANGLING = 0, LOWERCASE_NAME_MANGLING, LOWERCASE_AND_UNDERSCORE_NAME_MANGLING };

    const std::string& get_shared_lib()
    {
#if defined(__RPISWINDOWS__)
    #if defined(_WIN64) || defined(__WIN64__)
        return shared_lib_WIN64;
    #elif defined(_WIN32) || defined(__WIN32__)
        return shared_lib_WIN32;
    #else
        if (sizeof(void*) == 8) { // Assume 64bit
            return shared_lib_WIN64;
        } else {
            return shared_lib_WIN32;
        }
    #endif  
#elif defined(__RPISLINUX__)
        return shared_lib_LINUX;
#elif defined(__RPISAPPLE__)
        return shared_lib_APPLE;
#endif
    }
    
    inline std::string RPlower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    void *getFunctionPointer(const char *name, DLLNameManglingStyle mangling_style = NO_NAME_MANGLING)
    {
        std::string function_name;
        switch(mangling_style)
        {
            case NO_NAME_MANGLING:
                function_name = name; break;
            case LOWERCASE_NAME_MANGLING:
                function_name = RPlower(name); break;
            case LOWERCASE_AND_UNDERSCORE_NAME_MANGLING:
                function_name = RPlower(name) + "_"; break;
        }
        #if defined(__RPISWINDOWS__)
            return (void *) GetProcAddress(RefpropdllInstance, function_name.c_str());
        #elif defined(__RPISLINUX__)
            return dlsym(RefpropdllInstance, function_name.c_str());
        #elif defined(__RPISAPPLE__)
            return dlsym(RefpropdllInstance, function_name.c_str());
        #else
            return NULL;
        #endif
    }

    /**
     * @brief Set the function pointers in the DLL/SO
     */
    bool setFunctionPointers(std::string &err)
    {
        if (RefpropdllInstance==NULL)
        { 
            err = "REFPROP is not loaded, make sure you call this function after loading the library using load_REFPROP.";
            return false;
        }
        /* First determine the type of name mangling in use.
         * A) RPVersion -> RPVersion
         * B) RPVersion -> rpversion
         * C) RPVersion -> rpversion_
         */
        DLLNameManglingStyle mangling_style = NO_NAME_MANGLING; // defaults to no mangling

        SETUPdll = (SETUPdll_POINTER) getFunctionPointer("SETUPdll"); // try NO_NAME_MANGLING
        if (SETUPdll == NULL) 
        { 
            SETUPdll = (SETUPdll_POINTER) getFunctionPointer("setupdll"); // try LOWERCASE_NAME_MANGLING
            if (SETUPdll == NULL) 
            {
                SETUPdll = (SETUPdll_POINTER) getFunctionPointer("setupdll_"); // try LOWERCASE_AND_UNDERSCORE_NAME_MANGLING
                if (SETUPdll == NULL) 
                {
                    err = "Could not load the symbol SETUPdll or any of its mangled forms; REFPROP shared library broken.";
                    return false;
                } else {
                    mangling_style = LOWERCASE_AND_UNDERSCORE_NAME_MANGLING;
                }
            } else {
                mangling_style = LOWERCASE_NAME_MANGLING;
            }
        } else {
            mangling_style = NO_NAME_MANGLING;
        } 

        /* Set the pointers, platform independent
         *
         * Example: RPVersion = (RPVersion_POINTER) getFunctionPointer(STRINGIFY(RPVersion));
         *
         * ***MAGIC WARNING**!! X Macros in use
         * See http://stackoverflow.com/a/148610
         * See http://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c#202511
         */
        #define X(name)  name = (name ## _POINTER) getFunctionPointer(STRINGIFY(name), mangling_style);
           LIST_OF_REFPROP_FUNCTION_NAMES
        #undef X

        return true;
    }

    // See http://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
    inline bool RP_ends_with(const std::string & value, const std::string & ending) {
        if (value.empty()) return false;
        if (ending.empty()) return false;
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    std::string RP_join_path(const std::string &one, const std::string &two) {
        std::string result;
        std::string separator;
        #if defined(__RPISWINDOWS__)
            separator = "\\";
        #elif defined(__RPISLINUX__)
            separator = "/";
        #elif defined(__RPISAPPLE__)
            separator = "/";
        #endif
        if (!RP_ends_with(one, separator) && !one.empty()) {
            result = one + separator;
        } else {
            result = one;
        }
        result.append(two);
        return result;
    }

    bool load_REFPROP(std::string &err, const std::string &shared_library_path, const std::string &shared_library_name)
    {
        // If REFPROP is not loaded
        if (RefpropdllInstance == NULL)
        {
            // Load it
            std::string msg;
            std::string shared_lib;
            if (shared_library_name.empty()) 
            {
                shared_lib = get_shared_lib();
            } else {
                shared_lib = shared_library_name;
            }
            #if defined(__RPISWINDOWS__)
                // Load the DLL
                TCHAR refpropdllstring[_MAX_PATH];
                strcpy((char*)refpropdllstring, RP_join_path(shared_library_path, shared_lib).c_str());
                RefpropdllInstance = LoadLibrary(refpropdllstring);
                // Error handling
                if (RefpropdllInstance == NULL)
                {
                    std::stringstream msg_stream;
                    msg_stream << GetLastError(); // returns error
                    msg = msg_stream.str();
                } else {
                    TCHAR dllPath[_MAX_PATH];
                    HMODULE hModule;
                    hModule = GetModuleHandle(refpropdllstring);
                    GetModuleFileName(hModule, dllPath, _MAX_PATH);
                    #ifndef UNICODE
                        msg = dllPath;
                    #else
                        std::wstring wStr = dllPath;
                        msg = std::string(wStr.begin(), wStr.end());
                    #endif
                    RPPath_loaded = msg;
                }
            #elif ( defined(__RPISLINUX__) || defined(__RPISAPPLE__) )
                // Load library
                RefpropdllInstance = dlopen (RP_join_path(shared_library_path, shared_lib).c_str(), RTLD_NOW); 
                // Error handling
                if (RefpropdllInstance == NULL) 
                {
                    const char *errstr = dlerror();
                    if (errstr != NULL) 
                    {
                        msg = errstr;
                    }
                } else {
                    RPPath_loaded = RP_join_path(shared_library_path, shared_lib);
                }
            #else
                RefpropdllInstance = NULL;
                RPPath_loaded = "";
                msg = "Something is wrong with the platform definition, you should not end up here.";
            #endif

            if (RefpropdllInstance == NULL)
            {
                err = "Could not load REFPROP (" + shared_lib + ") due to: " + msg + ". ";
                err.append("Make sure the library is in your system search path. ");
                err.append("In case you run 64bit Windows and you have a REFPROP license, try installing the 64bit DLL from NIST. ");
                return false;
            }
            if (setFunctionPointers(err) != true)
            {
                err = "There was an error setting the REFPROP function pointers, check types and names in header file.";
                return false;
            }
            char rpv[versionstringlength] = { '\0' };
            RPVersion(rpv, versionstringlength);
            RPVersion_loaded = rpv;
            return true;
        }
        return true;
    }

    bool unload_REFPROP(std::string &err)
    {
        // If REFPROP is loaded
        if (RefpropdllInstance != NULL) {
#if defined(__RPISWINDOWS__)
            std::stringstream msg_stream;
            if (!FreeLibrary(RefpropdllInstance)) 
            {
                msg_stream << GetLastError(); // returns error
                err = msg_stream.str();
                return false;
            }
#elif (defined(__RPISLINUX__) || defined(__RPISAPPLE__))
            if (dlclose(RefpropdllInstance) != 0) 
            {
                const char* errstr = dlerror();
                if (errstr != NULL) 
                {
                    err = errstr;
                }
                return false;
            }
#else
            err = "Something is wrong with the platform definition, you should not end up here.";
            return false;
#endif
            RefpropdllInstance = NULL;
            RPVersion_loaded.clear();
            RPPath_loaded.clear();
        }
        return true;
    }

    /// The address of the REFPROP instance
    std::size_t REFPROP_address() {
        return reinterpret_cast<std::size_t>(RefpropdllInstance);
    }

#endif // REFPROP_IMPLEMENTATION

#ifdef REFPROP_LIB_NAMESPACE
}; /* namespace REFPROP_LIB_NAMESPACE */
#endif

#endif // REFPROP_LIB_H
