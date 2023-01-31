LRESULT rp_GETX(
    LPCOMPLEXARRAY     ret,
    LPCMCSTRING       fluid  )
{
    ierr = 0;
    int cols = 1;

    std::string fl = fluid->str;

    if ((fl != "") && (fl != " "))       // If not an empty fluid string, load fluid
    {
        ierr = cSetup(fluid->str);       // Note: Fluid will not load if same as previously loaded fluid.
        if (ierr > 0)
            return MAKELRESULT(ierr,1);
    }

    // allocate memory for x component values
    if (!MathcadArrayAllocate(ret,                 // allocate memory for ret array
        ncomp,                                     // number of rows, one for each component
        cols,                                      // number of columns (1)
        TRUE,                                      // allocate real part = TRUE
        FALSE))                                    // allocate imaginary pat = FALSE
    {
        // if allocation is unsuccessful, return the appropriate error code
        return MAKELRESULT(INSUFFICIENT_MEMORY, 1);
    }

    // Loop over components and return the mole fractions in x
    for (int i = 0; i < ncomp; i++)                // Index from 0 to (ncomp - 1)
    {
        ret->hReal[0][i] = x[i];                   // Set each row of return array
    }

    if (isUserInterrupted())
    {
        MathcadArrayFree(ret);                     // if user has interrupted -- free the allocated memory
        return MAKELRESULT(INTERRUPTED, 1);        // and return with the appropriate error code
    }

    return 0;               // return 0 to indicate there was no error

}

    FUNCTIONINFO    rp_getx = 
{ 
    (char *)("rp_getx"),                // Name by which Mathcad will recognize the function
    (char *)("fluid"),                  // rp_getx will be called as rp_getx(fluid)
    (char *)("Returns mole fraction array for the loaded mixture."),
                                        // description of rp_getx(fluid)
    (LPCFUNCTION)rp_GETX,               // pointer to the executable code
    COMPLEX_ARRAY,                      // the return type is a complex scalar
    1,                                  // the function takes only 1 argument
    { MC_STRING }                       // argument is a string
};
