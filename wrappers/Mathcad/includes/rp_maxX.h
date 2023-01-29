LRESULT rp_Criconden(
    LPCOMPLEXARRAY      ret,
    LPCMCSTRING       fluid,   
    LPCMCSTRING           X  )
{
    char herr[255];
    unsigned int lherr = 255;
    ierr = 0;
    unsigned int rows = 3, cols = 1;
    double ttrip, tnbpt, tc = 0.0, pc = 0.0, Dc = 0.0, Zc, acf, dip, Rgas, wmm0;

    std::string ccType = upper(X->str);            // Get ccType from second parameter (upper case)
    if ((ccType != "P") && (ccType != "T"))        // If ccType is neither "T" or "P", fail early
        return MAKELRESULT(INVALID_CCTYPE, 2);     //    Throw error on second argument.

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    // allocate memory for T, P, and D values
    if (!MathcadArrayAllocate(ret,                 // allocate memory for ret array
        rows,                                      // number of rows (3)
        cols,                                      // number of columns (1)
        TRUE,                                      // allocate real part = TRUE
        FALSE))                                    // allocate imaginary pat = FALSE
    {
        // if allocation is unsuccessful, return the appropriate error code
        return MAKELRESULT(INSUFFICIENT_MEMORY, 1);
    }

    if (ncomp == 1)                                // if not a mixture, return critical point values
    {
        int icomp = 1;
        INFOdll(&icomp, &wmm0, &ttrip, &tnbpt, &tc, &pc, &Dc, &Zc, &acf, &dip, &Rgas);
    }
    else                                           // *** (ncomp > 1) this is a Mixture ***
    {
        if (ccType == "T")                         // if need Cricondentherm
        {                                          //    get MAXT point
            MAXTdll(&x[0], &tc, &pc, &Dc, &ierr, herr, lherr);
            if (ierr != 0)
            {
                if (ierr == 331)
                    return MAKELRESULT(NO_SPLINES, 2);
                else if ((ierr == 1) || (ierr == -361))
                    return MAKELRESULT(MAX_T_UNKNOWN, 2);
                else
                    return MAKELRESULT(UNKNOWN, 2);
            }
        }
        else                                       // need Cricondenbar
        {                                          //    get MAXT point
            MAXPdll(&x[0], &tc, &pc, &Dc, &ierr, herr, lherr);
            if (ierr != 0)
            {
                if (ierr == 331)
                    return MAKELRESULT(NO_SPLINES, 2);
                else if ((ierr == 1) || (ierr == -362))
                    return MAKELRESULT(MAX_P_UNKNOWN, 2);
                else
                    return MAKELRESULT(UNKNOWN, 2);
            }
        }
        wmm0 = wmm;
    }

    if (isUserInterrupted())
    {
        // if user has interrupted -- free the allocated memory
        MathcadArrayFree(ret);
        // and return with an appropriate error code
        return MAKELRESULT(INTERRUPTED, 1);
    }

    ret->hReal[0][0] = tc;
    ret->hReal[0][1] = pc / 1000.0;    // convert from kPa to MPa
    ret->hReal[0][2] = Dc * wmm;       // convert from mol/L to kg/m³

    return 0;                          // return 0 to indicate there was no error
            
}           

    FUNCTIONINFO    rp_maxX = 
{ 
    (char *)("rp_maxX"),                // Name by which Mathcad will recognize the function
    (char *)("fluid,X"),                // rp_maxX will be called as rp_maxX(fluid,X)
    (char *)("Returns either the maximum saturation temperature (X = T) or saturation pressure (X = P) point (T,P,D) of the fluid/mixture specified"),
                                        // description of rp_maxX(fluid,X)
    (LPCFUNCTION)rp_Criconden,          // pointer to the executable code
    COMPLEX_ARRAY,                      // the return type is a complex array
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // 1st argument (fluid) is an MC_STRING
      MC_STRING }                       // 2nd argument (X) is an MC_STRING
};
