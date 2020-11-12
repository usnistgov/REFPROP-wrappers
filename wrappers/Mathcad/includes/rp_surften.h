LRESULT rp_Surften(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t   )
{
	char herr[255];
	int kph = 1;
	int ierr;
	double psat,tsat,rhol,rhov,xliq[20],xvap[20],sigma;

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);
	
    if( t->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		tsat = t->real;
		
	SATTdll(&tsat,&x[0],&kph,&psat,&rhol,&rhov,xliq,xvap,&ierr,herr,errormessagelength);

    if (ierr != 0)
    {
        if ((ierr == 1) || (ierr == 9) || (ierr == 121))
            return MAKELRESULT(T_OUT_OF_RANGE, 2); // Temperature too low | negative | > Tcrit
        else if (ierr == 8)
            return MAKELRESULT(BAD_COMPONENT, 1); // x out of range
        else
            return MAKELRESULT(UNCONVERGED, 2); // failed to converge
    }

	SURTENdll(&tsat, &rhol, &rhov, &xliq[0], &xvap[0], &sigma, &ierr, herr, errormessagelength);

	if (ierr != 0)
	{
		if ((ierr == 1)||(ierr == 9)||(ierr == 121)) 
			return MAKELRESULT(T_OUT_OF_RANGE,2); // Temperature too low | negative | > Tcrit
		else
			return MAKELRESULT(UNCONVERGED,2); // failed to converge
	}
	if (sigma < 0)
		return MAKELRESULT(NO_SURFTEN,1);

	ret->real = sigma;      // Return value in units of N/m

    return 0;               // return 0 to indicate there was no error
            
} 

FUNCTIONINFO    rp_surften = 
{ 
    (char *)("rp_surften"),             // Name by which mathcad will recognize the function
    (char *)("fluid,t"),                // rp_surften will be called as rp_surften(fluid,t)
    (char *)("Returns the surface tension [N/m] given the saturation temperature [K]"),
										// description of rp_surften(fluid,t)
    (LPCFUNCTION)rp_Surften,            // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // String argument
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
