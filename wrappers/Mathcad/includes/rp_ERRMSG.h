LRESULT rp_ErrMsg(
    LPMCSTRING        hstr,    // pointer to the results string
    LPCCOMPLEXSCALAR  iErr  )  // pointer to the dummy parameter received from Mathcad
{
    if (iErr->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);

    // Return string should already be stored when REFPROP DLL is loaded...
    int iErrNum = ierr;
    char herr[errormessagelength] = "No Error";     // Error String
    char *pdest;
    size_t nstr;

    if ((int)iErr->real == 0) iErrNum = ierr;

    if (iErrNum != 0)
        ERRMSGdll(&iErrNum, herr, errormessagelength);

    nstr = strlen(herr);
    while (' ' == herr[--nstr]) herr[nstr] = '\0';   // Strip trailing spaces
    nstr++;

    // allocate memory for return string
    pdest = MathcadAllocate((unsigned int)nstr + 1u);
    if (pdest == NULL)
        return MAKELRESULT(INSUFFICIENT_MEMORY, 2);  // insufficient memory
    else
    {
        strncpy(pdest, herr, nstr);
        pdest[nstr] = '\0';
        hstr->str = pdest;
    }

    return 0;                                                                    // normal return
}

FUNCTIONINFO    rp_ERRMSG = 
{                                       // These first three strings are not actually used!!!
    (char *)("rp_ERRMSG"),              // Name by which Mathcad will recognize the function
    (char *)("iErr"),                   // rp_ERRMSG will be called as rp_ERRMSG(iErr)
    (char *)("Returns the NIST Refprop Library/Materials Database version."),  // description of rp_ERRMSG(iErr)
    (LPCFUNCTION)rp_ErrMsg,             // pointer to the executable code
    MC_STRING,                          // the return type is an MC_STRING
    1,                                  // the function takes one (1) argument
    { COMPLEX_SCALAR }                  // argument is a scalar
};
    
    
    