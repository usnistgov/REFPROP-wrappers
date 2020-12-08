LRESULT rp_GetCAS(
    LPMCSTRING          hstr,
    LPCMCSTRING        fluid,
    LPCCOMPLEXSCALAR    comp)
{
    char *pdest;
    char *pcomment;
    int nstr;
    int icomp;
    char hname[namelengthshort+1];
    char hfull[namelengthlong+1];
    char hcasn[casnlength+1];
    int ierr = 0;
    std::string strFluid = fluid->str;

    // Call cSetup to parse out the fluid string and either call
    //      SETUPdll for a single pure fluid or SETMIXdll for
    //      a mixture.

    ierr = cSetup(strFluid);

    if (ierr != 0)
        return MAKELRESULT(FLUID_NOT_FOUND, 1);

    icomp = static_cast<int>(comp->real);
    if (icomp == 0)
    {
        if (ncomp == 1)
            icomp = 1;
        else if (ncomp > 1)
            return MAKELRESULT(INDIV_COMPONENT, 2);
    }
    else if (icomp > ncomp)
    {
        return MAKELRESULT(BAD_COMPONENT, 2);
    }

    NAMEdll(&icomp, hname, hfull, hcasn, namelengthshort, namelengthlong, casnlength);

    hcasn[casnlength] = '\0';

    pcomment = strstr(hcasn, "  ");
    if (pcomment != NULL)
        nstr = (unsigned int)(pcomment - hcasn);
    else
        nstr = (unsigned int)strlen(hcasn);

    // allocate memory for return string
    pdest = MathcadAllocate(nstr + 1);
    if (pdest == NULL)
        return MAKELRESULT(INSUFFICIENT_MEMORY, 2);  // insufficient memory
    else
    {
        strncpy(pdest, hcasn, nstr);
        hstr->str = pdest;
    }
    return 0;               // return 0 to indicate there was no error
}

FUNCTIONINFO    rp_getcasn =
{
    (char *)("rp_getcasn"),         // Name by which Mathcad will recognize the function
    (char *)("fluid,icomp"),        // rp_getcasn will be called as rp_getcasn(fluid,icomp)
    (char *)("Returns the CAS number for the specified fluid component as a string"),  // description of rp_getcasn(fluid,icomp)
    (LPCFUNCTION)rp_GetCAS,         // Pointer to the executable code
    MC_STRING,                      // The return type is a STRING
    2,                              // The function takes two (2) arguments
    { MC_STRING,                    // argument is a string 
    COMPLEX_SCALAR }                // argument is a scalar
};
