LRESULT rp_Getvers(
    LPMCSTRING          hstr,
    LPCCOMPLEXSCALAR    idum   )
{
    // NOTE: we don't care what gets passed in.
    // Return string should already be stored when REFPROP DLL is loaded...
    std::string Version = "RefProp Mathcad Add-in: Version " + rpVersion;
    char * c = MathcadAllocate(static_cast<int>(Version.size()) + 1); // Create a c-string (pointer) with same size as s
                                                                      // This process avoids the const-cast type which would result from,
                                                                      //     instead, converting the string using s.c_str()
    std::copy(Version.begin(), Version.end(), c);                     // Copy s into c.
    c[Version.size()] = '\0';                                         // tag on the null character
    hstr->str = c;                                                    // assign the string to the output parameter
    return 0;                                                         // normal return
}

FUNCTIONINFO    rp_getvers =
{                                       // These first three strings are not actually used!!!
    (char *)("rp_getvers"),             // Name by which Mathcad will recognize the function
    (char *)("idum"),                   // rp_getvers will be called as rp_getvers(idum)
    (char *)("Returns the Mathcad Add-In version number"),
                                        // description of rp_getvers(dum)
    (LPCFUNCTION)rp_Getvers,            // pointer to the executable code
    MC_STRING,                          // the return type is an MC_STRING
    1,                                  // the function takes one (1) argument
    { COMPLEX_SCALAR }                  // argument is a scalar
};
