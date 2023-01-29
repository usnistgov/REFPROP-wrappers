LRESULT rp_Tph(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p,
    LPCCOMPLEXSCALAR      h   )
{
    char herr[255];
    double pval,hval,dval,tval;
    double pc,tc,Dc,hc;
    double tsat,rhol,rhov,xliq[20],xvap[20],hliq,hvap;
    ierr = 0;
    int kph = 1;             // initialize to Liquid

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( p->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        pval = p->real * 1000.0;  // Convert from MPa to kPa

    if (pval > Pmax*(1 + extr)) return MAKELRESULT(P_OUT_OF_RANGE, 2);

    if( h->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,3);
    else
        hval = h->real * wmm;     // Convert from kJ/kg to J/mol

    // Are we above the critical pressure?
    CRITPdll(&x[0], &tc, &pc, &Dc, &ierr, herr, errormessagelength);
    if (ierr > 0)
        return MAKELRESULT(UNCONVERGED,1);

    if (pval < pc)
    {
        // Below the critical pressure
        SATPdll(&pval, &x[0], &kph, &tsat, &rhol, &rhov, &xliq[0], &xvap[0], &ierr, herr, errormessagelength);
        if (ierr > 0)
        {
            if ((ierr == 2)||(ierr == 4)||(ierr == 141)) 
                return MAKELRESULT(P_OUT_OF_RANGE,2); // Pressure too low | negative | > Pcrit
            else
                return MAKELRESULT(UNCONVERGED,2); // failed to converge
        }
        ENTHALdll(&tsat, &rhol, &x[0], &hliq);         // Get Saturated Liquid Enthalpy at P
        ENTHALdll(&tsat, &rhov, &x[0], &hvap);         // Get Saturated Vapor Enthalpy at P
        if ((hval >= hliq) && (hval <= hvap))
        {                                    // within saturation curve
            ret->real = tsat;                // return the saturation temperature
            return 0;
        }
        if (hval < hliq)                     // below liquid enthalpy
        {                                    // initialize...
            kph = 1;                         //    liquid phase
            tval = tsat;                     //    saturation temp
            dval = rhol;                     //    saturation density
        }
        else                                 // above vapor enthalpy
        {                                    // initialize...
            kph = 2;                         //    vapor phase
            tval = tsat;                     //    saturation temp
            dval = rhov;                     //    saturation density
        }
        PHFL1dll(&pval, &hval, &x[0], &kph, &tval, &dval, &ierr, herr, errormessagelength);
        if (ierr > 0)
            return MAKELRESULT(UNCONVERGED,2);
    }
    else {                                      // above critical pressure
        kph = 2;                                // use vapor iterations
        tval = tc + 10.0;                       //   initialize t just above tc
        dval = Dc * 2.0;                        //   initialize rho to 2 X Dc
        ENTHALdll(&tc, &Dc, x, &hc);            //   check critical enthalpy
        if (hval < hc / 2.0) tval = tc * 0.8;   //   if very low enthalpy, reduce t guess

        PHFL1dll(&pval, &hval, &x[0], &kph, &tval, &dval, &ierr, herr, errormessagelength);
        if (ierr > 0)
            return MAKELRESULT(UNCONVERGED, 3);
    }

    ret->real = tval;       // Returned in degrees K

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_tph = 
{ 
    (char *)("rp_tph"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,p,h"),              // rp_tph will be called as rp_tph(fluid,p,h)
    (char *)("Returns the temperature [K] given the pressure [MPa] and enthalpy [kJ/kg]"),
                                        // description of rp_tph(fluid,p,h)
    (LPCFUNCTION)rp_Tph,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes 3 arguments
    { MC_STRING,                        // String argument
      COMPLEX_SCALAR,
      COMPLEX_SCALAR }                  // arguments are complex scalars
};
    
    
    