//===================================================================================================================
// This file contains the wrapper functions for the various calls to REFPROPdll.  Because Mathcad Custom Functions
// cannot overload or return variable parameter lists, three variations are created to return different return types.
//
//     rp_REFPROP(hFld, hIn, hOut, a, b)  - Sets fluid and returns an array of numerical values specified in hOut,
//                                          at the state point specified in hIn with state values a ad b.
//     rp_REFPROP1(hIn, hOut, a, b)       - Takes no fluid name (uses existing) and returns a single scalar as
//                                          specified in hOut, at the state point specified in hIn with state
//                                          values a and b.  Fluid/mixture must be set with previous call to 
//                                          rp_REFPROP, rp_SETFLUIDS (separate header file), or rp_SETMIXTURE
//                                          (separate header file).
//     rp_REFPROPc(hFld, hIn, hOut, a, b) - Sets fluid (if non-blank) and returns single character string specified
//                                          in hOut, at the state point specified in hIn with state values a ad b.
//                                          Most string outputs do not require a state point, so the hIn, a, and b
//                                          parameters are ignored and can be specified as "", 0, and 0, resp.  Only
//                                          one or two output requests require a state point (e.g. hOut = "PHASE")
//
// In all cases, REFPROPdll (or REFPROP1dll) are called with sensible defaults for the omitted parameters, using the
// following assumptions:
//
//     1. hFld can be an empty string or single space to continue using the previously loaded fluid in calls to
//        rp_REFPROP and rp_REFPROc.  This is the assumption for rp_REFPROP1 (fluid cannot be changed).
//     2. Unit system is fixed and not included in the Mathcad wrapper, since Mathcad can handle the input/output
//        unit conversions required.  Required units are modified "MASS SI" (P in [MPa] and E in [kJ]) with
//        mods to thermal conductivity [W/m-K] and surface tension [N/m]. See RPMassUnits.h for more details.
//     3. All inputs/outputs are mass-based (iMass = 1) assuming the user can perform mass/molar conversions
//        in Mathcad if needed.
//     4. SATSPLN is always called (iFlag = 1) for mixtures
//     5. Composition, z, is set automatically through REFPROP fluid strings, or mixture files loaded through 
//        rp_REFPROP or rp_SETMIXTURE calls and maintained internally in the DLL until the fluid is changed or
//        the composition is manually modified through calls to utility routines rp_getx/rp_setx.
//     6. Output(200) array is returned as a mathcad array with reduced length to match the number of outputs
//        requested in hOut and adjusted for output variables that can return multiple values for mixtures.
//     7. hUnits is ignored except for calls to rp_REFPROPc, where a string return is requested.
//     8. iUCode is ignored except when returning "DLL#", "UNITNUMB", or "UNITS" as the requested value, in
//        which case Output is ignored.
//     9. Output parameters x, y, and x3 are ignored.  Vapor/liquid compositions x and y can be returned in the
//        OUTPUT array by including XLIQ or XVAP (or various forms) in the hOut string.
//    10. Output parameter q is ignored (only the Output array can be returned). Vapor quality in two-phase states
//        can be returned in the Output array by requesting QMASS or QMOLE in the hOut string.
//    11. Output parameters ierr and herr are processed automatically by the code, returning error messages and or
//        warnings if ierr is non-zero.
//    12. Remaining string length parameters (hFld_length, etc.) are handled automatically by the code.
//
// =================================================================================================================
// In addition to the above specialized REFPROPdll calls, there are a number of utility functions that handle string
// parsing of hFld, hIn, and hOut strings for both code simplification and to make them available within Mathcad
// for speedup of Mathcad level unit handling functions (REFPROP).  These extra utility functions include:
//
//    GetDel (rp_GetDel)   - Retrieves the delimiter used by the user in hOut strings.
//    IsChar (rp_IsChar)   - Boolean that returns true if an hOut prop is to be returned as a character string.
//    IsMulti (rp_IsMulti) - Boolean that returns true if an hOut prop can return multiple values (e.g. mixtures).
//    strSplitOut          - Specialized version of strsplit (RPstrings.h) that expands hOut array to include
//                           additional elements when multiple values returned for mixtures.  Only called by rp_UNITS
//                           to return unit codes on return values.
//    rp_UNITS             - Specialized call to ALLPROPSdll to retrieve the numerical unit codes on each of the
//                           requested output properties.  Used by the Mathcad universal REFPROP function to handle
//                           units application to the return values (see RefProp_units.mcdx include file).
//
// ==================================================================================================================
// Finally, the specialized version of ALLPROPS0 and ABFLSH is also implemented here if there is a need for
// the fastest single-phase computational speed and the most robust two-phase flash calculation.  The other versions
// of ALLPROPS and ALLPROPS1 are not implemented here as these functions can be achieved through the REFPROP
// function, and in fact, REFPROP calls ALLPROPS to get its properties.
//
//    ABFLSH (rp_ABFLSH)       - Generic, two-phase flash calculation that handles all inputs of T, P, D, h, s, and q.
//                               No attempt to simplify this routine or provide unit functions.  Units returned are
//                               "DEFAULT" REFPROP units; mass or molar basis depending on iFlag parameter.
//    ALLPROPS0 (rp_ALLPROPS0) - Single phase calculation based on Temperature (T) and Density (D) state point only
//                               as a faster replacement for REFPROP. Designed with the "superuser" in mind,
//                                 - No string manipulation is performed (codes passed as enumerated values).
//                                 - No unit manipulation is performed.  All values returned in molar based
//                                   "DEFAULT" REFPROP units; leaving unit application up to the user in Mathcad.
//                                 - Always returns an array of results no matter how many properties are requested.
//                                 - Unlike the other add-in functions, the composition array (z) must be passed.
//
// Other REFPROP 10 functions are implemented in their own header files, including
//
//    ERRMSG     (rp_ERRMSG)
//    FLAGS      (rp_FLAGS)
//    GETENUM    (rp_GETENUM)
//    SETFLUIDS  (rp_SETFLUIDS)
//    SETMIXTURE (rp_SETMIXTURE)
//    SETPATH    (rp_SETPATH)
//
//===================================================================================================================
// Utility functions for REFPROP and ALLPROPS
//===================================================================================================================

