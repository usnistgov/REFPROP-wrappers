LRESULT rp_PRtp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      p   )
{
    double tval,pval,Dval;
    double TminV, TmaxV, DmaxV, PmaxV;                          // Viscosity Limits
    double TminK, TmaxK, DmaxK, PmaxK;                          // TC Limits
    double ttrip, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas;
    double Dl, Dv, Q, U, H, S, Cv, Cp = 0.0, W;
    double mu, cond;                                            // Viscosity, TC values
    double xl[20], xv[20];
    ierr = 0;
    int icomp = 1, kph = 1, kguess = 0;
    char herr[255];
    char htypeV[] = "ETA";                                      // Viscosity equation set
    char htypeK[] = "TCX";                                      // Thermal Conductivity equation set

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if( t->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);
    else
        tval = t->real;

    if( p->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,3);
    else
        pval = p->real * 1000.0;   // Convert from MPa to kPa for REFPROP inputs

    //======================== Check EOS, Viscosity, and Thermal Conductivity Limits ==================================
    LIMITSdll(htypeV, &x[0], &TminV, &TmaxV, &DmaxV, &PmaxV, lengthofreference);
    LIMITSdll(htypeK, &x[0], &TminK, &TmaxK, &DmaxK, &PmaxK, lengthofreference);

    if ((tval > std::min(std::min(Tmax, TmaxV), TmaxK)*(1.0 + 0.5*extr)) ||   // if above Tmax for EOS or ETA or TCX, or
        (tval < std::max(std::max(Tmin, TminV), TminK)))                      //    below Tmin for EOS or ETA or TCX,
        return MAKELRESULT(T_OUT_OF_RANGE, 2);                                //    throw error.

    if (pval > std::min(std::min(Pmax, PmaxV), PmaxK)*(1.0 + extr))           // if above Pmax for EOS or ETA or TCX,
        return MAKELRESULT(P_OUT_OF_RANGE, 3);                                //    throw error.

    // Get critical pressure
    if (ncomp > 1)
    {
        CRITPdll(&x[0], &tc, &pc, &Dc, &ierr, herr, errormessagelength);
        if (ierr > 0)
        {
            return MAKELRESULT(UNCONVERGED, 2);
        }
    }
    else
    {
        INFOdll(&icomp, &wmm, &ttrip, &tnbpt, &tc, &pc, &Dc, &Zc, &acf, &dip, &Rgas);
    }

    // If above critical pressure (Liquid) and/or critical temperature (Vapor) use TPRHO instead of TPFLSH
    if (tval > tc) kph = 2;
    if (pval > pc) kph = 1;
    if ((pval > pc) || (tval > tc))
    {
        // Get single-phase density
        TPRHOdll(&tval, &pval, &x[0], &kph, &kguess, &Dval, &ierr, herr, errormessagelength);
        // Have density, now call THERM to get Enthalpy
        if (ierr <=0) CVCPdll(&tval, &Dval, &x[0], &Cv, &Cp);   // Calling CVCPdll should be faster than THERMdll
    }
    else
    {
        TPFLSHdll(&tval, &pval, &x[0], &Dval,   // [K], [kPa], [mol/L]
            &Dl, &Dv, &xl[0], &xv[0],           // Saturation terms
            &Q, &U, &H, &S, &Cv, &Cp, &W,       // Thermo properties
            &ierr, herr, errormessagelength);   // error code and string
    }
    // Check for errors
    if (ierr > 0) {  //                                                     RP10    
        if ((ierr == 1) || (ierr == 5) || (ierr == 9) || (ierr == 13) || (ierr == 17))
            return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
        else if ((ierr == 4) || (ierr == 12))
            return MAKELRESULT(P_OUT_OF_RANGE, 1);  // Pressure out of bounds
        else if (ierr == 8)
            return MAKELRESULT(X_SUM_NONUNITY, 1);  // component and/or sum < 0 or > 1
        else
            return MAKELRESULT(UNCONVERGED, 2);     // one of many convergence errors
    }

    TRNPRPdll(&tval, &Dval, &x[0], &mu, &cond, &ierr, herr, errormessagelength);

    // check for errors and handle by returning MAKELRESULT(n,p)
    // Error codes for TRNPRPdll changed radically in REFPROP 10
    if (vMajor < 10)  // Can only be REFPROP 9.1.1 or 10+ if we are here
    {
        // REFPROP 9.1.1 Error Flags
        if (ierr > 0)
        {
            if ((ierr == 40) || (ierr == 49) || (ierr == 50))
                return MAKELRESULT(NO_TRANSPORT, 1);     // viscosity model not defined
            else if (ierr == 51)
                return MAKELRESULT(INFINITE_K, 3);
            else
                return MAKELRESULT(UNCONVERGED, 2);
        }
        else if (ierr < 0) {
            if ((ierr == -41) || (ierr == -43) || (ierr == -51) || (ierr == -53))
                return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
            else if ((ierr == -42) || (ierr == -52))
                return MAKELRESULT(D_OUT_OF_RANGE, 3);  // Pressure (density) out of bounds
            else if (ierr <= -58)
                return MAKELRESULT(UNCONVERGED, 2);     // did not converge
        }
    }
    else // Assume REFPROP 10 or greater
    {
        // REFPROP 10 error flags here
        if (ierr > 0)
        {
            if ((ierr == 502) || (ierr == 540) || (ierr == 541) || (ierr == 542) || (ierr == 543))
                return MAKELRESULT(NO_TRANSPORT, 1);     // viscosity model not defined
            else if ((ierr == 73) || (ierr == 74))
                return MAKELRESULT(UNKNOWN, 2);  // inputs out of bounds
            else if (ierr == 561)
                return MAKELRESULT(UNCONVERGED, 2);     // Erroneous value returned for ETA or TCX
            else
                return MAKELRESULT(UNKNOWN, 2);  // One or more inputs to ETA or TCX out of bounds
        }
        else if (ierr < 0) return MAKELRESULT(UNCONVERGED, 2);  // Either ETA or TCX did not converge
    }
    if ((mu < 0) || (cond < 0))
        return MAKELRESULT(UNCONVERGED, 2);

    // µ returned in [µPa-s] = [mg / m-s]
    // k returned in [W/m-K] = [J/m-s-K]
    // Cp returned in [J/mol-K]
    // wmm returned in [g/mol]
    ret->real = mu * Cp / wmm / cond / 1000;   // Conversion: (µPa-s) * (J/mol-K) / (g/mol) / (W/m*K) / (mg/g) = 
                                               //          (mg/m-s) * (J/mol-K) * (mol/g) * (m-s-K/J) * (g/mg) =
                                               //          [dimensionless]

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_prtp = 
{ 
    (char *)("rp_prtp"),                 // Name by which Mathcad will recognize the function
    (char *)("fluid,t,p"),              // rp_prtp will be called as rp_prtp(fluid,t,p)
    (char *)("Returns the Prandtl Number [-] given the temperature [K] and pressure [MPa]"),
                                        // description of rp_prtp(fluid,t,p)
    (LPCFUNCTION)rp_PRtp,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 3 arguments
    { MC_STRING,                        // String argument
      COMPLEX_SCALAR,
      COMPLEX_SCALAR }                  // arguments are complex scalars
};
    
    
    