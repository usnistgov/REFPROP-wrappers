LRESULT rp_Rhofp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p   )
{
	char herr[256] = "OK\0";
    unsigned int lenherr = 3;
	int kph = 1;   // looking for saturated liquid (bubble point)
	int ierr;
	double psat,tsat,rhol,rhov,xliq[20],xvap[20];

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);
	
    if( p->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,1);
	else
		psat = p->real * 1000.0;   // Convert from MPa to kPa for REFPROP input

	SATPdll(&psat,&x[0],&kph,&tsat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,lenherr);

	if (ierr != 0)
	{
		if ((ierr == 2)||(ierr == 4) || (ierr == 12) ||(ierr == 141))
			return MAKELRESULT(P_OUT_OF_RANGE,2); // Pressure too low | negative | > Pcrit
		else
			return MAKELRESULT(UNCONVERGED,1); // failed to converge
	}
	ret->real = rhol*wmm;   // Convert from mol/L to kg/m³

    return 0;               // return 0 to indicate there was no error
            
}    

FUNCTIONINFO    rp_rhofp = 
{ 
    (char *)("rp_rhofp"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,p"),                // rp_rhofp will be called as rp_rhofp(fluid,p)
    (char *)("Returns the saturation liquid density [kg/m³] given the pressure [MPa]"),
										// description of rp_rhofp(fluid,p)
    (LPCFUNCTION)rp_Rhofp,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 2 arguments
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    
    
    