// Look for valid REFPROP delimiters in a string
std::string GetDel(std::string str)
{
    using namespace std;
    //Figure out which delimiter the user used in OutString.
    char dels[5] = ",;| ";                 // Possible delimiters
    string del = "";                       // Initialize to no delimiters (empty string)
    for (int i = 0; i < 4; i++)            // For each possible delimiter
    {
        if (str.find(dels[i]) != str.npos) // If delimiter found in OutString (hOut)
        {
            del = del + dels[i];           //     append dels found to del string
        }                                  //     (Could be more than one.)
    }
    return del;                            // Return all delimiters found or empty string
}

// Wrap GetDel in a Mathcad function for direct use in Mathcad
LRESULT rp_GETDEL(LPMCSTRING  ret,          // Return string parameter
                  LPCMCSTRING hStr)         // Input string parameter
{
    using namespace std;
    string InString = hStr->str;            // Assign Input String for easy manipulation

    string delim = GetDel(InString);                              // Find any delimiters in the Input String
    char * c = MathcadAllocate((unsigned int)delim.size() + 1u);  // Create a c-string (pointer) with same size as s
                                                                  // This process avoids the const-cast type which would result from,
                                                                  //     instead, converting the string using s.c_str()
    std::copy(delim.begin(), delim.end(), c);                     // Copy s into c.
    c[delim.size()] = '\0';                                       // Tag on the null character
    ret->str = c;                                                 // Assign the string to the output Return parameter
    return 0;
}

FUNCTIONINFO    rp_GetDel =
{
    (char *)("rp_GetDel"),            // Name by which Mathcad will recognize the function
    (char *)("hStr"),                 // rp_GetDel will be called as rp_GetDel(hStr)
    (char *)("Returns the delimiter used in the input string, hStr; empty string if none found."),   // Description of rp_GetDel(hStr)
    (LPCFUNCTION)rp_GETDEL,         // Pointer to the executable code
    MC_STRING,                        // The return type is a Mathcad string
    1,                                // The function takes only 1 argument
    { MC_STRING }                     // - argument is a string
};

//===================================================================================================================
// Look for character-based REFPROP outputs in an output string
int IsChar(std::string Str)
{
    using namespace std;

    string Ostring;
    vector<string>::iterator it;                            // Set up string vector iterator

    string delim = GetDel(Str);                             // Look for valid delimiters in input string
    if (delim.length() > 1)                                 // Only one valid delimiter allowed
        return MIXED_DELS;                                  //     Return error otherwise
    else                                                    // If no error, parse the input string on found delimiter
    {
        if (delim == "") delim = " ";                                 // If no delimiter, set to space (" ")
        vector<string> StrVec = strsplit(upper(Str), (char)delim[0]);     // Parse Input string into vector of strings
        int nStr = (int)StrVec.size();                                    // Set nStr based on number of delimited strings

        for (int i = 0; i < nStr; i++)                                    // check every property for charOut strings
        {
            //OutString might end with (n) or (nn), Find position of "(" character.
            if (strendswith(StrVec[i], ")"))                            // if Outstring ends with "(n)" or "(nn)"
                Ostring = StrVec[i].substr(0, StrVec[i].find("("));     //    get just the beginning of the string
            else                                                        // otherwise
                Ostring = StrVec[i];                                    //    use the whole string

            it = find(cOutVec.begin(), cOutVec.end(), Ostring);         // Look for property in vector of char output strings
            if (it != cOutVec.end())                                    // If found
                return 1;                                               //    Return 1 (true)
        }
    }
    return 0;  // Return 0 (false) - No character outputs requested
}

// Wrap IsChar() in a Mathcad function for direct use in Mathcad
LRESULT rp_ISCHAR(LPCOMPLEXSCALAR  ret,     // Return true/false or error code
                  LPCMCSTRING hStr)         // Input string parameter
{
    using namespace std;
    string InString = hStr->str;            // Assign Input String for easy manipulation

    int flag = IsChar(InString);            // See if output string is a character request
    if (flag == MIXED_DELS)                 // If invalid delimiters
        return MAKELRESULT(MIXED_DELS, 1);  //    Throw error on first parameter
    ret->real = (double)flag;               // Assign the result (true/false) to the output Return parameter
    return 0;                               // No Error
}

FUNCTIONINFO    rp_IsChar =
{
    (char *)("rp_IsChar"),            // Name by which Mathcad will recognize the function
    (char *)("hStr"),                 // rp_IsChar will be called as rp_IsChar(hStr)
    (char *)("Checks delimited string for character output request and returns true or false"),   // Description of rp_IsChar(hStr)
    (LPCFUNCTION)rp_ISCHAR,           // Pointer to the executable code
    COMPLEX_SCALAR,                   // The return type is a Mathcad string
    1,                                // The function takes only 1 argument
    { MC_STRING }                     // - argument is a string
};

//===================================================================================================================
// Look for multi-valued REFPROP outputs in a single, non-delimited output string. Returns true (1) or false (0)
int IsMulti(std::string Str)
{
    using namespace std;

    vector<string>::iterator it;                            // Set up string vector iterator

    it = find(mOutVec.begin(), mOutVec.end(), upper(Str));        // Look for property Str in vector of char output strings
    if (it != mOutVec.end())                                      // If found
        return 1;                                                 //    Return 1 (true)

    return 0;  // Return 0 (false) - No character outputs requested
}

// Wrap IsMulti() in a Mathcad function for direct use in Mathcad
LRESULT rp_ISMULTI(LPCOMPLEXSCALAR  ret,     // Return true/false or error code
                   LPCMCSTRING      hStr)    // Input string parameter
{
    using namespace std;
    string InString = hStr->str;             // Assign Input String for easy manipulation

    ret->real = (double)IsMulti(InString);   // Assign the result (true/false) to the output Return parameter
    return 0;                                // No Error
}

