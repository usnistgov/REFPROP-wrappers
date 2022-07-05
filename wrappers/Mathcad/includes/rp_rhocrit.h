LRESULT rp_Rhocrit(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR   comp   )
{
    char herr[256] = "Ok\0";
    unsigned int lherr = 255;
    int ierr = 0;
    int icomp = 1;
    double wmm0, ttrip, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas; 

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( comp->imag != 0.0 )
        return MAKELRESULT(1,2);
    else
        icomp = static_cast<int>(comp->real);
    
    if ((icomp > ncomp)||(icomp < 0))
        return MAKELRESULT(10,2);
    else if ((icomp == 0)&&(ncomp > 1))
    {
        CRITPdll(&x[0], &tc, &pc, &Dc, &ierr, herr, lherr);
        if (ierr > 0)
        {
            return MAKELRESULT(9,2);
        }
        wmm0 = wmm;
    }
    else 
    {
        if (icomp == 0) icomp = 1;
        INFOdll(&icomp,&wmm0,&ttrip,&tnbpt,&tc,&pc,&Dc,&Zc,&acf,&dip,&Rgas);
    }
        
    ret->real = Dc*wmm0;   // convert from mol/L to kg/m^3

    return 0;             // return 0 to indicate there was no error
            
}       

FUNCTIONINFO    rp_rhocrit = 
{ 
    (char *)("rp_rhocrit"),             // Name by which Mathcad will recognize the function
    (char *)("fluid,comp"),             // rp_rhocrit will be called as rp_rhocrit(fluid,comp)
    (char *)("Returns the critical point density [kg/m^3] of the component number specified"),
                                        // description of rp_rhocrit(fluid,comp)
    (LPCFUNCTION)rp_Rhocrit,            // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
    