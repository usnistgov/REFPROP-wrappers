LRESULT rp_Getpath( 
    LPMCSTRING          hstr,     // pointer to the results string
    LPCCOMPLEXSCALAR    idum   )  // pointer to the dummy parameter received from Mathcad
{
    // NOTE: we don't care what gets passed in.
    // Return string should already be stored when REFPROP DLL is loaded...
    char * c = MathcadAllocate(static_cast<int>(RPPath_loaded.size()) + 1); // Create a c-string (pointer) with same size as s
                                                                            // This process avoids the const-cast type which would result from,
                                                                            //     instead, converting the string using s.c_str()
    std::copy(RPPath_loaded.begin(), RPPath_loaded.end(), c);               // Copy s into c. 
    c[RPPath_loaded.size()] = '\0';                                         // Append with the null character
    hstr->str = c;                                                          // Assign the string to the output parameter
    return 0;                                                               // normal return
}

FUNCTIONINFO    rp_getpath = 
{                                                               // These first three strings are not actually used!!!
    (char *)("rp_getpath"),                                     // Name by which Mathcad will recognize the function
    (char *)("idum"),                                           // rp_getpath will be called as rp_getpath(idum)
    (char *)("Returns the full path for the fluid directory"),  // description of rp_getpath(idum)					
    (LPCFUNCTION)rp_Getpath,                                    // pointer to the executable code
    MC_STRING,                                                  // the return type is an MC_STRING
    1,                                                          // the function takes one (1) dummy argument
    { COMPLEX_SCALAR }                                          // argument is a scalar
};
    