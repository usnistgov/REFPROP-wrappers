LRESULT rp_Rgas(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR   comp   )
{
	int ierr = 0;
	int icomp = 1;
	double wmm, ttrip, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas; 

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);

    if( comp->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		icomp = static_cast<int>(comp->real);
	
    if ((icomp > ncomp) || (icomp < 0))
    {
        return MAKELRESULT(BAD_COMPONENT, 2);
    }
    else if( (icomp ==0 ) && (ncomp > 1) )
    {
        RMIX2dll(&x[0], &Rgas);
    }
    else
    {
        if (icomp == 0) icomp = 1;
        INFOdll(&icomp, &wmm, &ttrip, &tnbpt, &tc, &pc, &Dc, &Zc, &acf, &dip, &Rgas);
    }
    ret->real = Rgas;       // returned in J/mol-K

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_rgas = 
{ 
    (char *)("rp_rgas"),                // Name by which Mathcad will recognize the function
    (char *)("comp"),                   // rp_rgas will be called as rp_rgas(comp)
    (char *)("Returns the gas constant [J/mol-K] of the component number or mixture specified"),
										// description of rp_rgas(comp)
    (LPCFUNCTION)rp_Rgas,               // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    