LRESULT rp_GetMajor(
    LPCOMPLEXSCALAR   ret,     // Return REFPROP version number as a real
    LPCCOMPLEXSCALAR  idum  )  // pointer to the dummy parameter received from Mathcad
{
    // NOTE: we don't care what gets passed in, but this clears the unused variable warning.
    if (idum->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    // Return value should already be stored when REFPROP DLL is loaded...
    ret->real = vMajor + vMinor/10.0;                                            // assign the major.minor to the output parameter
    return 0;                                                                    // normal return
}

FUNCTIONINFO    rp_getRPnum = 
{                                       // These first three strings are not actually used!!!
    (char *)("rp_getRPnum"),            // Name by which Mathcad will recognize the function
    (char *)("idum"),                   // rp_getRPnum will be called as rp_getRPnum(idum); has to have a parameter.
    (char *)("Returns the NIST Refprop version as a number."),  // description of rp_getRPnum(idum)
    (LPCFUNCTION)rp_GetMajor,           // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    1,                                  // the function takes one (1) argument
    { COMPLEX_SCALAR }                  // argument is a scalar
};
    
    
    