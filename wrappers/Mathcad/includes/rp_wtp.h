LRESULT rp_Wtp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      p   )
{
	double tval,pval,Dval;
    double Dl, Dv, Q, U, H, S, Cv, Cp, W;
    double xl[20], xv[20];
	int ierr = 0;
    char herr[255];

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);

    if( t->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		tval = t->real;

    if (tval > Tmax*(1 + 0.5*extr)) return MAKELRESULT(T_OUT_OF_RANGE, 2);

    if( p->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,3);
	else
		pval = p->real * 1000.0;

    if (pval > Pmax*(1 + extr)) return MAKELRESULT(P_OUT_OF_RANGE, 3);

    TPFLSHdll(&tval, &pval, &x[0], &Dval,   // [K], [kPa], [mol/L]
        &Dl, &Dv, &xl[0], &xv[0],           // Saturation terms
        &Q, &U, &H, &S, &Cv, &Cp, &W,       // Thermo properties
        &ierr, herr, errormessagelength);   // error code and string

    if (ierr > 0)
        if ((ierr == 1) || (ierr == 5) || (ierr == 9) || (ierr == 13))
            return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
        else if ((ierr == 4) || (ierr == 12))
            return MAKELRESULT(P_OUT_OF_RANGE, 1);  // Pressure out of bounds
        else
            return MAKELRESULT(UNCONVERGED, 2);     // one of many convergence errors

	ret->real = W;

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_wtp = 
{ 
    (char *)("rp_wtp"),                 // Name by which Mathcad will recognize the function
    (char *)("fluid,t,p"),              // rp_wtp will be called as rp_wtp(fluid,t,p)
    (char *)("Returns the speed of sound [m/s] given the temperature [K] and pressure [MPa]"),
										// description of rp_wtp(fluid,t,p)
    (LPCFUNCTION)rp_Wtp,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 3 arguments
    { MC_STRING,                        // String argument
	  COMPLEX_SCALAR,
	  COMPLEX_SCALAR }                  // arguments are complex scalars
};
    
    
    