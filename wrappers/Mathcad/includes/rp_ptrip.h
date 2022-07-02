LRESULT rp_Ptrip(
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
    int kph = 1;
    double D,Dl,Dv,xl[20],xv[20],Umol,Hmol,Smol,Cv,Cp,w;
    double wmm, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas;
    double ttrip = 0.0;
    double ptrip = 0.0;
    double dtrip = 0.0;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( comp->imag != 0.0 )            // If Imaginary part of component not zero..
        return MAKELRESULT(MUST_BE_REAL,2);       //    Return and error,
    else
        icomp = static_cast<int>(comp->real);       //    Otherwise, get the real part.
    
    if ((icomp > ncomp)||(icomp < 0))  // Illegal component number...
        return MAKELRESULT(BAD_COMPONENT,2);      //    ...Return and error.
    else
    {
        if ((icomp == 0) && (ncomp > 1))
            LIMITSdll(htyp, &x[0], &ttrip, &Tmax, &Dmax, &Pmax, lhtyp);
        else
        {
            if (icomp == 0) icomp = 1;
            INFOdll(&icomp, &wmm, &ttrip, &tnbpt, &tc, &pc, &Dc, &Zc, &acf, &dip, &Rgas);
        }
        double _Q = 0;

        TQFLSHdll(&ttrip, &_Q, &x[0], &kph, &ptrip, &D, &Dl, &Dv, &xl[0], &xv[0], &Umol, &Hmol, &Smol, &Cv, &Cp, &w, &ierr, herr, errormessagelength);

        if (ierr > 0)
        {
            if ((ierr == 1)||(ierr == 8)||(ierr == 9))
                return MAKELRESULT(T_OUT_OF_RANGE,2);
            else
                return MAKELRESULT(UNCONVERGED,2);
        }
    }
        
    ret->real = ptrip/1000.0;  // convert from kPa to MPa

    return 0;                  // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_ptrip = 
{ 
    (char *)("rp_ptrip"),               // Name by which mathcad will recognize the function
    (char *)("fluid,comp"),             // rp_ptrip will be called as rp_ptrip(fluid,comp)
    (char *)("Returns triple point pressure [MPa] of the component number specified"),
                                        // description of rp_ptrip(fluid,comp)
    (LPCFUNCTION)rp_Ptrip,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
    