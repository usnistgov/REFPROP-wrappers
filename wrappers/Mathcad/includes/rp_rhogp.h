LRESULT rp_Rhogp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p   )
{
	char herr[256] = "OK\0";
    unsigned int lenherr = 3;
	int kph = 2;   // looking for saturated vapor (dew point)
	int ierr;
    double psat, tsat, rhol, rhov, xliq[20] = { 0.0 }, xvap[20] = { 0.0 };

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);
	
    if( p->imag != 0.0 )
		return MAKELRESULT(1,1);
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
	ret->real = rhov*wmm;   // Convert from mol/L to kg/m³

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_rhogp = 
{ 
    (char *)("rp_rhogp"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,p"),                // rp_rhogp will be called as rp_rhogp(fluid,p)
    (char *)("Returns the saturation liquid density [kg/m³] given the pressure [MPa]"),
										// description of rp_rhogp(fluid,p)
    (LPCFUNCTION)rp_Rhogp,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 2 arguments
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    
    
    