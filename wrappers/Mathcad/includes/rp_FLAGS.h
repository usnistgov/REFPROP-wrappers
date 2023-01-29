LRESULT rp_Flags(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING        flag,
    LPCCOMPLEXSCALAR    val   )
{
    ierr = 0;
    int jFlag = 0;                              // input flag value to set
    int kFlag = 0;                              // return flag value
    char hFlag[refpropcharlength];              // Flag character array
    char herr[errormessagelength] = "OK\0";

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);                       // Throw error: Routine not Implemented (< REFPROP 10)
        
    if( val->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL, 2);                          // Parameter cannot be complex
    else
        jFlag = static_cast<int>(val->real);                          // Extract real part to (int)jFlag

    std::string flag_string = flag->str;                              // Extract first parameter to a string variable

    //Check for special Mathcad DLL flag to suppress warning Pop-ups from REFPROP calls 
    if (upper(flag_string) == "NO WARNINGS")
    {
        if (jFlag == 0)
            NO_WARNINGS = 0;                                          // Do not suppress warning pop-ups
        else if (jFlag == 1)
            NO_WARNINGS = 1;                                          // Suppress warning pop-ups
        ret->real = (double)NO_WARNINGS;                              // pass back the NO_WARNINGS flag state;
        return 0;                                                     // return from this routing (do not call FLAGSdll)
    }

    std::copy(flag_string.begin(), flag_string.end(), hFlag);         // Copy string flag_string into c_str hFlag.
    hFlag[flag_string.size()] = '\0';                                 // Append with a null character

    // These flags cannot be set in the Mathcad wrapper as it will cause unexpected results
    if (strstartswith(upper(flag_string), "RETURN") || strstartswith(upper(flag_string), "WRITE") ||
        strstartswith(upper(flag_string), "DIR") || strstartswith(upper(flag_string), "DEBUG"))
    {
        jFlag = -999;                                                 // Can't set this value, so just return what's already set
        msg = "Cannot change '" + flag_string + "' flag in Mathcad \ninterface, as this would cause unexpected behavior.\n\nReturning the current value.";
        MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", MB_ICONWARNING | MB_OK);  // Pop-up warning message box.
    }

    FLAGSdll(hFlag, &jFlag, &kFlag, &ierr, herr, refpropcharlength, errormessagelength);

    // Check for errors:
    // 601 = Invalid flag name
    if (ierr != 0) {
        if (ierr == 601)
            return MAKELRESULT(BAD_FLAG, 1);               // hFlag is invalid
        else
            return MAKELRESULT(UNKNOWN, 2);                // Error setting Flag
    }

    ret->real = kFlag;       // return value set in kFlag

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_FLAGS =
{
    (char *)("rp_FLAGS"),               // Name by which Mathcad will recognize the function
    (char *)("flag, jflag"),            // rp_FLAGS will be called as rp_FLAGS(flag,jflag)
    (char *)("Sets the flag specified in the string parameter, flag, to the value passed in jflag; -999 retrieve current value."),
                                        // Description of rp_FLAGS(flag,jflag)
    (LPCFUNCTION)rp_Flags,              // Pointer to the executable code
    COMPLEX_SCALAR,                     // The return type is a complex scalar
    2,                                  // The function takes 2 arguments
    { MC_STRING,                        //   - 1st argument is a string
      COMPLEX_SCALAR }                  //   - 2nd argument is a scalar
};
    