FUNCTIONINFO    rp_IsMulti =
{
    (char *)("rp_IsMulti"),           // Name by which Mathcad will recognize the function
    (char *)("hStr"),                 // rp_IsMulti will be called as rp_IsMulti(hStr)
    (char *)("Checks delimited string for character output request and returns true or false"),   // Description of rp_IsMulti(hStr)
    (LPCFUNCTION)rp_ISMULTI,          // Pointer to the executable code
    COMPLEX_SCALAR,                   // The return type is a Mathcad string
    1,                                // The function takes only 1 argument
    { MC_STRING }                     // - argument is a string
};



//===================================================================================================================
// REFPROP1
//===================================================================================================================

LRESULT rp_Refprop1(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING      hInput,
    LPCMCSTRING     hOutput,
    LPCCOMPLEXSCALAR      a,
    LPCCOMPLEXSCALAR      b)
{
    using namespace std;

    ierr = 0;                                   // Error Code
    char herr[errormessagelength] = "OK\0";     // Error String

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);                       // Throw error: Routine not Implemented (< REFPROP 10)
        
    if( a->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL, 3);                          // Parameter cannot be complex

    if (b->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 4);                          // Parameter cannot be complex

    double aval = a->real;
    double bval = b->real;
    double cval, qval;                                                // Holds result from REFPROP1
    string InString = hInput->str;                               // Input String for easy manipulation
    string OutString = hOutput->str;                             // Output String for easy manipulation

    if (GetDel(OutString) != "")                                      // At least one delimiter found
        return MAKELRESULT(ONLY_SINGLE, 2);                           //    Return error - Only single hOut strings allowed
    else if (OutString == "DLL#")                                     // If DLL# requested
    {                                                                 //    iUcode not available from REFPROP1dll
        return MAKELRESULT(DLL_REQ, 2);                               //    Return error - Must get DLL# from REFPROP or REFPROPc call
    }
    else                                                              // Otherwise, only one hOut requested.
    {
        if ( IsMulti(OutString) )                                     //     if multi-output string found
            return MAKELRESULT(ONLY_SINGLE, 2);                       //        throw error on OutString

        if (IsChar(OutString))                                        //     if char-output string found
            return MAKELRESULT(NO_CHARS, 2);                          //        throw error on OutString
    }

    char hIn[refpropcharlength] = "";
    char hOut[refpropcharlength] = "";
    copy(InString.begin(), InString.end(), hIn);           // Copy string fluid_string into c_str hFld. 
    copy(OutString.begin(), OutString.end(), hOut);        // Copy string fluid_string into c_str hFld. 
    hIn[InString.length()] = '\0';
    hOut[OutString.length()] = '\0';

    REFPROP1dll(                      // Call REFPROP1dll (no fluid string required and only one value returned)
        hIn,                          // hIn c-string
        hOut,                         // hOut c-string
        &iUnits,                      // Units code (set with GETENUMdll)
        &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions)
        &aval,                        // 1st state point value
        &bval,                        // 2nd state point value
        &x[0],                        // composition array [20]
        &cval,                        // return value
        &qval,                        // vapor quality (in 2-phase region)
        &ierr,                        // error message
        herr,                         // error string
        refpropcharlength,            // max length of hIn string
        refpropcharlength,            // max length of hOut string
        errormessagelength);          // max length of error message string

    // Check for errors:
    if (ierr > 0) {                                      // IF Error returned
        return MAKELRESULT(UNKNOWN10, 2);                //    Return REFPROP1 error
    }
    else if (ierr < 0)                                   // IF Warning returned
    {
        if (!NO_WARNINGS)                                // NO_WARNINGS flag set in rp_FLAGS
        {
            size_t nwstr = strlen(herr);
            while (' ' == herr[--nwstr]) herr[nwstr] = '\0';                           // Strip trailing spaces
            MessageBox(hwndDlg, herr, "NIST RefProp Add-In", MB_ICONWARNING | MB_OK);  // Pop-up warning message box.
        }
    }

    ret->real = cval;                 // get the returned value

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_REFPROP1 =
{
    (char *)("rp_REFPROP1"),            // Name by which Mathcad will recognize the function
    (char *)("hIn, hOut, a, b"),        // rp_REFPROP1 will be called as rp_REFPROP1(hIn, hOut, a, b)
    (char *)("Calls REFPROP1dll with hIn and hOut strings as well as input values a and b; all others defaulted."),
                                        // Description of rp_REFPROP1(hIn, hOut, a, b)
    (LPCFUNCTION)rp_Refprop1,           // Pointer to the executable code
    COMPLEX_SCALAR,                     // The return type is a complex scalar
    4,                                  // The function takes 2 arguments
    { MC_STRING,                        //   - 1st argument is a string
      MC_STRING,                        //   - 2nd argument is a string
      COMPLEX_SCALAR,                   //   - 3rd argument is a scalar
      COMPLEX_SCALAR }                  //   - 4th argument is a scalar
};

//===================================================================================================================
// REFPROP
//===================================================================================================================

LRESULT rp_Refprop(
    LPCOMPLEXARRAY      ret,
    LPCMCSTRING      hFluid,
    LPCMCSTRING      hInput,
    LPCMCSTRING     hOutput,
    LPCCOMPLEXSCALAR      a,
    LPCCOMPLEXSCALAR      b)

