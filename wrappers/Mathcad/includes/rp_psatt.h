LRESULT rp_Psatt(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t   )
{
	char herr[256] = "OK\0";
	int kph = 1;
	int ierr;
	double psat,tsat,rhol,rhov,xliq[20],xvap[20];

	ierr = cSetup(fluid->str);
	if (ierr > 0)
		return MAKELRESULT(ierr,1);
	
    if( t->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		tsat = t->real;
		
	SATTdll(&tsat,&x[0],&kph,&psat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,errormessagelength);

	if (ierr > 0)
	{
		if ((ierr == 1)||(ierr == 9)||(ierr == 121)) 
			return MAKELRESULT(T_OUT_OF_RANGE,2); // Temperature too low | negative | > Pcrit
		else
			return MAKELRESULT(UNCONVERGED,2); // failed to converge
	}

	ret->real = psat / 1000.0;  // convert from kPa to MPa

    return 0;               // return 0 to indicate there was no error

}           
    
FUNCTIONINFO    rp_psatt = 
{ 
    (char *)("rp_psatt"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,t"),                // rp_psatt will be called as rp_psatt(fluid,t)
    (char *)("Returns the saturation pressure [MPa] given the temperature [K]"),
										// description of rp_psatt(fluid,t)
    (LPCFUNCTION)rp_Psatt,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    
    
