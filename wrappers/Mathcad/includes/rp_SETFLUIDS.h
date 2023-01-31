//===================================================================================================================
// SETMIXTURE
//===================================================================================================================

LRESULT rp_SetMixture(
    LPCOMPLEXARRAY        retz,
    LPCMCSTRING         hFluid)
{
    using namespace std;
    char herr[errormessagelength] = "OK\0";     // Error String
    unsigned int cols = 1;                      // columns (1) in return array

    ierr = 0;                                   // Error Code

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);            // Throw error: Routine not Implemented (< REFPROP 10)

    string FluidString = upper(hFluid->str);               // Get fluid string parameter

    if ((FluidString == "") || (FluidString == " "))
        return MAKELRESULT(BAD_FILE, 1);

    char hMix[componentstringlength] = "";
    copy(FluidString.begin(), FluidString.end(), hMix);    // Copy string fluid_string into c_str hFld.
    hMix[FluidString.length()] = '\0';

    SETMIXTUREdll(                    // Call SETMIXTUREdll (fluid string required and composition returned)
        hMix,                         // hMix c-string
        &x[0],                        // the global composition array (returned value)
        &ierr,                        // error message
        componentstringlength);       // max length of component fluids string

    // Check for errors:
    if (ierr != 0) {
        return MAKELRESULT(BAD_FILE, 1);                // Error setting Flag
    }
    else
    {
        LastFluid = FluidString;

        // Get & set ncomp
        char hOut2[refpropcharlength] = "NCOMP";         // Copy string fluid_string into c_str hFld.
        char hIn[refpropcharlength] = "";
        double aval = 0, bval = 0, cval, qval;
        int ierr2;

        REFPROP1dll(                      // Call REFPROP1dll (no fluid string required and only one value returned)
            hIn,                          // hIn c-string    (empty)
            hOut2,                        // hOut c-string   "NCOMP"
            &iUnits,                      // Units code (set with GETENUMdll)
            &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions)
            &aval,                        // 1st state point value
            &bval,                        // 2nd state point value
            &x[0],                        // composition array [20]
            &cval,                        // return value
            &qval,                        // vapor quality (in 2-phase region)
            &ierr2,                        // error message
            herr,                         // error string
            refpropcharlength,            // max length of hIn string
            refpropcharlength,            // max length of hOut string
            errormessagelength);          // max length of error message string

        ncomp = (int)cval;                // get number of components.

        char htype[] = "EOS";

        LIMITSdll(htype, &x[0], &Tmin, &Tmax, &Dmax, &Pmax, lengthofreference);   // Get Tmin, Tmax, Dmzx, Pmax for this fluid/mixture
        WMOLdll(&x[0], &wmm);                                                     // Get molar mass [kg/kmol or g/mol]
        SATSPLNdll(&x[0], &ierr, herr, errormessagelength);                       // Call SATSPLN for new composition
        if (ierr > 0) return SATSPLN_FAILED;


        if (!MathcadArrayAllocate(retz,   // allocate memory for ret array
            (unsigned int)ncomp,          // number of rows, one for each component
            cols,                         // number of columns (1)
            TRUE,                         // allocate real part = TRUE
            FALSE))                       // allocate imaginary pat = FALSE
        {
            // if allocation is unsuccessful, return the appropriate error code
            return MAKELRESULT(INSUFFICIENT_MEMORY, 1);
        }

        // Loop over components and return the mole fractions in x
        for (int i = 0; i < ncomp; i++)                // Index from 0 to (ncomp - 1)
        {
            retz->hReal[0][i] = x[i];                  // Set each row of return array
        }

        if (isUserInterrupted())
        {
            MathcadArrayFree(retz);                     // if user has interrupted -- free the allocated memory
            return MAKELRESULT(INTERRUPTED, 1);        // and return with the appropriate error code
        }
    }

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_SETMIXTURE =
{
    (char *)("rp_SETMIXTURE"),          // Name by which Mathcad will recognize the function
    (char *)("hMix"),                   // rp_SETMIXTURE will be called as rp_SETMIXTURE(hMix)
                                        // Description of rp_SETMIXTURE(hMix)
    (char *)("Calls SETMIXTUREdll with hMix and returns composition array, z, if successful."),
    (LPCFUNCTION)rp_SetMixture,         // Pointer to the executable code
    COMPLEX_ARRAY,                      // The return type is a complex scalar
    1,                                  // The function takes 2 arguments
    { MC_STRING }                       //   - 1st argument is a scalar
};