{
    using namespace std;

    char htype[] = "EOS";

    ierr = 0;                                                          // Error Code [int,out]
    int iUCode;                                                        // Unit Code [int,out]
    char herr[errormessagelength] = "OK\0";                            // Error String
    char hUnits[refpropcharlength];                                    // hUnits string for REFPROP Call
    double x2[20] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };       // initialize x[] for pure fluid
    double y2[20] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };       // initialize y[] for pure fluid
    double x3[20] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };       // initialize x3[] for pure fluid

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);                        // Throw error: Routine not Implemented (< REFPROP 10)

    if (a->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 3);                           // Parameter cannot be complex

    if (b->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 4);                           // Parameter cannot be complex

    double aval = a->real;
    double bval = b->real;
    double Output[200] = { 0.0 };                                      // Initialize Output array to zero, Holds results from REFPROP
    double qval;                                                       // q value returned from refprop separately
    int nOut = 1;                                                      // number of output props requested
    unsigned int rows = 1, cols = 1;
    string FluidString = upper(hFluid->str);                           // Get fluid string parameter
    string InString = upper(hInput->str);                              // Input String for easy manipulation
    string OutString = upper(hOutput->str);                            // Output String for easy manipulation

    if (FluidString.empty())
    {
        FluidString = " ";                  // If empty fluid string passed, add a space for REFPROP
    }

    string dels = GetDel(OutString);
    if (dels.length() > 1)                                            // Don't allow mixed delimiters, 
        return MAKELRESULT(MIXED_DELS, 3);                            //     throw error up front
    if (dels == "") dels = " ";                                       // set space " " as default

    vector<string> props = strsplit(OutString, (char)dels[0]);        // Split the OutString (hOut) in to multiple strings
    int nProp = (int)props.size();                                    //     Set nProp based on number of outputs requested
    nOut = nProp;                                                     //     Initialize nOut to nProp (single-valued properties)

    if (IsChar(OutString))                                            // If any requested string outputs
        return MAKELRESULT(NO_CHARS, 3);                              //     throw error on OutString

    if ((OutString.find("DLL#") != OutString.npos) && (nProp > 1))    // Look for special case of "DLL#
            return MAKELRESULT(DLL_REQ, 3);                           // If found, can be only 1 output

    if (find(props.begin(),props.end(),"X") != props.end())           // Look for special case of "X"
        return MAKELRESULT(BAD_HOUT_X, 3);                            // If found, can only use "XMOLE" or "XMASS"

    if ((OutString.find("UNITNUMB") != OutString.npos ||
         OutString.find("UNITS") != OutString.npos) && (nProp > 1))   // Look for special case of "UNITNUMB" or "UNITS"
        return MAKELRESULT(DLL_REQ, 3);                               // If found, can only be 1 output request
                                                                      // NOTE: "UNITSTRING" is checked by IsChar

    // Convert C++ strings to char arrays so they can be passed to REFPROPdll
    char hFld[componentstringlength] = "";
    char hIn[refpropcharlength] = "";
    char hOut[refpropcharlength] = "";
    copy(FluidString.begin(), FluidString.end(), hFld);    // Copy string fluid_string into c_str hFld.
    copy(InString.begin(), InString.end(), hIn);           // Copy string fluid_string into c_str hFld. 
    copy(OutString.begin(), OutString.end(), hOut);        // Copy string fluid_string into c_str hFld. 
    hFld[FluidString.length()] = '\0';
    hIn[InString.length()] = '\0';
    hOut[OutString.length()] = '\0';

    REFPROPdll(hFld,                  // Passed in Fluid String
        hIn,                          // hIn string
        hOut,                         // hOut string
        &iUnits,                      // Units code (set with GETENUMdll)
        &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions)
        &iFlag,                       // iFlag = 1 (ALWAYS call SATSPLN if new mixture)
        &aval,                        // 1st state point value
        &bval,                        // 2nd state point value
        &x[0],                        // composition array [20]
        &Output[0],                   // Output array [200]
        hUnits,                       // return unit string
        &iUCode,                      // return unit code
        &x2[0],                       // liquid mole fractions
        &y2[0],                        // liquid mole fractions
        &x3[0],                       // liquid mole fractions
        &qval,                        // vapor quality (in 2-phase region)
        &ierr,                        // error message
        herr,                         // error string
        componentstringlength,
        refpropcharlength,
        refpropcharlength,
        refpropcharlength,
        errormessagelength);

    // Check for errors from REFPROPdll call
    if (ierr > 0) {
        if (ierr == 601)
            return MAKELRESULT(BAD_HIN, 2);              // Invalid input, hIn
        else if (ierr == 811)
            return MAKELRESULT(BAD_HOUT, 3);             // Invalid parameter requested in hOut
        else if (ierr == 101)
            return MAKELRESULT(FLUID_NOT_FOUND, 1);      // Fluid/mixture in hFld not found
        else
            return MAKELRESULT(UNKNOWN10, 1);            // Error setting Flag
    }
    else
    {
        if (ierr < 0)                                    // IF Warning returned
        {
            if (!NO_WARNINGS)                            // Check NO_WARNINGS flag set in rp_FLAGS
            {
                size_t nwstr = strlen(herr);
                while (' ' == herr[--nwstr]) herr[nwstr] = '\0';                           // Strip trailing spaces
                MessageBox(hwndDlg, herr, "NIST RefProp Add-In", MB_ICONWARNING | MB_OK);  // Pop-up warning message box.
            }
        }

        if ((FluidString != " ") && (OutString != "UNITNUMB") && (LastFluid != FluidString))
        {
            LastFluid = FluidString;
            LIMITSdll(htype, &x[0], &Tmin, &Tmax, &Dmax, &Pmax, lengthofreference);   // Get Tmin, Tmax, Dmzx, Pmax for this fluid/mixture
            WMOLdll(&x[0], &wmm);                                                     // Get molar mass [kg/kmol or g/mol]

            //set ncomp
            char hOut2[refpropcharlength] = "NCOMP";         // Set hOut2 to NCOMP
            double cval;
            int ierr2;

            REFPROP1dll(                      // Call REFPROP1dll (no fluid string required and only one value returned)
                hIn,                          // hIn c-string
                hOut2,                        // hOut c-string
                &iUnits,                      // Units code (set with GETENUMdll)
                &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions)
                &aval,                        // 1st state point value
                &bval,                        // 2nd state point value
                &x[0],                        // composition array [20]
                &cval,                        // return value
                &qval,                        // vapor quality (in 2-phase region)
                &ierr2,                        // error message
                herr,                         // error string
                refpropcharlength,            // max length of hIn string
                refpropcharlength,            // max length of hOut string
                errormessagelength);          // max length of error message string

            ncomp = (int)cval;     // get number of components; double -> int conversion hopefully won't cause problems.
        }

        if ((OutString == "DLL#") || (OutString == "UNITNUMB") || (OutString == "UNITS"))  // Special case: hOut = "DLL#" or "UNITNUMB" or "UNITS"
        {
            nOut = 1;                                          //     only one return value
            Output[0] = (double)iUCode;                        //     get numeric DLL# or UNIT Code from iUCode
        }
        else
        {
            // If more that one component, increase length of Output array for each multi-valued property requested.
            // Note: REFPROPdll will have returned warning, ierr = -852 for mismatch between hOut and Output length
            if (ncomp > 1)
            {
                // check each and every output prop string to see if it is one of the multiOut strings
                for (int i = 0; i < nProp; i++)
                {
                    if ( IsMulti(props[i]) ) nOut += (ncomp - 1);             //    add additional Output slots for extra components
                }
            }
        }

        rows = (unsigned int)nOut;                                            // Set number of rows to return to Mathcad (Cols = 1)

        //allocate memory for the Output values
        if (!MathcadArrayAllocate(
            ret,                        // this is our return COMPLEX array
            rows,                       // initialize number of rows
            cols,                       // Should only be one (1) column wide
            TRUE,                       // Real values allocated
            FALSE))                     // No complex portions allocated
        {
            // if allocation is not successful, return with the appropriate error code
            return MAKELRESULT(INSUFFICIENT_MEMORY, 1);
        }

        // Assign return array elements
        for (int i = 0; i < nOut; i++)
        {
            ret->hReal[0][i] = Output[i];             // Load elements of Output array
            if (isUserInterrupted())                  // Release Mathcad memory if the calculations are interrupted
            {
                MathcadArrayFree(ret);                //      if use has interrupted -- free the allocated memory
                return MAKELRESULT(INTERRUPTED, 1);   //      and return with an appropriate error code
            }
        }
    }
    
    return 0;               // return 0 to indicate there was no error
}

