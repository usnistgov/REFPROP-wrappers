LRESULT rp_GetEnum(
    LPCOMPLEXSCALAR     ret,
    LPCCOMPLEXSCALAR    Flagnum,
    LPCMCSTRING         Enum)
{
    ierr = 0;
    int iEnum = 0;
    char hEnum[refpropcharlength];              // Enumerated variable string
    std::string flag_string;                    // Flag string for easy manipulation
    char herr[errormessagelength] = "OK\0";

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);                       // Throw error: Routine not Implemented (< REFPROP 10)
        
    if( Flagnum->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL, 2);                          // Parameter cannot be complex
    else
        iFlag = static_cast<int>(Flagnum->real);                      // Extract real part to (int)jFlag

    std::string hEnum_string = Enum->str;                             // Extract first parameter to a string variable
    std::copy(hEnum_string.begin(), hEnum_string.end(), hEnum);       // Copy string flag_string into c_str hFlag.
    hEnum[hEnum_string.size()] = '\0';                                // Append with a null character

    GETENUMdll(&iFlag, hEnum, &iEnum, &ierr, herr, refpropcharlength, errormessagelength);

    // Check for errors:
    // 601 = Invalid flag name
    if (ierr != 0)
            return MAKELRESULT(BAD_ENUM, 2);               // hFlag is invalid

    ret->real = (double)iEnum;                             // return value set in iEnum

    return 0;                                              // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_GETENUM =
{
    (char *)("rp_GETENUM"),             // Name by which Mathcad will recognize the function
    (char *)("iFlag, hEnum"),           // rp_GETENUM will be called as rp_GETENUM(flag,jflag)
    (char *)("Returns the enumeration number for the string parameter, hEnum; range of enumerations set in iFlag."),
                                        // Description of rp_GETENUM(flag,jflag)
    (LPCFUNCTION)rp_GetEnum,            // Pointer to the executable code
    COMPLEX_SCALAR,                     // The return type is a complex scalar
    2,                                  // The function takes 2 arguments
    { COMPLEX_SCALAR,                   //   - 1st argument is a scalar (actually an integer)
      MC_STRING }                       //   - 2nd argument is a string (lookup Enumeration string)
};