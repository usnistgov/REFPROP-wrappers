LRESULT rp_Ptrho(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR    rho   )
{
	double tval,pval,dval;
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

	PRESSdll(&tval, &dval, &x[0], &pval);  // [K], [mol/L], [kPa]

	ret->real = pval / 1000.0;   // Convert from kPa to MPa

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_ptrho = 
{ 
    (char *)("rp_ptrho"),               // Name by which mathcad will recognize the function
    (char *)("t,rho"),                  // rp_ptrho will be called as rp_ptrho(t,rho)
    (char *)("Returns the pressure [MPa] given the temperature [K] and density [kg/m³]"),
										// description of rp_ptrho(t,rho)
    (LPCFUNCTION)rp_Ptrho,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes 3 arguments
    { MC_STRING,                        // String argument
	  COMPLEX_SCALAR,
	  COMPLEX_SCALAR }                  // arguments are complex scalars
};
    
    
    