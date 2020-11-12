LRESULT rp_Muft(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      t)
{
    char herr[255];
    int kph = 1;   // looking for saturated liquid (bubble point)
    int ierr;
    double psat, tsat, rhol, rhov, xliq[20], xvap[20];
    double mu, cond;

    ierr = cSetup(fluid->str);
    if (ierr != 0)
        return MAKELRESULT(ierr, 1);

    if (t->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);
    else
        tsat = t->real;            // T in [K]

    SATTdll(&tsat, &x[0], &kph, &psat, &rhol, &rhov, &xliq[0], &xvap[0], &ierr, herr, errormessagelength);

    if (ierr != 0)
    {
        if ((ierr == 1) || (ierr == 9) || (ierr == 121))
            return MAKELRESULT(T_OUT_OF_RANGE, 2); // Temperature too low | negative | > Tcrit
        else if (ierr == 8)
            return MAKELRESULT(BAD_COMPONENT, 1); // x out of range
        else
            return MAKELRESULT(UNCONVERGED, 2); // failed to converge
    }

    TRNPRPdll(&tsat, &rhol, &x[0], &mu, &cond, &ierr, herr, errormessagelength);

    // check for errors and return MAKELRESULT(n,p)
    if ((ierr > 0) && (ierr != 51))
    {
        if ((ierr == 40) || (ierr == 49) || (ierr == 50))
            return MAKELRESULT(NO_TRANSPORT, 1);     // viscosity model not defined
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
    if (mu < 0)
        return MAKELRESULT(UNCONVERGED, 2);

    ret->real = mu;       // returned in µPa-s

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_muft =
{
    (char *)("rp_muft"),                // Name by which mathcad will recognize the function
    (char *)("fluid,t"),                // rp_muft will be called as rp_muft(fluid,t)
    (char *)("Returns the saturation liquid viscosity [µPa-s] given the saturation temperature [K]"),
                                        // description of rp_muft(fluid,t)
    (LPCFUNCTION)rp_Muft,               // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // String argument
    COMPLEX_SCALAR }                    // argument is a complex scalar
};
