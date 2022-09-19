LRESULT rp_PsatL(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t   )
{
    char herr[256] = "OK\0";
    int kph = 1;                   // Get liquid phase pressure if mixture
    int ierr;
    double psat,tsat,rhol,rhov,xliq[20],xvap[20];

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);
    
    if( t->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        tsat = t->real;
        
    SATTdll(&tsat,&x[0],&kph,&psat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,errormessagelength);

    if (ierr > 0)
    {
        if ((ierr == 1)||(ierr == 9)||(ierr == 121)) 
            return MAKELRESULT(T_OUT_OF_RANGE,2); // Temperature too low | T & x out of range | T > Tcrit
        else
            return MAKELRESULT(UNCONVERGED,2); // failed to converge
    }

    ret->real = psat / 1000.0;  // convert from kPa to MPa

    return 0;               // return 0 to indicate there was no error

}

LRESULT rp_PsatV(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t)
{
    char herr[256] = "OK\0";
    int kph = 2;                   // Get vapor phase pressure if mixture
    int ierr;
    double psat, tsat, rhol, rhov, xliq[20], xvap[20];

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr, 1);

    if (t->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    else
        tsat = t->real;

    SATTdll(&tsat, &x[0], &kph, &psat, &rhol, &rhov, &xliq[0], &xvap[0], &ierr, herr, errormessagelength);

    if (ierr > 0)
    {
        if ((ierr == 1) || (ierr == 9) || (ierr == 121))
            return MAKELRESULT(T_OUT_OF_RANGE, 2); // Temperature too low | T & x out of range | T > Tcrit
        else
            return MAKELRESULT(UNCONVERGED, 2); // failed to converge
    }

    ret->real = psat / 1000.0;  // convert from kPa to MPa

    return 0;               // return 0 to indicate there was no error

}
    
FUNCTIONINFO    rp_psatt = 
{ 
    (char *)("rp_psatt"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,t"),                // rp_psatt will be called as rp_psatt(fluid,t)
    (char *)("Returns the saturation pressure [MPa] given the temperature [K]"),
                                        // description of rp_psatt(fluid,t)
    (LPCFUNCTION)rp_PsatL,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // argument is a string
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
    
FUNCTIONINFO    rp_psattf =
{
    (char *)("rp_psattf"),              // Name by which Mathcad will recognize the function
    (char *)("fluid,t"),                // rp_psattf will be called as rp_psattf(fluid,t)
    (char *)("Returns the Liquid saturation pressure [MPa] given the temperature [K]"),
    // description of rp_psattf(fluid,t)
    (LPCFUNCTION)rp_PsatL,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // argument is a string
    COMPLEX_SCALAR }                    // argument is a complex scalar
};

FUNCTIONINFO    rp_psattg =
{
    (char *)("rp_psattg"),              // Name by which Mathcad will recognize the function
    (char *)("fluid,t"),                // rp_psattg will be called as rp_psattg(fluid,t)
    (char *)("Returns the Vapor saturation pressure [MPa] given the temperature [K]"),
    // description of rp_psattg(fluid,t)
    (LPCFUNCTION)rp_PsatV,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // argument is a string
    COMPLEX_SCALAR }                    // argument is a complex scalar
};
