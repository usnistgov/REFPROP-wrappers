LRESULT rp_Rhoft(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t   )
{
	char herr[256] = "OK\0";
    unsigned int lenherr = 3;
	int kph = 1;   // looking for saturated liquid (bubble point)
	int ierr;
	double psat,tsat,rhol,rhov,xliq[20],xvap[20];

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);
	
    if( t->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,1);
	else
		tsat = t->real;            // T in [K]

	SATTdll(&tsat,&x[0],&kph,&psat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,lenherr);

	if (ierr != 0)
	{
		if ((ierr == 1)||(ierr == 9)||(ierr == 121)) 
			return MAKELRESULT(T_OUT_OF_RANGE,2); // Temperature too low | negative | > Tcrit
		else if (ierr == 8)
			return MAKELRESULT(BAD_COMPONENT,2); // x out of range
        else 
            return MAKELRESULT(UNCONVERGED, 2); // failed to converge
    }
	ret->real = rhol*wmm;   // Convert from mol/L to kg/m³

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_rhoft = 
{ 
    (char*)("rp_rhoft"),                // Name by which mathcad will recognize the function
    (char*)("fluid,t"),                 // rp_rhoft will be called as rp_rhoft(fluid,t)
    (char*)("Returns the saturation liquid density [kg/m³] given the temperature [K]"),
										// description of rp_rhoft(fluid,p)
    (LPCFUNCTION)rp_Rhoft,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 2 arguments
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    
    
