    
LRESULT rp_Pmax(
    LPCOMPLEXSCALAR      ret,
    LPCMCSTRING        fluid   )
{
	int ierr = 0;

	ierr = cSetup(fluid->str);
	if (ierr > 0)
		return MAKELRESULT(ierr,1);

	ret->real = Pmax/1000.0;  // convert from kPa to MPa

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_pmax = 
{ 
    (char *)("rp_pmax"),                // Name by which mathcad will recognize the function
    (char *)("fluid"),                  // rp_pmax will be called as rp_pmax(fluid)
    (char *)("Returns maximum pressure [MPa] for the fluid/mixture specified"),
										// description of rp_pmax(fluid)
    (LPCFUNCTION)rp_Pmax,               // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    1,                                  // the function takes on 1 argument
    { MC_STRING }                       // argument is an MC_STRING
};
    