LRESULT rp_Mutp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      p)
{
	double mu,cond;
    double tval, pval, Dval;
    double Dl, Dv, Q, U, H, S, Cv, Cp, W;
    double xl[20], xv[20];
    int ierr;
    char herr[255];

    ierr = cSetup(fluid->str);
    if (ierr != 0)
        return MAKELRESULT(ierr, 1);

    if (t->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    else
        tval = t->real;

    if (tval > Tmax*(1 + 0.5*extr)) return MAKELRESULT(T_OUT_OF_RANGE, 2);

    if (p->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 3);
    else
        pval = p->real * 1000.0;   // Convert from MPa to kPa for REFPROP inputs

    if (pval > Pmax*(1 + extr)) return MAKELRESULT(P_OUT_OF_RANGE, 3);

    TPFLSHdll(&tval, &pval, &x[0], &Dval,   // [K], [kPa], [mol/L]
        &Dl, &Dv, &xl[0], &xv[0],           // Saturation terms
        &Q, &U, &H, &S, &Cv, &Cp, &W,       // Thermo properties
        &ierr, herr, errormessagelength);   // error code and string

    if (ierr > 0)
        if ((ierr == 1) || (ierr == 5) || (ierr == 9) || (ierr == 13))
            return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
        else if ((ierr == 4) || (ierr == 12))
            return MAKELRESULT(P_OUT_OF_RANGE, 3);  // Pressure out of bounds
        else
            return MAKELRESULT(UNCONVERGED, 2);     // one of many convergence errors

	TRNPRPdll(&tval,&Dval,&x[0],&mu,&cond,&ierr,herr,errormessagelength);

	// check for errors and return MAKELRESULT(n,p)
	if ((ierr > 0) && (ierr != 51))
	{
		if ((ierr == 40) || (ierr == 49) || (ierr == 50))
			return MAKELRESULT(NO_TRANSPORT,1);     // viscosity model not defined
		else 
            return MAKELRESULT(UNCONVERGED,2);
	}
    else if (ierr < 0) {
        if ((ierr == -41) || (ierr == -43) || (ierr == -51) || (ierr == -53))
            return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
        else if ((ierr == -42) || (ierr == -52))
            return MAKELRESULT(D_OUT_OF_RANGE, 3);  // Pressure (density) out of bounds
        else if (ierr <= -58)
            return MAKELRESULT(UNCONVERGED, 2);     // did not converge
    }
	if (mu < 0)
		return MAKELRESULT(UNCONVERGED,2);

	ret->real = mu;         // returned in µPa-s

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_mutp = 
{ 
    (char *)("rp_mutp"),                // Name by which mathcad will recognize the function
    (char *)("fluid,t,p"),              // rp_mutp will be called as rp_mutp(fluid,t,p)
    (char *)("Returns the viscosity [µPa-s] given the temperature [K] and pressure [MPa]"),
										// description of rp_mutp(fluid,t,p)
    (LPCFUNCTION)rp_Mutp,               // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 3 arguments
    { MC_STRING,                        // String argument
	  COMPLEX_SCALAR,
	  COMPLEX_SCALAR }                  // arguments are complex scalars
};
    