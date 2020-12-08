    
    
    LRESULT rp_Rhotrip(   LPCOMPLEXSCALAR     ret,
                                LPCMCSTRING    fluid,  LPCCOMPLEXSCALAR    comp   )
    {
		char herr[256] = "Ok\0";
		char htyp[4] = "EOS";
		unsigned int lhtyp = 3;
        unsigned int lherr = 255;
		int ierr = 0;
		int icomp = 1;
		int kph = 1;
		//double wmm, ttrip, tnbpt, tc, pc, Dc, Zc, acf, dip, Rgas; 
		//double ptrip, dtrip; 
		double wmm,tmax,Dmax,pmax,rhol,rhov,xl,xv;
		double ttrip, ptrip, dtrip; 

		ierr = cSetup(fluid->str);
		if (ierr != 0 )
			return MAKELRESULT(ierr,1);
	

        if( comp->imag != 0.0 )
			return MAKELRESULT(1,2);
		else
			icomp = static_cast<int>(comp->real);
	
		if ((icomp > ncomp)||(icomp < 0))
			return MAKELRESULT(10,2);
		else
		{
			LIMITSdll(htyp,&x[0],&ttrip,&tmax,&Dmax,&pmax,lhtyp);
            double _Q = 0;
			SATTdll(&ttrip,&x[0],&kph,&ptrip,&rhol,&rhov,&xl,&xv,&ierr,herr,lherr);
			if (ierr != 0)
			{
				return MAKELRESULT(9,2);
			}
		}

		WMOLdll(&x[0],&wmm);
		
	    ret->real = dtrip*wmm;   // convert from mol/L to kg/m^3

        return 0;               // return 0 to indicate there was no error
            
    }           
FUNCTIONINFO    rp_rhotrip = 
{ 
    (char *)("rp_rhotrip"),                          // Name by which mathcad will recognize the function
    (char *)("fluid,comp"),                             // rp_rhotrip will be called as rp_rhotrip(fluid,comp)
    (char *)("Returns triple point density [kg/m^3] of the component number specified"),
										// description of rp_rhotrip(fluid,comp)
    (LPCFUNCTION)rp_Rhotrip,               // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    2,                                  // the function takes on 1 argument
    { MC_STRING,
	  COMPLEX_SCALAR }                  // argument is a complex scalar
};
    