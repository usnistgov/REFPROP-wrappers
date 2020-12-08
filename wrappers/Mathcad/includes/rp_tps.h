LRESULT rp_Tps(
    LPCOMPLEXSCALAR     ret,
    LPCMCSTRING       fluid,
    LPCCOMPLEXSCALAR      p,
    LPCCOMPLEXSCALAR      s   )
{
	char herr[255];
	double pval,sval,dval,tval;
	double pc,tc,Dc,sc;
	double tsat,rhol,rhov,xliq[20],xvap[20],sliq,svap;
	int ierr;
	int kph = 1;               // initialize to liquid

	ierr = cSetup(fluid->str);
	if (ierr != 0 )
		return MAKELRESULT(ierr,1);

    if( p->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,2);
	else
		pval = p->real * 1000.0;  // Convert from MPa to kPa

    if (pval > Pmax*(1 + extr)) return MAKELRESULT(P_OUT_OF_RANGE, 2);

    if( s->imag != 0.0 )
		return MAKELRESULT(MUST_BE_REAL,3);
	else
		sval = s->real * wmm;  // Convert from kJ/kg-K to J/mol-K

	// Are we above the critical pressure?
	CRITPdll(&x[0], &tc, &pc, &Dc, &ierr, herr, errormessagelength);
	if (ierr != 0)
		return MAKELRESULT(UNCONVERGED,2);

	if (pval < pc)
	{
		// Below the critical pressure
		SATPdll(&pval, &x[0], &kph, &tsat, &rhol, &rhov, &xliq[0], &xvap[0], &ierr, herr, errormessagelength);
		if (ierr != 0)
		{
			if ((ierr == 2)||(ierr == 4)||(ierr == 141)) 
				return MAKELRESULT(4,1); // Pressure too low | negative | > Pcrit
			else
				return MAKELRESULT(9,1); // failed to converge
		}
		ENTROdll(&tsat, &rhol, &x[0], &sliq); // Get Saturated Liquid Entropy at P
		ENTROdll(&tsat, &rhov, &x[0], &svap); // Get Saturated Vapor Entropy at P
		if ((sval >= sliq)&&(sval <= svap))
		{                                     // within saturation curve
			ret->real = tsat;                 // return the saturation temperature
			return 0;
		}
		if (sval < sliq)                      // below liquid entropy
		{                                     // initialize...
			kph = 1;                          //    liquid phase
			tval = tsat;                      //    saturation temp
			dval = rhol;                      //    saturation density
		}
		else                                  // above vapor entropy
		{                                     // initialize...
			kph = 2;                          //    vapor phase
			tval = tsat;                      //    saturation temp
			dval = rhov;                      //    saturation density
		}
		PSFL1dll(&pval, &sval, &x[0], &kph, &tval, &dval, &ierr, herr, errormessagelength);
		if (ierr != 0)
			return MAKELRESULT(UNCONVERGED,3);
    }
    else
    {                                          // above critical pressure
        kph = 2;                               // use vapor iterations
        tval = tc + 10.0;                      //   initialize t just above tc
        dval = Dc * 2.0;                       //   initialize rho to 2 X Dc
        ENTROdll(&tc, &Dc, &x[0], &sc);        //   check critical entropy
        if (sval < sc / 2.0) tval = tc * 0.8;  //   if very low entropy, reduce t guess

        PSFL1dll(&pval, &sval, &x[0], &kph, &tval, &dval, &ierr, herr, errormessagelength);
        if (ierr != 0)
            return MAKELRESULT(UNCONVERGED, 3);
    }

	ret->real = tval;       // Returned in degrees K

    return 0;               // return 0 to indicate there was no error
            
}

FUNCTIONINFO    rp_tps = 
{ 
    (char *)("rp_tps"),                 // Name by which mathcad will recognize the function
    (char *)("fluid,p,s"),              // rp_tps will be called as rp_tps(fluid,p,s)
    (char *)("Returns the temperature [K] given the pressure [MPa] and entropy [kJ/kg-K]"),
										// description of rp_tps(fluid,p,s)
    (LPCFUNCTION)rp_Tps,                // pointer to the executable code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    3,                                  // the function takes on 2 arguments
    { MC_STRING,                        // String argument
	  COMPLEX_SCALAR,
	  COMPLEX_SCALAR }                  // arguments are complex scalars
};
    