FUNCTIONINFO    rp_REFPROP =
{
    (char *)("rp_REFPROP"),                   // Name by which Mathcad will recognize the function
    (char *)("hFld, hIn, hOut, a, b"),        // rp_REFPROP1 will be called as rp_REFPROP1(hFld, hIn, hOut, a, b)
    (char *)("Calls REFPROP1dll with hIn and hOut strings as well as input values a and b; all others defaulted."),
    // Description of rp_REFPROP(hFld, hIn, hOut, a, b)
    (LPCFUNCTION)rp_Refprop,                  // Pointer to the executable code
    COMPLEX_ARRAY,                            // The return type is a complex scalar
    5,                                        // The function takes 2 arguments
    { MC_STRING,                              //   - 1st argument is a string
      MC_STRING,                              //   - 2nd argument is a string
      MC_STRING,                              //   - 3rd argument is a string
      COMPLEX_SCALAR,                         //   - 4th argument is a scalar
      COMPLEX_SCALAR }                        //   - 5th argument is a scalar
};

//===================================================================================================================
// REFPROPc
//===================================================================================================================

LRESULT rp_Refpropc(
    LPMCSTRING         hstr,    // Pointer to the results string
    LPCMCSTRING      hFluid,    // Takes a Fluid string, but can be blank or a space to use loaded fluid
    LPCMCSTRING      hInput,    // Needed for just a few character calls, otherwise empty string
    LPCMCSTRING     hOutput,    // Requested string output
    LPCCOMPLEXSCALAR      a,    // Need numeric input a for just a few character calls, otherwise zero
    LPCCOMPLEXSCALAR      b)    // Need numeric input a for just a few character calls, otherwise zero
{
    using namespace std;

    ierr = 0;                                   // Error Code [int,out]
    int iUCode;                                 // Unit Code [int,out]
    char herr[errormessagelength] = "OK\0";     // Error String
    char hUnits[refpropcharlength];             // hUnits string for REFPROP Call
    double x2[20] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };  // initialize x[] for pure fluid
    double y2[20] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };  // initialize y[] for pure fluid
    double x3[20] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };  // initialize x3[] for pure fluid

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);                       // Throw error: Routine not Implemented (< REFPROP 10)

    if (a->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 3);                          // Parameter cannot be complex

    if (b->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 4);                          // Parameter cannot be complex

    double aval = a->real;
    double bval = b->real;
    double Output[200] = { 0.0 };                                      // Initialize Output array to zero, Holds results from REFPROP
    double qval;                                                       // q value returned from refprop separately
    string FluidString = upper(hFluid->str);                           // Get fluid string parameter
    string InString = hInput->str;                                     // Input String for easy manipulation
    string OutString = hOutput->str;                                   // Output String for easy manipulation

    if (FluidString.empty())
    {
        FluidString = " ";                  // If empty fluid string passed, add a space for REFPROP
    }

    if (GetDel(OutString) != "")                                    // At least one delimiter found
    {
        MessageBox(hwndDlg, "Delimiter Found", "NIST RefProp Add-In", MB_ICONWARNING | MB_OK);  // Debug.
        return MAKELRESULT(ONLY_CHARS, 2);                          //      Multiple Values Requested
    }
    else if(upper(OutString) != "DLL#")                             // Skip char check if "DLL#" (Returns both types)
    {
        if (!IsChar(upper(OutString)))                              // If OutString does not contain a char string request
            return MAKELRESULT(ONLY_CHARS, 3);                      //    throw error on OutString
    }

    // Convert strings to character arrays so they can be passed to the REFPROP DLL
    char hFld[componentstringlength] = "";
    char hIn[refpropcharlength] = "";
    char hOut[refpropcharlength] = "";
    copy(FluidString.begin(), FluidString.end(), hFld);          // Copy string fluid_string into c_str hFld.
    copy(InString.begin(), InString.end(), hIn);            // Copy string fluid_string into c_str hFld. 
    copy(OutString.begin(), OutString.end(), hOut);         // Copy string fluid_string into c_str hFld. 
    hFld[FluidString.length()] = '\0';
    hIn[InString.length()] = '\0';
    hOut[OutString.length()] = '\0';

    REFPROPdll(hFld,                  // Passed in Fluid String
        hIn,                          // hIn string
        hOut,                         // hOut string
        &iUnits,                      // Units code (set with GETENUMdll)
        &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions)
        &iFlag,                       // iFlag = 1 (ALWAYS call SATSPLN if new mixture)
        &aval,                        // 1st state point value
        &bval,                        // 2nd state point value
        &x[0],                        // composition array [20]
        &Output[0],                   // Output array [200]
        hUnits,                       // return unit string
        &iUCode,                      // return unit code
        &x2[0],                       // liquid mole fractions
        &y2[0],                       // vapor mole fractions
        &x3[0],                       // reserved for 2nd liquid phase for LLE or VLLE
        &qval,                        // vapor quality (in 2-phase region)
        &ierr,                        // error message
        herr,                         // error string
        componentstringlength,
        refpropcharlength,
        refpropcharlength,
        refpropcharlength,
        errormessagelength);

    // Check for errors from REFPROPdll call
    if (ierr > 0)                                        // trap errors; pass through negative warnings
    {
        if (ierr == 601)
            return MAKELRESULT(BAD_HIN, 2);              // Invalid input, hIn
        else if (ierr == 811)
            return MAKELRESULT(BAD_HOUT, 3);             // Invalid parameter requested in hOut
        else if (ierr == 101)
            return MAKELRESULT(FLUID_NOT_FOUND, 1);      // Fluid/mixture in hFld not found
        else
            return MAKELRESULT(UNKNOWN10, 1);            // Error setting Flag
    }
    else  // No errors, set fluid constants and ncomp if new fluid and process output string
    {
        if (ierr < 0)                                    // IF Warning returned
        {
            if (!NO_WARNINGS)                            // Check NO_WARNINGS flag set in rp_FLAGS
            {
                size_t nwstr = strlen(herr);
                while (' ' == herr[--nwstr]) herr[nwstr] = '\0';                           // Strip trailing spaces
                MessageBox(hwndDlg, herr, "NIST RefProp Add-In", MB_ICONWARNING | MB_OK);  // Pop-up warning message box.
            }
        }

        if (FluidString != " ")
        {
            char htype[] = "EOS";

            LastFluid = FluidString;
            LIMITSdll(htype, &x[0], &Tmin, &Tmax, &Dmax, &Pmax, lengthofreference);   // Get Tmin, Tmax, Dmzx, Pmax for this fluid/mixture
            WMOLdll(&x[0], &wmm);                                                     // Get molar mass [kg/kmol or g/mol]

            // Set ncomp and increase the number of outputs if mixture
            // NOTE: Can't get ncomp consistently from the length of the x[] array above?  Or x2[].
            //       We need to call REFPROP again, though it's a pretty quick call.
            char hOut2[refpropcharlength] = "NCOMP";         // get the number of components.
            double cval;
            int ierr2;

            REFPROP1dll(                      // Call REFPROP1dll (no fluid string required and only one value returned)
                hIn,                          // hIn c-string
                hOut2,                        // hOut c-string
                &iUnits,                      // Units code (set with GETENUMdll)
                &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions)
                &aval,                        // 1st state point value
                &bval,                        // 2nd state point value
                &x[0],                        // composition array [20]
                &cval,                        // return value
                &qval,                        // vapor quality (in 2-phase region)
                &ierr2,                       // error message
                herr,                         // error string
                refpropcharlength,            // max length of hIn string
                refpropcharlength,            // max length of hOut string
                errormessagelength);          // max length of error message string

            ncomp = (int)cval;     // get number of components; double -> int conversion
        }

        size_t nstr = strlen(hUnits);
        while (' ' == hUnits[--nstr]) hUnits[nstr] = '\0';      // Strip trailing spaces from return string (hUnits)
        nstr++;

        char * c = MathcadAllocate((unsigned int)nstr + 1u);    // Create a c-string (pointer) with same size as hUnits
        if (c == NULL)                                          // if allocation fails
            return MAKELRESULT(INSUFFICIENT_MEMORY, 3);         //    Throw insufficient memory error
        else                                                    // otherwise...
        {
            std::strncpy(c, hUnits, nstr);                      //    Copy hUnits into c which has been mem-allocated by Mathcad.
            c[nstr] = '\0';                                     //    Tag on the null character
            hstr->str = c;                                      //    Assign the string to the output parameter
        }
    }

    return 0;               // return 0 to indicate there was no error
}

