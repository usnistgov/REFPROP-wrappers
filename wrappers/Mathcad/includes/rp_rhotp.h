LRESULT rp_Rhotp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      p   )
{
	double tval,pval,Dval;
    double Dl, Dv, Q, U, H, S, Cv, Cp, W;
    double xl[20], xv[20];
	int ierr;
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
		pval = p->real * 1000.0;   // Convert from MPa to kPa for REFPROP inputs

    if (pval > Pmax*(1 + extr)) return MAKELRESULT(P_OUT_OF_RANGE, 3);

    TPFLSHdll(&tval, &pval, &x[0], &Dval,   // [K], [kPa], [mol/L]
        &Dl, &Dv, &xl[0], &xv[0],           // Saturation terms
        &Q, &U, &H, &S, &Cv, &Cp, &W,       // Thermo properties
        &ierr, herr, errormessagelength);   // error code and string

    if (ierr > 0) {
        // Use this pop-up window for debugging if needed
        //===============================================================================
        //std::string msg;
        //msg = "NIST RefProp DLL Version: " + RPVersion_loaded;
        //msg.append("\n\nError in rp_rhotp:\n\n");
        //msg.append(herr);
        //MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);

        if ((ierr == 1) || (ierr == 5) || (ierr == 9) || (ierr == 13))
            return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
        else if (ierr == 8)
            return MAKELRESULT(X_SUM_NONUNITY, 1);  // component and/or sum < 0 or > 1
        else if ((ierr == 4) || (ierr == 12))
            return MAKELRESULT(P_OUT_OF_RANGE, 1);  // Pressure out of bounds
        else
            return MAKELRESULT(UNCONVERGED, 2);     // one of many convergence errors
    }
	ret->real = Dval*wmm;   // Convert from mol/L to kg/m³

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_rhotp = 
{ 
    (char *)("rp_rhotp"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,t,p"),              // rp_rhotp will be called as rp_rhotp(fluid,t,p)
    (char *)("Returns the density [kg/m³] given the temperature [K] and pressure [MPa]"),
										// description of rp_rhotp(fluid,t,p)
    (LPCFUNCTION)rp_Rhotp,              // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 3 arguments
    { MC_STRING,
	  COMPLEX_SCALAR,
	  COMPLEX_SCALAR }                  // arguments are complex scalars
};
    
    
