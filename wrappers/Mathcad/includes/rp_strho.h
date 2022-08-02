LRESULT rp_Strho(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR    rho   )
{
    double tval, dval, S;
    int ierr;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr, 1);

    if (t->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    else
        tval = t->real;

    if (rho->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 3);
    else
        dval = rho->real / wmm;  // Convert from kg/m³ to mol/L

    ENTROdll(&tval,&dval, &x[0], &S);  // [K], [mol/L], [J/mol-K]

    ret->real = S / wmm;   // Convert from J/mol-K to kJ/kg-K

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_strho = 
{ 
    (char *)("rp_strho"),               // Name by which mathcad will recognize the function
    (char *)("t,rho"),                  // rp_strho will be called as rp_strho(t,rho)
    (char *)("Verification Function: Returns the entropy [J/kg-K] given the temperature [K] and density [kg/m³]"),
                                        // description of rp_strho(t,rho)
    (LPCFUNCTION)rp_Strho,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 3 arguments
    { MC_STRING,                        // String argument
      COMPLEX_SCALAR,
      COMPLEX_SCALAR }                  // arguments are complex scalars
};
