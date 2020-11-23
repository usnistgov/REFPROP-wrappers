LRESULT rp_Wmol(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR   comp   )
{
	int icomp,ierr;
	double wmm0, ttrip, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas;

    std::string strFluid = fluid->str;
	ierr = cSetup(strFluid);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);

    if (comp->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    else
        icomp = static_cast<int>(comp->real);

    if((icomp > ncomp)||(icomp < 0))
		return 	MAKELRESULT(BAD_COMPONENT,2);   // Invalid component number

    if ((icomp == 0) && (ncomp > 1))
        wmm0 = wmm;         // this is the mixture molecular wt.
    else                    // otherwise get the individual component MW values
    {
        if (icomp == 0) icomp = 1;
        INFOdll(&icomp, &wmm0, &ttrip, &tnbpt, &tc, &pc, &Dc, &Zc, &acf, &dip, &Rgas);
    }

	ret->real = wmm0;

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_wmol = 
{ 
    (char *)("rp_wmol"),                // Name by which Mathcad will recognize the function
    (char *)("fluid,comp"),             // rp_wmol will be called as rp_wmol(fluid,comp)
    (char *)("Returns molecular weight [g/mol] of the component number specified"),
										// description of rp_wmol(fluid,comp)
    (LPCFUNCTION)rp_Wmol,               // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes two arguments
    { MC_STRING,                        // first argument is an MC_STRING
	  COMPLEX_SCALAR }                  // second argument is a complex scalar
};
    