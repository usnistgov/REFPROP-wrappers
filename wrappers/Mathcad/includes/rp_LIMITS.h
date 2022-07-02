LRESULT rp_LIMITS(
    LPCOMPLEXARRAY      ret,
    LPCMCSTRING       fluid,
    LPCMCSTRING       htype)
{
    double Tminx, Tmaxx, Dmaxx, Pmaxx;                          // Limits variables
    int ierr = 0;
    unsigned int rows = 4, cols = 1;

    ierr = cSetup(fluid->str);
    if (ierr > 0)
        return MAKELRESULT(ierr, 1);

    std::string strType = upper(htype->str);  // Extract to std::string for each compare

    if ((strType != "EOS") && (strType != "ETA") && (strType != "TCX"))
        return MAKELRESULT(INVALID_MODEL, 2);

    // Get Limits for requested model
    LIMITSdll(&strType[0], &x[0], &Tminx, &Tmaxx, &Dmaxx, &Pmaxx, lengthofreference);

    //allocate memory for the Limit values
    if (!MathcadArrayAllocate(
        ret,
        rows,
        cols,
        TRUE,
        FALSE))
    {
        // if allocation is not successful, return with the appropriate error code
        return MAKELRESULT(INSUFFICIENT_MEMORY, 1);
    }

    if (isUserInterrupted())
    {
        // if use has interrupted -- free the allocated memory
        MathcadArrayFree(ret);
        // and return with an appropriate error code
        return MAKELRESULT(INTERRUPTED, 1);
    }

    // Assign return array elements
	ret->hReal[0][0] = Tminx;         // Returned in K
    ret->hReal[0][1] = Tmaxx;         // Returned in K
    ret->hReal[0][2] = Dmaxx * wmm;   // Convert from mol/L to kg/m³
    ret->hReal[0][3] = Pmaxx / 1000;  // Convert from kPa to MPa

    return 0;               // return 0 to indicate there was no error
            
}           

FUNCTIONINFO    rp_limits = 
{ 
    (char *)("rp_limits"),              // Name by which mathcad will recognize the function
    (char *)("fluid,htype"),            // rp_limits will be called as rp_limits(fluid,htype)
    (char *)("Returns limits Tmin [K], Tmax [K], Dmax [kg/m°], Pmax [MPa] of the fluid model in htype as an array."),
										// description of rp_limits(fluid,htype)
    (LPCFUNCTION)rp_LIMITS,             // pointer to the executable code
    COMPLEX_ARRAY,                      // the return type is a 1x4 complex array
    2,                                  // the function takes 2 arguments
    { MC_STRING,                        // String argument
	  MC_STRING }                       // String argument
};
    