FUNCTIONINFO    rp_REFPROPc =
{
    (char *)("rp_REFPROPc"),                  // Name by which Mathcad will recognize the function
    (char *)("hFLD, hIn, hOut, a, b"),        // rp_REFPROPc will be called as rp_REFPROPc(hFld, hIn, hOut, a, b)
    (char *)("Calls REFPROPdll with hFld, hIn, and hOut strings as well as input values a and b; Returns only character string values."),
    // Description of rp_REFPROP1(hFld, hIn, hOut, a, b)
    (LPCFUNCTION)rp_Refpropc,                 // Pointer to the executable code
    MC_STRING,                                // The return type is a Mathcad string
    5,                                        // The function takes 2 arguments
    { MC_STRING,                              //   - 1st argument is a string
      MC_STRING,                              //   - 2nd argument is a string
      MC_STRING,                              //   - 3rd argument is a string
      COMPLEX_SCALAR,                         //   - 4th argument is a scalar
      COMPLEX_SCALAR }                        //   - 5th argument is a scalar
};


//===================================================================================================================
// ALLPROP (Special case of extracting only the units array)
//===================================================================================================================

// This is a specialized case of strsplit.
// if the delimited string element returns multiple values, the element is added ncomp times.
// This routine is only used by rp_UnitCodes (rp_UNITS) to match returned units array to REFPROP output array based on hOut strings
std::vector<std::string> strSplitOut(const std::string &s, char del)
{
    using namespace std;

    vector<string> v;
    string::const_iterator i1 = s.begin(), i2;
    vector<string>::iterator it;                                          // Create iterator for search through mOutVec
    int times = 1;
    while (true) {
        i2 = find(i1, s.end(), del);
        times = 1;
        if (ncomp > 1)
        {
            if ( IsMulti(string(i1,i2)) ) times = ncomp;     // Set to insert substring ncomp times
        }
        v.insert(v.end(), times, string(i1, i2));  // Use v.insert (allows multiple inserts) instead of v.pushback
        if (i2 == s.end())
            break;
        i1 = i2 + 1;
    }
    return v;
}