//===================================================================================================================
// SETFLUIDS
//===================================================================================================================

LRESULT rp_SetFluids(
    LPCOMPLEXSCALAR        ret,
    LPCMCSTRING         hFluid)
{
    using namespace std;

    ierr = 0;                                   // Error Code

    if (vMajor < 10.0)      // shouldn't even get here if it's not version 10 or higher, but just in case...
        return MAKELRESULT(NOT_IMPLEMENTED, 1);                       // Throw error: Routine not Implemented (< REFPROP 10)

    string FluidString = upper(hFluid->str);                          // Get fluid string parameter

    if ((FluidString == "") || (FluidString == " "))
        return MAKELRESULT(BAD_FILE, 1);

    char hFld[componentstringlength] = "";
    copy(FluidString.begin(), FluidString.end(), hFld);    // Copy string fluid_string into c_str hFld.
    hFld[FluidString.length()] = '\0';

    SETFLUIDSdll(                     // Call REFPROP1dll (no fluid string required and only one value returned)
        hFld,                         // hIn c-string
        &ierr,                        // error message
        componentstringlength);        // max length of component fluids string
                                       // Check for errors:
    if (ierr != 0)
        return MAKELRESULT(UNKNOWN10, 2);                 // Error setting Flag
    else
    {
        LastFluid = FluidString;

        // Get & set ncomp
        char herr[errormessagelength] = "OK\0";          // Error String
        char hOut2[refpropcharlength] = "NCOMP";         // Copy string fluid_string into c_str hFld.
        char hIn[refpropcharlength] = "";
        double aval = 0, bval = 0, cval, qval;
        int ierr2;

        REFPROP1dll(                      // Call REFPROP1dll (no fluid string required and only one value returned)
            hIn,                          // hIn c-string    (empty)
            hOut2,                        // hOut c-string   "NCOMP"
            &iUnits,                      // Units code (set with GETENUMdll)
            &iMass,                       // iMass (0 = Mole Fractions, 1 = Mass Fractions)
            &aval,                        // 1st state point value
            &bval,                        // 2nd state point value
            &x[0],                        // composition array [20]
            &cval,                        // return value
            &qval,                        // vapor quality (in 2-phase region)
            &ierr2,                        // error message
            herr,                         // error string
            refpropcharlength,            // max length of hIn string
            refpropcharlength,            // max length of hOut string
            errormessagelength);          // max length of error message string

        ncomp = (int)cval;                // get number of components.

        x[0] = 1.0;                                   // initialize 1st mole fraction to 1
        for (int j = 1; j < 20; j++) { x[j] = 0.0; }  // initialize mole fractions to zero

        if (ncomp == 1)
        {
            char htype[] = "EOS";

            LIMITSdll(htype, &x[0], &Tmin, &Tmax, &Dmax, &Pmax, lengthofreference);   // Get Tmin, Tmax, Dmzx, Pmax for this fluid/mixture
            WMOLdll(&x[0], &wmm);                                                     // Get molar mass [kg/kmol or g/mol]
        }
    }

    ret->real = (double)ierr;             // get the returned value

    return 0;               // return 0 to indicate there was no error

}

FUNCTIONINFO    rp_SETFLUIDS =
{
    (char *)("rp_SETFLUIDS"),            // Name by which Mathcad will recognize the function
    (char *)("hFld"),                   // rp_SETFLUIDS will be called as rp_SETFLUIDS(hFld)
    (char *)("Calls SETFLUIDSdll with hFld and ierr (0) if successful."),
    // Description of rp_REFPROP1(hIn, hOut, a, b)
    (LPCFUNCTION)rp_SetFluids,          // Pointer to the executable code
    COMPLEX_SCALAR,                     // The return type is a complex scalar
    1,                                  // The function takes 2 arguments
    { MC_STRING }                       //   - 1st argument is a scalar
};
