LRESULT rp_Rhomax(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid   )
{
	char herr[255];
    unsigned int lherr = 255;
	int ierr = 0;
	int icomp = 0;
	double tc, pc, Dc; 

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);

	CRITPdll(&x[0],&tc,&pc,&Dc,&ierr,herr,lherr);
	if (ierr != 0)
	{
		return MAKELRESULT(UNCONVERGED,2);
	}
		
	ret->real = Dmax*wmm;   // convert from mol/L to kg/m^3

    return 0;             // return 0 to indicate there was no error
            
}       

FUNCTIONINFO    rp_rhomax = 
{ 
    (char *)("rp_rhomax"),         // Name by which Mathcad will recognize the function
    (char *)("fluid"),             // rp_rhomax will be called as rp_rhomax(fluid)
    (char *)("Returns the maximum density [kg/m^3] of the fluid/mixture specified"),
								   // description of rp_rhomax(fluid)
    (LPCFUNCTION)rp_Rhomax,        // pointer to the executable code
    COMPLEX_SCALAR,                // the return type is a complex scalar
    1,                             // the function takes on 1 argument
    { MC_STRING }                  // argument is a complex scalar
};
    