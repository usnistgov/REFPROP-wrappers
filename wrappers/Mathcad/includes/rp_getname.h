LRESULT rp_Getname(
    LPMCSTRING          hstr,
    LPCMCSTRING        fluid,
    LPCCOMPLEXSCALAR    comp)
{
    char *pcomment;
    unsigned int nstr;
    int icomp;
    char hname[namelengthshort + 1];
    char hfull[namelengthlong + 1];
    char hcasn[casnlength + 1];
    ierr = 0;
    std::string strFluid = fluid->str;
    std::string Ad_Hoc = "Ad-Hoc Mixture";

    // Call cSetup to parse out the fluid string and either call
    //      SETUPdll for a single pure fluid or SETMIXdll for
    //      a mixture.

    ierr = cSetup(strFluid);

    if (ierr > 0)
        return MAKELRESULT(ierr, 1);

    icomp = static_cast<int>(comp->real);

    if ((icomp == 0) && (ncomp > 1))                                 // if this is a mixture,
    {
        strcpy(hfull, MixName);                                      //   Get name from 1st line of mixture file.
        nstr = (unsigned int)strlen(hfull);                          //   Set length of Mixture Name string
    }
    else
    {                                                                // Pure Fluid (ncomp = 1)
        if (icomp == 0)                                              // if user is requesting component 0 name
        {
            if (ncomp == 1)                                          //     if there's only one component...
                icomp = 1;                                           //         Assume user meant component 1 and get the name
            else if (ncomp > 1)                                      //     if this is a custom mixture...
                return MAKELRESULT(INDIV_COMPONENT, 2);              //         throw message component name 0 undefined.
        }
        else if (icomp > ncomp)                                      // if Component requested > number of components
            return MAKELRESULT(BAD_COMPONENT, 2);                    //     throw and error message

        // Get the i-th component long name from REFPROP and make sure it's null terminated.
        NAMEdll(&icomp, hname, hfull, hcasn, namelengthshort, namelengthlong, casnlength);
        //hfull[namelengthlong] = '\0';   // Not needed  NAMEdll takes care of null ending.
    }

    // strip any spaces off end of fluid name
    pcomment = strstr(hfull, "  ");
    if (pcomment != NULL)
        nstr = (unsigned int)(pcomment - hfull);
    else
        nstr = (unsigned int)strlen(hfull);

    // allocate memory for return string
    char * pdest = MathcadAllocate(nstr + 1u);
    if (pdest == NULL )
        return MAKELRESULT(INSUFFICIENT_MEMORY,2);  // insufficient memory
    else
    {
        strncpy(pdest,hfull,(size_t)nstr);
        hstr->str = pdest;
    }
    return 0;               // return 0 to indicate there was no error            
}

FUNCTIONINFO    rp_getname = 
{ 
    (char *)("rp_getname"),         // Name by which Mathcad will recognize the function
    (char *)("fluid,icomp"),        // rp_getname will be called as rp_getname(fluid,icomp)
                                    // description of rp_getname(fluid,icomp)
    (char *)("Returns the full fluid name for the specified fluid component (icomp = n) or mixture (icomp = 0)"),
    (LPCFUNCTION)rp_Getname,        // Pointer to the executable code
    MC_STRING,                      // The return type is a STRING
    2,                              // The function takes two (2) arguments
    { MC_STRING,                    // fluid name argument is a String 
    COMPLEX_SCALAR }                // component argument is a scalar
};
