LRESULT rp_Zfp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p   )
{
    char herr[255];
    int kph = 1;       //  kph = 1 (saturated liquid conc.); kph = 2 (saturated vapor conc.)
    ierr = 0;
    int icomp = 1;
    double psat, tsat, rhol, rhov, xliq[20], xvap[20];
    double tc, pc, Dc, Rgas, ttrip, tnbpt, Zc, acf, dip;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( p->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        psat = p->real * 1000.0;

    SATPdll(&psat,&x[0],&kph,&tsat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,errormessagelength);

    if (ierr > 0)
    {
        if ((ierr == 2) || (ierr == 4) || (ierr == 12) || (ierr == 141))
            return MAKELRESULT(P_OUT_OF_RANGE, 2); // Pressure too low | negative | > Pcrit
        else if (ierr == 8)
            return MAKELRESULT(X_SUM_NONUNITY, 1);
        else
            return MAKELRESULT(UNCONVERGED,2); // failed to converge
    }

    // Update sat liquid temperature if mixture using correct density
    TSATDdll(&rhol, &x[0], &tsat, &ierr, herr, errormessagelength);

    // Get Rgas
    if (ncomp > 1)
    {
        RMIX2dll(&x[0], &Rgas);              // mixture gas constant [J/mol-K]
    }
    else
    {
        INFOdll(&icomp, &wmm, &ttrip, &tnbpt, &tc, &pc, &Dc, &Zc, &acf, &dip, &Rgas);
    }

    ret->real = psat / (Rgas * tsat * rhol);   // calculate compressibility factor

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_zfp =
{
    (char *)("rp_zfp"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,p"),                // rp_zfp will be called as rp_zfp(fluid,p)
    (char *)("Returns the saturation liquid compressibility factor [-] given the pressure [MPa]"),
                                        // description of rp_zfp(fluid,p)
    (LPCFUNCTION)rp_Zfp,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // argument is a MC_STRING
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
