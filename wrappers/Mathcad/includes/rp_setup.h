#include "mcadincl.h"


	extern void SETUP0(int *i, char *hfld, char *hfm, char *hrf, int *ierr, char *herr, 
		               unsigned int lenhfld, unsigned int lenhfm, unsigned int lenhrf,
					   unsigned int lenherr);

    LRESULT rp_Setup(   LPCOMPLEXSCALAR     res,
                                LPCMCSTRING    fluid   );

    FUNCTIONINFO    rp_setup = 
    { 
    "rp_setup",                         // Name by which mathcad will recognize the function
    "fluid",                            // rp_setup will be called as rp_setup(fluid)
    "Sets the fluid to be used by the Refprop functions",
										// description of rp_setup(fluid)
    (LPCFUNCTION)rp_Setup,              // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    1,                                  //  the function takes on 1 argument
    { STRING }                          // argument is a string
    };
    
    
    LRESULT rp_Setup(   LPCOMPLEXSCALAR     res,
                                LPCMCSTRING    fluid   )
    {
	
		char *pdest;
        unsigned int nstr;
		int count = 1;
		int ierr = 0;
		int lenherr = 3;
		char hfmix[256] = "fluids\\hmx.bnc\0";
		char hrf[4] = "DEF\0";
		char herr[256] = "Ok\0";
        // get length of fluid string and pass to SETUP
		nstr = (unsigned int)strlen(fluid->str);
		// Only pure fluids implemented at this time.  Look for fluids separated
		// by "|" symbols, or .mix files and flag error if found
        pdest = strstr( fluid->str, ".mix" );
        if ( pdest != NULL ) return MAKELRESULT(8,1);
		pdest = strstr( fluid->str, "|");
		if ( pdest != NULL ) return MAKELRESULT(8,1);
		// LATER: count number of '|' characters in string; i = count + 1;
		SETUP0(&count,fluid->str,hfmix,hrf,&ierr,herr,nstr,14,3,lenherr);
		if (ierr != 0 )
			return MAKELRESULT(7,1);
		else
		    res->real = 0.0;
        return 0;               // return 0 to indicate there was no error
            
    }           
    