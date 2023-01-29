LRESULT rp_Rhoth(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t,
    LPCCOMPLEXSCALAR      h,
    LPCCOMPLEXSCALAR      r   )
{
    char herr[errormessagelength];
    double pval, hval, Dval, tval, qval;
    double rhol, rhov, xliq[20], xvap[20];
    double U, S, Cv, Cp, W;
    double tc, pc, Dc;      //  , hvap, psat;   // no longer needed
    ierr = 0;
    int kr;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr,1);

    if (t->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    else
        tval = t->real;  // Temperature in K

    if (tval > Tmax*(1 + 0.5*extr)) return MAKELRESULT(T_OUT_OF_RANGE, 2);

    if (h->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 3);
    else
        hval = h->real * wmm;  // Convert from kJ/kg-K to J/mol-K

    if( r->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,4);
    else
    {
        kr = static_cast<int>(r->real);  // Convert to integer
        // root must be 1 or 2...
        if ((kr<1)||(kr>2)) return MAKELRESULT(INVALID_FLAG,4);
    }

    // Upper root not supported in REFPROP 9.1 for T < Tc and H > Hvap.  Find Tc and check if kr == 2
    /* Fixed in REFPROP 10 *** Comment this section out ****
    if (kr == 2)
    {
        CRITPdll(&x[0], &tc, &pc, &Dc, &ierr, herr, errormessagelength);
        if (ierr > 0)
            return MAKELRESULT(UNCONVERGED, 1);
        if (tval <= tc)
        {
            SATTdll(&tval, &x[0], &kph, &psat, &rhol, &rhov, &xliq[0], &xvap[0], &ierr, herr, errormessagelength);
            if (ierr > 0) {
                if ((ierr == 1) || (ierr == 9) || (ierr == 121))
                    return MAKELRESULT(T_OUT_OF_RANGE, 2); // Temperature too low | negative | > Tcrit
                else if (ierr == 8)
                    return MAKELRESULT(BAD_COMPONENT, 2); // x out of range
                else
                    return MAKELRESULT(UNCONVERGED, 2); // failed to converge
            }
            ENTHALdll(&tval, &rhov, &x[0], &hvap);
            if (hval >= hvap)
                return MAKELRESULT(NO_UPPER_ROOT, 4);
        }
    }
    */

    // REFPROP 10 Issue: Single-phase root confusion when T = Tc exactly
    // As a workaround, adjust tval slightly if Tc passed in
    CRITPdll(&x[0], &tc, &pc, &Dc, &ierr, herr, errormessagelength);
    if (ierr > 0)
        return MAKELRESULT(UNCONVERGED, 1);
    if (tval == tc) tval = tval + 0.0001;

    pval = 0.0;

    THFLSHdll(&tval, &hval, &x[0], &kr, &pval, &Dval, &rhol, &rhov, &xliq[0], &xvap[0], &qval, &U, &S, &Cv, &Cp, &W, &ierr, herr, errormessagelength);

    if (ierr > 0) {
        if (ierr == 250)
            return MAKELRESULT(S_OUT_OF_RANGE, 1);
        else if (ierr <= 12)
            return MAKELRESULT(P_OUT_OF_RANGE, 2);
        else if (ierr == 249)
            return MAKELRESULT(H_OUT_OF_RANGE, 3);
        else
            return MAKELRESULT(UNCONVERGED, 1);
    }

    // Otherwise...
    ret->real = Dval * wmm;   // Convert from mol/L to kg/m³

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_rhoth = 
{ 
    (char *)("rp_rhoth"),							// Name by which mathcad will recognize the function
    (char *)("fluid,t,h,r"),						// rp_rhoth will be called as rp_rhoth(fluid,t,h,r)
    (char *)("Returns the density [kg/m³] given the temperature [K] and enthalpy [kJ/kg]"),
                                        // description of rp_rhoth(fluid,t,h,r)
    (LPCFUNCTION)rp_Rhoth,				// pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    4,                                  // the function takes on 4 arguments
    { MC_STRING,                        // String argument
      COMPLEX_SCALAR,
      COMPLEX_SCALAR,
      COMPLEX_SCALAR }                  // arguments are complex scalars
};