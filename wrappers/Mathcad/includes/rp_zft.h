LRESULT rp_Zft(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t   )
{
    char herr[255];
    int kph = 1;       //  kph = 1 (saturated liquid conc.); kph = 2 (saturated vapor conc.)  
    int ierr;
    int icomp = 1;
    double psat,tsat,rhol,rhov,xliq[20],xvap[20];
    double tc, pc, Dc, Rgas, ttrip, tnbpt, Zc, acf, dip;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);
    
    if( t->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        tsat = t->real;

    SATTdll(&tsat,&x[0],&kph,&psat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,errormessagelength);

    if (ierr > 0)
    {
        if ((ierr == 1)||(ierr == 9)||(ierr == 121)) 
            return MAKELRESULT(T_OUT_OF_RANGE,2); // Temperature too low | negative | > Tcrit
        else
            return MAKELRESULT(UNCONVERGED,2); // failed to converge
    }

    // Update sat liquid pressure if mixture
    PRESSdll(&tsat, &rhol, &x[0], &psat);  // [K], [mol/L], [kPa]

    // Get Rgas
    if (ncomp > 1)
    {
        RMIX2dll(&x[0], &Rgas);              // mixture gas constant [J/mol-K]
    }
    else
    {
        INFOdll(&icomp, &wmm, &ttrip, &tnbpt, &tc, &pc, &Dc, &Zc, &acf, &dip, &Rgas);
    }

    ret->real = psat / (Rgas * tsat * rhol);   // calculate compressibility factor

    return 0;               // return 0 to indicate there was no error
            
}    

FUNCTIONINFO    rp_zft = 
{ 
    (char *)("rp_zft"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,t"),                // rp_zft will be called as rp_zft(fluid,t)
    (char *)("Returns the saturation liquid compressibility factor [-] given the temperature [K]"),
                                        // description of rp_zft(fluid,t)
    (LPCFUNCTION)rp_Zft,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // argument is a MC_STRING
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
