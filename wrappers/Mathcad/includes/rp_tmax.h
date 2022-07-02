LRESULT rp_Tmax(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid   )
{
	int ierr = 0;

	ierr = cSetup(fluid->str);
	if (ierr > 0)
		return MAKELRESULT(ierr,1);

    ret->real = Tmax;

    return 0;               // return 0 to indicate there was no error
            
}           

    FUNCTIONINFO    rp_tmax = 
{ 
    (char *)("rp_tmax"),               // Name by which Mathcad will recognize the function
    (char *)("fluid"),                  // rp_tmax will be called as rp_tmax(fluid)
    (char *)("Returns maximum temperature [K] of the fluid/mixture specified"),
										// description of rp_tmax(fluid)
    (LPCFUNCTION)rp_Tmax,              // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    1,                                  // the function takes on 1 argument
    { MC_STRING }                       // argument is an MC_STRING
};
