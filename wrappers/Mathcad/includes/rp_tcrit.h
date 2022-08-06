LRESULT rp_Tcrit(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR   comp   )
{
    char herr[255];
    unsigned int lherr = 255;
    int ierr = 0;
    int icomp = 1;
    double ttrip, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas; 

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( comp->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        icomp = static_cast<int>(comp->real);
    
    if ((icomp > ncomp)||(icomp < 0))
        return MAKELRESULT(BAD_COMPONENT,2);
    else if ((icomp == 0)&&(ncomp > 1))
    {
        CRITPdll(&x[0],&tc,&pc,&Dc,&ierr,herr,lherr);
        if (ierr > 0)
        {
            return MAKELRESULT(UNCONVERGED,2);
        }
    }
    else 
    {
        if (icomp == 0) icomp = 1;
        INFOdll(&icomp,&wmm,&ttrip,&tnbpt,&tc,&pc,&Dc,&Zc,&acf,&dip,&Rgas);
    }
        
    ret->real = tc;

    return 0;               // return 0 to indicate there was no error

}

    FUNCTIONINFO    rp_tcrit = 
{ 
    (char *)("rp_tcrit"),               // Name by which Mathcad will recognize the function
    (char *)("fluid,comp"),             // rp_tcrit will be called as rp_tcrit(fluid,comp)
    (char *)("Returns molecular weight [g/mol] of the component number specified"),
                                        // description of rp_tcrit(fluid,comp)
    (LPCFUNCTION)rp_Tcrit,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
