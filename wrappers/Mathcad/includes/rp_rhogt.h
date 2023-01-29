LRESULT rp_Rhogt(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t   )
{
    char herr[255];
    int kph = 2;   // looking for saturated vapor (dew point)
    ierr = 0;
    double psat,tsat,rhol,rhov,xliq[20],xvap[20];

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);
    
    if( t->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,1);
    else
        tsat = t->real;            // T in [K]

    SATTdll(&tsat,&x[0],&kph,&psat,&rhol,&rhov,&xliq[0],&xvap[0],&ierr,herr,errormessagelength);

    if (ierr > 0)
    {
        if ((ierr == 1)||(ierr == 9)||(ierr == 121)) 
            return MAKELRESULT(T_OUT_OF_RANGE,2); // Temperature too low | negative | > Tcrit
        else if (ierr == 8)
            return MAKELRESULT(BAD_COMPONENT,2); // x out of range
        else 
            return MAKELRESULT(UNCONVERGED, 2); // failed to converge
    }
    ret->real = rhov*wmm;   // Convert from mol/L to kg/m³

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_rhogt = 
{ 
    (char*)("rp_rhogt"),                // Name by which mathcad will recognize the function
    (char*)("fluid,t"),                 // rp_rhogt will be called as rp_rhogt(fluid,t)
    (char*)("Returns the saturation liquid density [kg/m³] given the temperature [K]"),
                                        // description of rp_rhogt(fluid,p)
    (LPCFUNCTION)rp_Rhogt,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 2 arguments
    { MC_STRING,
      COMPLEX_SCALAR }                  // argument is a complex scalar
};
    
    
