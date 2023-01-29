LRESULT rp_Cvtrho(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR    rho   )
{
    double tval,dval,cvval,cpval;
    ierr = 0;

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

    CVCPdll(&tval, &dval, &x[0], &cvval, &cpval);  // [K], [mol/L], [], [J/mol-K], [J/mol-K]

    ret->real = cvval / wmm;      // Convert from J/mol-K to kJ/kg-K

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_cvtrho = 
{ 
    (char *)("rp_cvtrho"),              // Name by which mathcad will recognize the function
    (char *)("fluid,t,rho"),            // rp_cvtrho will be called as rp_cvtrho(t,rho)
    (char *)("Returns the isochoric specific heat [kJ/kg-K] given the temperature [K] and density [kg/m³]"),
                                        // description of rp_cvtrho(t,rho)
    (LPCFUNCTION)rp_Cvtrho,             // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 2 arguments
    { MC_STRING,                        // String argument
      COMPLEX_SCALAR,
      COMPLEX_SCALAR }                  // arguments are complex scalars
};
    
    
