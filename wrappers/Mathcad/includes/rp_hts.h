LRESULT rp_Hts(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      s   )
{
	char herr[255];
	double pval,hval,sval,dval,tval;
	double rhol,rhov,xliq[20],xvap[20],qval,uval,cv,cp,wval;
	int ierr;
	int kph = 1;
	int kr = 1;

	ierr = cSetup(fluid->str);
	if (ierr > 0)
		return MAKELRESULT(ierr,1);

    if( t->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		tval = t->real;  // Get temperature in K

    if (tval > Tmax*(1 + 0.5*extr)) return MAKELRESULT(T_OUT_OF_RANGE, 2);

    if( s->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,3);
	else
		sval = s->real * wmm;  // Convert from kJ/kg-K to J/mol-K

	pval = 0.0;
	hval = 0.0;

    TSFLSHdll(&tval,&sval,&x[0],&kr,&pval,&dval,&rhol,&rhov,&xliq[0],&xvap[0],&qval,
              &uval,&hval,&cv,&cp,&wval,&ierr,herr,errormessagelength);

	if (ierr > 0)
	{
		if (ierr == 249)
		{
			return MAKELRESULT(H_OUT_OF_RANGE, 3);
		}
        else if (ierr == 250)
        {
            return MAKELRESULT(S_OUT_OF_RANGE, 3);
        }
		else if (ierr <= 12)
		{
			return MAKELRESULT(P_OUT_OF_RANGE, 1);
		}
		else
		{
			return MAKELRESULT(UNCONVERGED, 2);
		}
	}

	ret->real = hval / wmm;       // Returned in kJ/kg

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_hts = 
{ 
    (char *)("rp_hts"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,p,s"),              // rp_hts will be called as rp_hts(fluid,t,s)
    (char *)("Returns the enthalpy [kJ/kg] given the pressure [MPa] and entropy [kJ/kg-K]"),
										// description of rp_hts(fluid,t,s)
    (LPCFUNCTION)rp_Hts,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 2 arguments
    { MC_STRING,                        // String argument
	  COMPLEX_SCALAR,
	  COMPLEX_SCALAR }                  // arguments are complex scalars
};