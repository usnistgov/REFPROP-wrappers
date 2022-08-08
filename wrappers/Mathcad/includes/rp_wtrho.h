LRESULT rp_Wtrho(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR    rho   )
{
    double tval, dval;
    double P, U, H, S, Cv, Cp, W, hjt;
    int ierr;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( t->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        tval = t->real;

    if( rho->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,3);
    else
        dval = rho->real / wmm;  // Convert from kg/m³ to mol/L
    
    THERMdll(&tval, &dval, &x[0], &P, &U, &H, &S, &Cv, &Cp, &W, &hjt);

    ret->real = W;       // returned in m/s

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_wtrho = 
{ 
    (char *)("rp_wtrho"),               // Name by which mathcad will recognize the function
    (char *)("fluid,t,rho"),            // rp_wtrho will be called as rp_wtrho(fluid,t,rho)
    (char *)("Verification Function: Returns the speed of sound [m/s] given the temperature [K] and density [kg/m³]"),
                                        // description of rp_wtrho(fluid,t,rho)
    (LPCFUNCTION)rp_Wtrho,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes 3 arguments
    { MC_STRING,                        // String argument
      COMPLEX_SCALAR,
      COMPLEX_SCALAR }                  // arguments are complex scalars
};