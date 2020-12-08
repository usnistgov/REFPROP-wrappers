LRESULT rp_Extrap(      
    LPMCSTRING          hstr,                        
    LPCCOMPLEXSCALAR    flag  )
{
	char *pdest;
	int iset;
	char hset[23]    = "Extrapolation Enabled";
	char hnotset[23] = "Extrapolation Disabled";

	iset = static_cast<int>(flag->real);

    if ((iset == 0) || (iset = 1))
        extr = iset;
    else
        return MAKELRESULT(INVALID_FLAG, 1);

	// allocate memory for return string
	pdest = MathcadAllocate(23);
	if (pdest == NULL )
		return MAKELRESULT(INSUFFICIENT_MEMORY,1);  // insufficient memory
	else
	{
		if (extr == 1)
			strncpy(pdest,hset,23);
		else
			strncpy(pdest,hnotset,23);
		hstr->str = pdest;
	}
    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_extrap = 
{ 
    (char *)("rp_extrap"),              // Name by which Mathcad will recognize the function
    (char *)("i"),                      // rp_extrap will be called as rp_extrap(i)
    (char *)("Sets a flag that allows extrapolation to 1.5*Tmax and returns a verification string."),
                                        // description of rp_extrap(i)
    (LPCFUNCTION)rp_Extrap,             // pointer to the executable code
    MC_STRING,                          // the return type is a MC_STRING
    1,                                  // the function takes on 1 argument
    { COMPLEX_SCALAR }                  // arguments are complex scalars
};
    