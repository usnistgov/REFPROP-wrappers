LRESULT rp_SETX(
    LPMCSTRING           ret,
    LPCCOMPLEXARRAY      mfrac  )
{
    ierr = 0;
    double sum = 0.0, xtol = 1.0E-4;
    char htype[] = "EOS";
    char herr[errormessagelength];
    char *pdest;
    char hset[16] = "Composition set";

    if (mfrac->hImag != NULL)
        return MAKELRESULT(MUST_BE_REAL, 1);

    if (mfrac->rows != (unsigned int)ncomp)
        return MAKELRESULT(COMPONENT_COUNT, 1);

    if (ncomp == 1)
        return MAKELRESULT(ONLY_MIX, 1);

    for (int i = 0; i < ncomp; i++)                // Index from 0 to (ncomp - 1)
    {
        sum += mfrac->hReal[0][i];                 // Add up mixture fractions
    }
    if (abs(sum - 1.0) > xtol)
        return MAKELRESULT(X_SUM_NONUNITY, 1);     // Mixture fractions must sum to unity.

    // Loop over components and return the mole fractions in x
    for (int i = 0; i < ncomp; i++)                // Index from 0 to (ncomp - 1)
    {
        x[i] = mfrac->hReal[0][i];                 // Set each fraction from the mfrac array
    }
    LIMITSdll(htype, &x[0], &Tmin, &Tmax, &Dmax, &Pmax, lengthofreference);   // Get Tmin, Tmax, Dmzx, Pmax for this fluid/mixture
    WMOLdll(&x[0], &wmm);                                                     // Get molar mass [kg/kmol or g/mol]
    SATSPLNdll(&x[0], &ierr, herr, errormessagelength);                       // Call SATSPLN for new composition
    if (ierr > 0) return SATSPLN_FAILED;

    // allocate memory for return string
    size_t mlen = strlen(hset);
    pdest = MathcadAllocate((int)mlen + 1u);
    if (pdest == NULL)
        return MAKELRESULT(INSUFFICIENT_MEMORY, 1);  // insufficient memory
    else
    {
        strncpy(pdest, hset, mlen); pdest[mlen] = '\0';
        ret->str = pdest;
    }

    if (isUserInterrupted())
    {
        MathcadFree(pdest);
        return MAKELRESULT(INTERRUPTED, 1);  // Interrupted
    }
                                                    // No need to check for interrupt, we're here.
    return 0;                                       // return 0 to indicate there was no error

}

    FUNCTIONINFO    rp_setx = 
{ 
    (char *)("rp_setx"),                // Name by which Mathcad will recognize the function
    (char *)("mfrac"),                  // rp_setx will be called as rp_setx(mfrac)
    (char *)("Returns mole fraction array for the loaded mixture."),
                                        // description of rp_setx(mfrac)
    (LPCFUNCTION)rp_SETX,               // pointer to the executable code
    MC_STRING,                          // the return type is a String
    1,                                  // the function takes only 1 argument
    { COMPLEX_ARRAY }                   // argument is a complex array
};