// This routine takes the requested properties from hOut and retrieves
// the character code (in the set unit system) for each one from ALLPROPS.
LRESULT rp_UnitCodes(
    LPCOMPLEXARRAY      ret,
    LPCMCSTRING     hOutput)
{
    using namespace std;

    ierr = 0;                                   // Error Code [int,out]
    int iFl = 2, iU = -iUnits;                  // Unit Code [int,out]
    char herr[errormessagelength] = "OK\0";     // Error String
    char hUnits[componentstringlength] = "\0";  // hUnits string for REFPROP Call
    double T = 300.0, D = 0.0;                  // Dummy state-point values (will not be evaluated)
    unsigned int rows = 1, cols = 1;            // Rows and columns to return

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);            // Throw error: Routine not Implemented (< REFPROP 10)

    double Output[200] = { 0.0 };                          // Initialize Output array to zero, Holds results from ALLPROPS
    int iUCodeArray[200] = { 0 };                          // Initialize IUCodeArray to zeros, Holds units from ALLPROPS
    string OutString = hOutput->str;                       // Output String for easy manipulation


    char hOut[componentstringlength] = "";                 // Initialize hOut C-string.
    copy(OutString.begin(), OutString.end(), hOut);   // Copy string fluid_string into c_str hFld. 
    hOut[OutString.length()] = '\0';                       // Append null character to terminate string.

    ALLPROPSdll(hOut,                 // hOut delimited string of requested properties
        &iU,                          // Units code (sent as negative value so input scaling is bypassed)
        &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions; always 0 for Mathcad wrapper)
        &iFl,                         // iFlag = 2 (Return units only, no properties calculated)
        &T,                           // 1st state point value, T = 300; Dummy value as nothing is calculated
        &D,                           // 2nd state point value, D = 0  ; Dummy value as nothing is calculated
        &x[0],                        // composition array [20]
        &Output[0],                   // Output array [200] - Nothing returned here.
        hUnits,                       // return unit string
        &iUCodeArray[0],              // return unit codes for each delimited element string in hOut
        &ierr,                        // error message
        herr,                         // error string
        componentstringlength,
        componentstringlength,
        errormessagelength);

    // Check for errors from REFPROPdll call
    if (ierr > 0) {
        if (ierr == 811)
            return MAKELRESULT(BAD_HOUT, 3);             // Invalid parameter requested in hOut
        else
            return MAKELRESULT(UNKNOWN10, 1);            // Some other REFPROP 10 error
    }
    else
    {
        if (ierr < 0)                                    // IF Warning returned
        {
            if (!NO_WARNINGS)                            // Check NO_WARNINGS flag set in rp_FLAGS
            {
                size_t nwstr = strlen(herr);
                while (' ' == herr[--nwstr]) herr[nwstr] = '\0';                           // Strip trailing spaces
                MessageBox(hwndDlg, herr, "NIST RefProp Add-In", MB_ICONWARNING | MB_OK);  // Pop-up warning message box.
            }
        }
        //Figure out which delimiter the user used in OutString.
        int  nProp = 1;                                        // Initialize to just one property requested; no delimiters
        string delim = GetDel(OutString);
        if (delim == "") delim = " ";

        vector<string> props = strSplitOut(OutString, (char)delim[0]);    // Split the Outstring (hOut) in to multiple strings, modified for multiOut values
        nProp = (int)props.size();                             // Count the number of properties requested

        rows = (unsigned int)nProp;                            // Set number of rows to return to Mathcad (Cols = 1)

        if (!MathcadArrayAllocate(
            ret,                        // this is our return COMPLEX array
            rows,                       // initialize number of rows
            cols,                       // Should only be one (1) column wide
            TRUE,                       // Real values allocated
            FALSE))                     // No complex portions allocated
        {
            // if allocation is not successful, return with the appropriate error code
            return MAKELRESULT(INSUFFICIENT_MEMORY, 1);
        }

        // Assign return array elements

        for (int i = 0; i < nProp; i++)
        {
            ret->hReal[0][i] = (double)iUCodeArray[i];    // Load elements of Unit code array
            if (isUserInterrupted())                  // Release Mathcad memory if the calculations were interrupted
            {
                MathcadArrayFree(ret);                         // if use has interrupted -- free the allocated memory
                return MAKELRESULT(INTERRUPTED, 1);            // and return with an appropriate error code
            }
        }

    }
    return 0;               // return 0 to indicate there was no error
}

FUNCTIONINFO    rp_UNITS =
{
    (char *)("rp_UNITS"),                     // Name by which Mathcad will recognize the function
    (char *)("hOut"),                         // rp_UNITS will be called as rp_UNITS(hOut)
    (char *)("Calls ALLPROPSdll with hOut string to request units for each property listed in hOut"),
    // Description of rp_UNITS(hOut)
    (LPCFUNCTION)rp_UnitCodes,                // Pointer to the executable code
    COMPLEX_ARRAY,                            // The return type is a complex scalar
    1,                                        // The function takes 1 arguments
    { MC_STRING }                             //   - 1st argument is a string
};


