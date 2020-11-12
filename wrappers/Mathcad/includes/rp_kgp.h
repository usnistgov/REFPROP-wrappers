LRESULT rp_Kgp(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p)
{
    char herr[255];
    int kph = 2;   // looking for saturated vapor (dew point)
    int ierr;
    double psat, tsat, rhol, rhov, xliq[20], xvap[20];
    double mu, cond;

    ierr = cSetup(fluid->str);
    if (ierr != 0)
        return MAKELRESULT(ierr, 1);

    if (p->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    else
        psat = p->real * 1000.0;       // P in [kPa]

    SATPdll(&psat, &x[0], &kph, &tsat, &rhol, &rhov, &xliq[0], &xvap[0], &ierr, herr, errormessagelength);

    if (ierr != 0)
    {
        if ((ierr == 2) || (ierr == 4) || (ierr == 12) || (ierr == 141))
            return MAKELRESULT(P_OUT_OF_RANGE, 2); // Pressure too low | negative | > Pcrit
        else if (ierr == 8)
            return MAKELRESULT(BAD_COMPONENT, 1); // x out of range
        else
            return MAKELRESULT(UNCONVERGED, 2); // failed to converge
    }

    TRNPRPdll(&tsat, &rhov, &x[0], &mu, &cond, &ierr, herr, errormessagelength);

    // check for errors and return MAKELRESULT(n,1)
    if (ierr > 0)
    {
        if ((ierr == 39) || (ierr == 40) || (ierr == 50))
            return MAKELRESULT(NO_TRANSPORT, 1);     // conductivity model not defined
        else if (ierr == 51)
            return MAKELRESULT(INFINITE_K, 3);
        else
            return MAKELRESULT(UNCONVERGED, 2);
    }
    else if (ierr < 0) {
        if ((ierr == -31) || (ierr == -33) || (ierr == -51) || (ierr == -53))
            return MAKELRESULT(T_OUT_OF_RANGE, 2);  // Temperature out of bounds
        else if ((ierr == -32) || (ierr == -52))
            return MAKELRESULT(D_OUT_OF_RANGE, 3);  // Pressure (density) out of bounds
        else if (ierr <= -58)
            return MAKELRESULT(UNCONVERGED, 2);     // did not converge
    }
    if (cond < 0)
        return MAKELRESULT(UNCONVERGED, 2);

    ret->real = cond;       // returned in W/m-s

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_kgp =
{
    (char *)("rp_kgp"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,t"),                // rp_kgp will be called as rp_kgp(fluid,t)
    (char *)("Returns the saturation vapor viscosity [W/m-s] given the saturation pressure [MPa]"),
                                        // description of rp_kgp(fluid,t)
    (LPCFUNCTION)rp_Kgp,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // String argument
    COMPLEX_SCALAR }                    // argument is a complex scalar
};
