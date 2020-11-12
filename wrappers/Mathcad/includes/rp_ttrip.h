LRESULT rp_Ttrip(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR   comp   )
{
	char herr[256] = "OK\0";
	char htyp[4] = "EOS";
	unsigned int lhtyp = 3;
    unsigned int lherr = 255;
	int ierr = 0;
	int icomp = 1;
	double wmm, ttrip, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas; 
	double tmax,Dmax,pmax;

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);

    if( comp->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		icomp = (int)comp->real;
	
	if ((icomp > ncomp)||(icomp < 0))
		return MAKELRESULT(BAD_COMPONENT,2);
	else if ((icomp == 0)&&(ncomp > 1))
		LIMITSdll(htyp,&x[0],&ttrip,&tmax,&Dmax,&pmax,lhtyp);
	else 
	{
		if (icomp == 0) icomp = 1;
		INFOdll(&icomp,&wmm,&ttrip,&tnbpt,&tc,&pc,&Dc,&Zc,&acf,&dip,&Rgas);
	}
		
	ret->real = ttrip;

    return 0;               // return 0 to indicate there was no error
            
} 

FUNCTIONINFO    rp_ttrip = 
{ 
    (char *)("rp_ttrip"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,comp"),             // rp_ttrip will be called as rp_ttrip(fluid,comp)
    (char *)("Returns the triple point temperature [K] of the component number specified"),
										// description of rp_ttrip(fluid,comp)
    (LPCFUNCTION)rp_Ttrip,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    