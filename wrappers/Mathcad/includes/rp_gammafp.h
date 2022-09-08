LRESULT rp_Gfp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p   )
{
    char herr[255];
    int kph = 1;       //  kph = 1 (saturated liquid conc.); kph = 2 (saturated vapor conc.)
    int ierr;
    double psat, tsat, rhol, rhov, xliq[20], xvap[20];
    double Cv = 0.0, Cp = 0.0;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);
    
    if( p->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        psat = p->real * 1000.0;

    SATPdll(&psat, &x[0], &kph, &tsat, &rhol, &rhov, &xliq[0], &xvap[0], &ierr, herr, errormessagelength);

    if (ierr > 0)
    {
        if ((ierr == 2) || (ierr == 4) || (ierr == 12) || (ierr == 141))
            return MAKELRESULT(P_OUT_OF_RANGE,2); // Pressure too low | negative | > Pcrit
        else
            return MAKELRESULT(UNCONVERGED,2); // failed to converge
    }

    CVCPdll(&tsat, &rhol, &x[0], &Cv, &Cp);

    ret->real = Cp / Cv;    // return gamma [dimensionless]

    return 0;               // return 0 to indicate there was no error
            
}  

FUNCTIONINFO    rp_gammafp = 
{ 
    (char *)("rp_gammafp"),             // Name by which mathcad will recognize the function
    (char *)("fluid,p"),                // rp_gammafp will be called as rp_gammafp(fluid,p)
    (char *)("Returns the saturation liquid heat capacity ratio, Gamma [-] given the pressure [MPa]"),
                                        // description of rp_gammafp(fluid,p)
    (LPCFUNCTION)rp_Gfp,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // argument is a MC_STRING
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
    