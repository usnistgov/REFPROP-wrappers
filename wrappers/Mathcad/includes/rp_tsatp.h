LRESULT rp_Tsatp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p   )
{
	char herr[256] = "OK\0";
	int kph = 1;
	int ierr;
	double psat,tsat,rhol,rhov,xliq[20],xvap[20];

	ierr = cSetup(fluid->str);
	if (ierr > 0)
		return MAKELRESULT(ierr,1);
	
    if( p->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		psat = p->real * 1000.0;   // Convert from MPa to kPa for REFPROP call
		
	SATPdll(&psat,&x[0],&kph,&tsat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,errormessagelength);

	if (ierr > 0)
	{
		if ((ierr == 2)||(ierr == 4)||(ierr == 12)||(ierr == 141)) 
			return MAKELRESULT(P_OUT_OF_RANGE,2); // Pressure too low | negative | > Pcrit
		else
			return MAKELRESULT(UNCONVERGED,2); // failed to converge
	}
	ret->real = tsat;

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_tsatp = 
{ 
    (char *)("rp_tsatp"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,p"),                // rp_tsatp will be called as rp_tsatp(fluid,p)
    (char *)("Returns the saturation temperature [K] given the pressure [MPa]"),
										// description of rp_tsatp(fluid,p)
    (LPCFUNCTION)rp_Tsatp,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    
    
    