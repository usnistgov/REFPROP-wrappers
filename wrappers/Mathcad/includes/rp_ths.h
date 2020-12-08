LRESULT rp_Ths(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      h,
    LPCCOMPLEXSCALAR      s   )
{
	char herr[255];
	double hval, sval, tval, pval, dval, qual, eval, Cv, Cp, wval;
	double rhol, rhov, xliq[20], xvap[20];
	int ierr;
	int kph = 1;

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);

    if( h->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		hval = h->real * wmm;  // Convert from kJ/kg to J/mol

    if( s->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,3);
	else
		sval = s->real * wmm;  // Convert from kJ/kg-K to J/mol-K

	// Use the full HSFLSH function

	HSFLSHdll(&hval, &sval, &x[0], &tval, &pval, &dval, &rhol, &rhov, &xliq[0], &xvap[0], &qual, &eval, &Cv, &Cp, &wval, &ierr, herr, errormessagelength);
    if (ierr != 0)
    {
        if ((ierr == 4) || (ierr == 12))
            return MAKELRESULT(P_OUT_OF_RANGE, 1);
        else if (ierr == 249)
            return MAKELRESULT(H_OUT_OF_RANGE, 2);
        else
            return MAKELRESULT(UNCONVERGED, 3);
    }

	ret->real = tval;       // Returned in degrees K

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_ths = 
{ 
    (char *)("rp_ths"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,h,s"),              // rp_ths will be called as rp_ths(fluid,h,s)
    (char *)("Returns the temperature [K] given the enthalpy [kJ/kg] and entropy [kJ/kg-K]"),
										// description of rp_ths(fluid,h,s)
    (LPCFUNCTION)rp_Ths,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes 3 arguments
    { MC_STRING,                        // String argument
	  COMPLEX_SCALAR,
	  COMPLEX_SCALAR }                  // arguments are complex scalars
};