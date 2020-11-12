LRESULT rp_Ktp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      p)
{
    double mu, cond;
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

    TRNPRPdll(&tval,&Dval,x,&mu,&cond,&ierr,herr,errormessagelength);

	// check for errors and return MAKELRESULT(n,1)
    if (ierr > 0)
    {
        if ((ierr == 39) || (ierr == 40) || (ierr == 50))
            return MAKELRESULT(NO_TRANSPORT, 1);     // conductivity model not defined
        else if (ierr == 51)
            return MAKELRESULT(INFINITE_K, 3);
        else
            return MAKELRESULT(UNCONVERGED, 2);
    }
    else if (ierr < 0) {
        if ((ierr == -31) || (ierr == -33) || (ierr == -51) || (ierr == -53))
            return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
        else if ((ierr == -32) || (ierr == -52))
            return MAKELRESULT(D_OUT_OF_RANGE, 3);  // Pressure (density) out of bounds
        else if (ierr <= -58)
            return MAKELRESULT(UNCONVERGED, 2);     // did not converge
    }
    if (cond < 0)
		return MAKELRESULT(UNCONVERGED,2);

	ret->real = cond;       // returned in W/m-K

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_ktp =
{
    (char *)("rp_ktp"),                // Name by which mathcad will recognize the function
    (char *)("fluid,t,p"),             // rp_ktp will be called as rp_ktp(fluid,t,p)
    (char *)("Returns the thermal conductivity [W/m-s] given the temperature [K] and pressure [MPa]"),
                                       // description of rp_ktp(fluid,t,p)
    (LPCFUNCTION)rp_Ktp,               // pointer to the executable code
    COMPLEX_SCALAR,                    // the return type is a complex scalar
    3,                                 // the function takes on 3 arguments
    { MC_STRING,                       // String argument
    COMPLEX_SCALAR,
    COMPLEX_SCALAR }                   // arguments are complex scalars
};