// This routine takes the requested properties from hOut and retrieves
// the character code (in the set unit system) for each one from ALLPROPS.
// Implementing this as a beta feature for performance testing.
//
LRESULT rp_AllProps0(
    LPCOMPLEXARRAY       ret,
    LPCCOMPLEXARRAY      iOUT,                 // parameter 1
    LPCCOMPLEXSCALAR     TEMP,                 // parameter 2
    LPCCOMPLEXSCALAR     DEN,                  // parameter 3
    LPCCOMPLEXARRAY      Z,                    // parameter 4
    LPCCOMPLEXARRAY      OUTPUT)               // parameter 5
{
    using namespace std;

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1); // Throw error: Routine not Implemented (< REFPROP 10)

    if (Z->rows != (unsigned int)ncomp)         // mole fraction array must match number of components
        return MAKELRESULT(COMPONENT_COUNT, 5);

    if ((iOUT->cols > 1) && (iOUT->rows > 1))   // iOut can be column or row vector, but not both.
        return MAKELRESULT(VECTOR_REQ, 2);

    int iIn = (int)iOUT->rows;                  // iIn [int,in]         Number of properties to calculate; Total size of iOut array.
    int iOut[200] = { 0 };                      // iOut(200) [int,in]   All elements initialized to zero.

    int iFl = 3;                                // Look in non-T/D dependent properties
    char hEnum[refpropcharlength] = "X";        // Mole/Mass Fraction variable
    char herr[errormessagelength] = "OK\0";     // Error String
    int iX;                                     // Store enumeration of "X"

    GETENUMdll(&iFl, hEnum, &iX, &ierr, herr, refpropcharlength, errormessagelength);

    for (int i = 0; i < iIn; i++)               // Index from 0 to (iIn - 1)
    {
        if (iOUT->cols > 1)
            iOut[i] = (int)iOUT->hReal[i][0];   //     Set each iOut element from iOUT row array
        else
            iOut[i] = (int)iOUT->hReal[0][i];   //     Set each iOut element from iOUT column array

        if (iOut[i] == iX)                      //     IF enumerated value for just "X" found
            return MAKELRESULT(BAD_HOUT_X, 2);  //         throw error: Must use XMASS or XMOLE
    }
    iFl = 0;                                    // iFlag [int,in]   Not used, would be (int)iFLAG->real;
    double T = (double)TEMP->real;              // T [double,in]    Temperature
    double D = (double)DEN->real;               // D [double,in]    Density
    // Loop over components and return the mole fractions in x
    for (int i = 0; i < ncomp; i++)             // Index from 0 to (ncomp - 1)
    {
        x[i] = Z->hReal[0][i];                  //    Set each fraction from the Z array
    }
    ierr = 0;                                   // Error Code [int,out]
    unsigned int cols = 1;                      // Columns to return

    double Output[200] = { 0.0 };               // Initialize Output array to zero, Holds results from ALLPROPS
    for (int i = 0; i < iIn; i++)               // Index from 0 to iIn
    {
        Output[i] = OUTPUT->hReal[0][i];        //    Initialize to the OUTPUT array that was passed in.
    }

    ALLPROPS0dll(&iIn,                // iIn number of requested properties
        &iOut[0],                     // array of property codes
        &iFl,                         // iFlag (not used)
        &T,                           // 1st state point value, T
        &D,                           // 2nd state point value, D
        &x[0],                        // composition array [20]
        &Output[0],                   // Output array [200]
        &ierr,                        // error code
        herr,                         // error string
        errormessagelength);

    // Check for errors from REFPROPdll call
    if (ierr > 0) {
        if (ierr == 811)
            return MAKELRESULT(BAD_HOUT, 3);             // Invalid parameter requested in hOut
        else
            return MAKELRESULT(UNKNOWN10, 1);            // Some other REFPROP 10 error
    }
    else
    {
        if (ierr < 0)                                    // IF Warning returned
        {
            if (!NO_WARNINGS)                            // Check NO_WARNINGS flag set in rp_FLAGS
            {
                size_t nwstr = strlen(herr);
                while (' ' == herr[--nwstr]) herr[nwstr] = '\0';                           // Strip trailing spaces
                MessageBox(hwndDlg, herr, "NIST RefProp Add-In", MB_ICONWARNING | MB_OK);  // Pop-up warning message box.
            }
        }

        if (!MathcadArrayAllocate(
            ret,                        // this is our return COMPLEX array
            iIn,                        // initialize number of rows
            cols,                       // Should only be one (1) column wide
            TRUE,                       // Real values allocated
            FALSE))                     // No complex portions allocated
        {
            // if allocation is not successful, return with the appropriate error code
            return MAKELRESULT(INSUFFICIENT_MEMORY, 1);
        }

        // Assign return array elements

        for (int i = 0; i < iIn; i++)
        {
            ret->hReal[0][i] = (double)Output[i];    // Load elements of Unit code array
            if (isUserInterrupted())                  // Release Mathcad memory if the calculations were interrupted
            {
                MathcadArrayFree(ret);                         // if use has interrupted -- free the allocated memory
                return MAKELRESULT(INTERRUPTED, 1);            // and return with an appropriate error code
            }
        }

    }
    return 0;               // return 0 to indicate there was no error
}

FUNCTIONINFO    rp_ALLPROPS0 =
{
    (char *)("rp_ALLPROPS0"),                 // Name by which Mathcad will recognize the function
    (char *)("iIn,iOut,iFlag,T,D,z"),         // rp_ALLPROPS0 will be called as rp_ALLPROPS0(iIn,iOut,iFlag,T,D,z)
    (char *)("[Beta Feature] Calls ALLPROPS0dll with (T, D, z) for each property listed in iOut array."),
    // Description of rp_ALLPROPS0(hOut)
    (LPCFUNCTION)rp_AllProps0,                // Pointer to the executable code
    COMPLEX_ARRAY,                            // The return type is a complex scalar
    5,                                        // The function takes 1 arguments
    { 
        COMPLEX_ARRAY,                        //   - 1st argument is an array
        COMPLEX_SCALAR,                       //   - 2nd argument is a scalar
        COMPLEX_SCALAR,                       //   - 1st argument is a scalar
        COMPLEX_ARRAY,                        //   - 1st argument is an array
        COMPLEX_ARRAY                         //   - 1st argument is an array
    }
};
