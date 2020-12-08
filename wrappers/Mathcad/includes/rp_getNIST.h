LRESULT rp_GetNIST(
    LPMCSTRING        hstr,    // pointer to the results string
    LPCCOMPLEXSCALAR  dum   )  // pointer to the dummy parameter received from Mathcad
{
    // NOTE: we don't care what gets passed in.
    // Return string should already be stored when REFPROP DLL is loaded...
    std::string NISTVers = "NIST RefProp Library: Version " + RPVersion_loaded;  // Extract the DLL version into string NISTVers
    char * c = MathcadAllocate(static_cast<int>(NISTVers.size()) + 1);           // Create a c-string (pointer) with same size as s
                                                                                 // This process avoids the const-cast type which would result from,
                                                                                 //     instead, converting the string using s.c_str()
    std::copy(NISTVers.begin(), NISTVers.end(), c);                              // Copy s into c.
    c[NISTVers.size()] = '\0';                                                   // tag on the null character
    hstr->str = c;                                                               // assign the string to the output parameter
    return 0;                                                                    // normal return
}

FUNCTIONINFO    rp_getNIST = 
{                                       // These first three strings are not actually used!!!
    (char *)("rp_getNIST"),             // Name by which Mathcad will recognize the function
    (char *)("idum"),                   // rp_getNIST will be called as rp_getNIST(idum)
    (char *)("Returns the NIST Refprop Library/Materials Database version."),  // description of rp_getNIST(idum)
    (LPCFUNCTION)rp_GetNIST,            // pointer to the executable code
    MC_STRING,                          // the return type is an MC_STRING
    1,                                  // the function takes one (1) argument
    { COMPLEX_SCALAR }                  // argument is a scalar
};
    
    
    