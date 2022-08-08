LRESULT rp_Pts(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      s   )
{
    char herr[errormessagelength];
    double pval,sval,Dval,tval,qval;
    double rhol, rhov, xliq[20], xvap[20];
    double U, H, Cv, Cp, W;
    int ierr;
    int kr = 1;   // No dual roots for entropy functions

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( t->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        tval = t->real;  // Temperature in K

    if (tval > Tmax*(1 + 0.5*extr)) return MAKELRESULT(T_OUT_OF_RANGE, 2);

    if( s->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,3);
    else
        sval = s->real * wmm;  // Convert from kJ/kg-K to J/mol-K

    pval = 0.0;

    TSFLSHdll(&tval, &sval, &x[0], &kr, &pval, &Dval, &rhol, &rhov, &xliq[0], &xvap[0], &qval, &U, &H, &Cv, &Cp, &W, &ierr, herr, errormessagelength);

    if (ierr > 0)
    {
        if (ierr == 250)
        {
            return MAKELRESULT(S_OUT_OF_RANGE,3);
        }
        else if (ierr <= 12)
        {
            return MAKELRESULT(P_OUT_OF_RANGE,2);
        }
        else
        {
            return MAKELRESULT(UNCONVERGED,1);
        }
    }

    // If pval > Pmax, return error 4
    if (pval > Pmax + extr * Pmax)
        return MAKELRESULT(P_OUT_OF_RANGE,1);

    // Otherwise...
    ret->real = pval / 1000.0;       // Returned in MPa

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_pts = 
{ 
    (char *)("rp_pts"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,t,s,r"),            // rp_pts will be called as rp_pts(fluid,t,s,r)
    (char *)("Returns the pressure [MPa] given the temperature [K] and entropy [kJ/kg-K]"),
                                        // description of rp_pts(fluid,t,s,r)
    (LPCFUNCTION)rp_Pts,				// pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 3 arguments
    { MC_STRING,                        // String argument
      COMPLEX_SCALAR,
      COMPLEX_SCALAR }                  // arguments are complex scalars
};

    