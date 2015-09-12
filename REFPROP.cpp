#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>
#include <cstring>
#include <cmath>

#include "REFPROP.h"

#define REFPROP_IMPLEMENTATION
#include "REFPROP_lib.h"
#undef REFPROP_IMPLEMENTATION

namespace REFPROP {

double F2K(double T_F){
	// A simple function for interface testing purposes - convert from Fahrenheit to Kelvin
    return 5.0/9.0*(T_F-32.0)+273.15;
}

// Code from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}
// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

/** Utility function to convert a single input value from base-SI units to REFPROP's modified SI units
 */
double SI_to_RPSI(parameters para1, double val){
	double SI_to_RPSI_conversion_factor;
	switch (para1){
		#define X(EnumeratedValue, String, SI_TO_RPSI, Description) case EnumeratedValue: SI_to_RPSI_conversion_factor = SI_TO_RPSI; break;
			PARAMETERS
		#undef X
		default:
			SI_to_RPSI_conversion_factor = 1e99;
	}
	return val*SI_to_RPSI_conversion_factor;
}

/** Utility function to convert a single input value from REFPROP's modified SI units to base-SI units
 */
double RPSI_to_SI(parameters para1, double val){
	double SI_to_RPSI_conversion_factor;
	switch (para1){
		#define X(EnumeratedValue, String, SI_TO_RPSI, Description) case EnumeratedValue: SI_to_RPSI_conversion_factor = SI_TO_RPSI; break;
			PARAMETERS
		#undef X
		default:
			SI_to_RPSI_conversion_factor = 1e99;
	}
	return val/SI_to_RPSI_conversion_factor;
}


/** Utility function to convert a single input value from REFPROP mass-based units to REFPROP mole-based units
 */
double RPmass_to_RPmolar(parameters input, double val, double molar_mass)
{
    switch (input){
        case T_PARA:
        case P_PARA:
            return val;
        case Dmass_PARA:
            return val/molar_mass; // [kg/m^3 -> mol/L]
        case Hmass_PARA:
        case Smass_PARA:
        case Umass_PARA:
            return val*(molar_mass/1000);// [J/kg -> J/mol]
    }
}
/** Utility function to convert a single input value from base-SI mass-based units to REFPROP mole-based units
 */
double SImass_to_RPmolar(parameters input, double val, double molar_mass)
{
    switch (input){
        case T_PARA:
            return val; // [no conversion]
        case P_PARA:
            return val/1000; // [Pa -> kPa]
        case Dmass_PARA:
            return val/molar_mass; // [kg/m^3 -> mol/L]
        case Hmass_PARA:
        case Smass_PARA:
        case Umass_PARA:
            return val*(molar_mass/1000); // [J/kg -> J/mol]
    }
}

/** Utility function to split flash input into two separate parameters
 */
void split_input_pair(input_pairs pair, parameters &para1, parameters &para2)
{
    switch (pair){
		#define X(Enum, Para1, Para2, String) case Enum: para1 = Para1; para2 = Para2; break;
			INPUT_PAIRS
		#undef X
		default:
			std::cout << __LINE__ << ": invalid\n";
    }
}

/** Utility function to construct flash input from two separate parameters and convert 
 *  input variables to REFPROP's modified SI system of units
 */
input_pairs build_input_pair_RPSI_from_SI(parameters para1, const double value1, parameters para2, const double value2, double &out1, double &out2)
{
	out1 = value1; out2 = value2;
	bool swap = false;
	input_pairs pair = INPUT_PAIR_INVALID;
	#define X(Enum, Para1, Para2, String) \
		if (para1 == Para1 && para2 == Para2){ swap = false; pair = Enum; } \
		if (para1 == Para2 && para2 == Para1){ swap = true; pair = Enum; }

		INPUT_PAIRS
	#undef X
	if (pair == INPUT_PAIR_INVALID){
		std::cout << __LINE__ << ": invalid\n";
	}
	else{
		// Convert units to RPSI system
		if (swap){
			std::swap(out1, out2);
			std::swap(para1, para2);
		}
		out1 = SI_to_RPSI(para1, out1);
		out2 = SI_to_RPSI(para2, out2);
	}

	return pair;
}
/** Utility function to construct flash input from two separate parameters 
 *  with input variables already in REFPROP's modified SI system of units
 */
input_pairs build_input_pair(parameters para1, const double value1, parameters para2, const double value2, double &out1, double &out2)
{
	out1 = value1; out2 = value2;
	bool swap = false;
	input_pairs pair = INPUT_PAIR_INVALID;
	#define X(Enum, Para1, Para2, String) \
		if (para1 == Para1 && para2 == Para2){ swap = false; pair = Enum; } \
		if (para1 == Para2 && para2 == Para1){ swap = true; pair = Enum; }

		INPUT_PAIRS
	#undef X
	if (pair == INPUT_PAIR_INVALID){
		std::cout << __LINE__ << ": invalid\n";
	}
	else{
		if (swap){ std::swap(out1, out2); std::swap(para1, para2); }
	}
	return pair;
}

parameters name_to_parameter(const std::string &name)
{
	#define X(EnumeratedValue, String, SI_TO_RPSI, Description) if (String == name){ return EnumeratedValue; }
		PARAMETERS
	#undef X
	std::cout << __LINE__ << ": invalid\n";
	return INVALID_PARAMETER;
}

/** Utility function to convert from REFPROP mass-based units to REFPROP molar-based units
 * @param value1 The first input parameter in REFPROP mass-based units (kg/m^3, kPa, etc.)
 * @param value2 The second input parameter in REFPROP mass-based units (kg/m^3, kPa, etc.)
 * @param molar_mass The molar mass in g/mol
 * @param output1 The first input parameter in REFPROP molar units (mol/L, kPa, etc.)
 * @param output2 The second output parameter in REFPROP molar units (mol/L, kPa, etc.)
 */
void mass_inputs_to_molar_inputs(input_pairs pair, double value1, double value2, double molar_mass, double &output1, double &output2)
{
    if (molar_mass > 1000 || molar_mass < 1){
        std::cout << "molar mass is in the wrong unit system" << std::endl;
    }
    parameters para1, para2;
    split_input_pair(pair, para1, para2);
    output1 = RPmass_to_RPmolar(para1, value1, molar_mass); 
    output2 = RPmass_to_RPmolar(para2, value2, molar_mass); 
}

SetupReturnStruct Setup(long *nc, const std::string &fluids, const std::string &hmx_bnc, const std::string &reference_state)
{
    std::string errstr;
    bool loaded_REFPROP = load_REFPROP(errstr);
    SetupReturnStruct err;
    char herr_buffer[255], hmx_bnc_str[255], reference_state_str[4], fluids_str[10000];
    strcpy(fluids_str, fluids.c_str());
    strcpy(hmx_bnc_str, hmx_bnc.c_str());
    strcpy(reference_state_str, reference_state.c_str());
    SETUPdll(nc, fluids_str, hmx_bnc_str, reference_state_str, &(err.ierr), herr_buffer, refpropcharlength, filepathlength, lengthofreference, errormessagelength);
    std::string buffer(herr_buffer);
	buffer += "\0"; // Null terminate
    err.herr = rtrim(buffer) ;
    /*
    char hfij[6][8];
    long icomp = 1, jcomp = 2;
    char hmodij[3], hfmix[255], hbinp[255], hmxrul[255];
    std::vector<double> fij(6);
    GETKTV(&icomp,&jcomp,hmodij, &(fij[0]),hfmix,hfij,hbinp,hmxrul);
    char bbuffer[1000];
    sprintf(bbuffer, "%g %g %g %g %g %s %s %s", fij[0], fij[1], fij[2], fij[3], fij[4], hfmix, hbinp, hmxrul);
    std::string sbbuffer(bbuffer);
    err.herr += rtrim(sbbuffer)+ "\0";
    */
    return err;
}
SetupReturnStruct Setup(const std::string &fluids)
{
    std::string errstr;
    bool loaded_REFPROP = load_REFPROP(errstr);
    SetupReturnStruct err;
    char herr_buffer[errormessagelength], hmx_bnc[255] = "HMX.BNC", reference_state[4] = "DEF", fluids_str[10000];
    strcpy(fluids_str, fluids.c_str());
    size_t bar_count = std::count(fluids.begin(), fluids.end(), '|');
    long nc = static_cast<long>(bar_count) + 1;
    SETUPdll(&nc, fluids_str, hmx_bnc, reference_state, &(err.ierr), herr_buffer, refpropcharlength, filepathlength, lengthofreference, errormessagelength);
    std::string buffer(herr_buffer, herr_buffer + errormessagelength);
	buffer += "\0"; // Null terminate
    err.herr = rtrim(buffer);
    return err;
}
SaturationSplineReturnStruct BuildSaturationSpline(const std::vector<double> &z){
    SaturationSplineReturnStruct out;
    char herr_buffer[255];
    // Construct the saturation spline
    SATSPLNdll(const_cast<double*>(&z[0]), &(out.ierr), herr_buffer, errormessagelength);
    std::string buffer(herr_buffer);
	buffer += "\0"; // Null terminate
    out.herr = rtrim(buffer);
    strcpy(herr_buffer, "");
    
    /*
    subroutine SPLNVAL (isp,iderv,a,f,ierr,herr)
    c
    c  calculates the function value of a spline
    c
    c  inputs:
    c      isp--indicator for which spline to use (1-nc: composition,
    c           nc+1: temperature, nc+2: pressure, nc+3: density,
    c           nc+4: enthalpy, nc+5: entropy)
    c    iderv--values of -1 and -2 return lower and upper root values,
    c           value of 0 returns spline function, value of 1 returns
    c           derivative of spline function with respect to density,
    c           and a value of 2 returns the 2nd derivative
    c        a--root value
    */
    long N = 1000;
    out.T.resize(N);
    out.p.resize(N);
    out.rhox.resize(N);
    out.rhoy.resize(N);
    
    // Get density limit values from the spline
    long isp = z.size() + 3, iderv, ierr2;
    double rho_dummy = 0;
    iderv = -1;
    SPLNVALdll(&isp,&iderv,&rho_dummy,&(out.rhoy[0]),&ierr2,herr_buffer, errormessagelength);
    iderv = -2;
    SPLNVALdll(&isp,&iderv,&rho_dummy,&(out.rhoy[N-1]),&ierr2,herr_buffer, errormessagelength);
    iderv = 0; // Get the actual spline value
    // Collect each of the state variable vectors in turn
    for (int i = 0; i < N; ++i){
        double rho = exp((log(out.rhoy[N-1])-log(out.rhoy[0]))/(N-1)*(i) + log(out.rhoy[0]));
        out.rhoy[i] = rho;
        isp = z.size() + 1; // T
        SPLNVALdll(&isp, &iderv, &rho, &(out.T[i]), &ierr2, herr_buffer, errormessagelength);
        isp = z.size() + 2; // p
        SPLNVALdll(&isp, &iderv, &rho, &(out.p[i]), &ierr2, herr_buffer, errormessagelength);
        isp = z.size() + 3; // rhox
        SPLNVALdll(&isp, &iderv, &rho, &(out.rhox[i]), &ierr2, herr_buffer, errormessagelength);
    }
    return out;
}
/**

*/
FlashReturnStruct FlashMolarRPSI(input_pairs pair, double value1, double value2, const std::vector<double> &z, flash_input_option option)
{
    FlashReturnStruct out;
    char herr_buffer[errormessagelength];
    
    long MOLAR_QUALITY = 1, MASS_QUALITY = 2;
    
    // Resize output mole fraction vectors
    out.x.resize(z.size());
    out.y.resize(z.size());
      
    switch(pair){
        case PT_INPUTS:
            out.p = value1; out.T = value2; 
            TPFLSHdll(&out.T,&out.p,const_cast<double*>(&z[0]),&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength); 
            break;
        case DmolarT_INPUTS:
            out.d = value1; out.T = value2; 
            TDFLSHdll(&out.T,&out.d,const_cast<double*>(&z[0]),&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength); 
            break;
        case DmolarP_INPUTS:
            out.d = value1; out.p = value2;
            PDFLSHdll(&out.p,&out.d,const_cast<double*>(&z[0]),&out.T,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case HmolarP_INPUTS:
            out.h = value1; out.p = value2; 
            PHFLSHdll(&out.p,&out.h,const_cast<double*>(&z[0]),&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case PSmolar_INPUTS:
            out.p = value1; out.s = value2; 
            PSFLSHdll(&out.p,&out.s,const_cast<double*>(&z[0]),&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case PUmolar_INPUTS:
            out.p = value2; out.u = value1; 
            PEFLSHdll(&out.p,&out.u,const_cast<double*>(&z[0]),&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case HmolarSmolar_INPUTS:
            out.h = value1; out.s = value2;
            HSFLSHdll(&out.h,&out.s,const_cast<double*>(&z[0]),&out.T,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case SmolarUmolar_INPUTS:
            out.s = value1; out.u = value2; 
            ESFLSHdll(&out.u,&out.s,const_cast<double*>(&z[0]),&out.T,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.h,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case DmolarHmolar_INPUTS:
            out.d = value1; out.h = value2;
            DHFLSHdll(&out.d,&out.h,const_cast<double*>(&z[0]),&out.T,&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case DmolarSmolar_INPUTS:
            out.d = value1; out.s = value2;
            DSFLSHdll(&out.d,&out.s,const_cast<double*>(&z[0]),&out.T,&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        case DmolarUmolar_INPUTS:
            out.d = value1; out.u = value2;
            DEFLSHdll(&out.d,&out.u,const_cast<double*>(&z[0]),&out.T,&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        case QT_INPUTS:
            out.q = value1; out.T = value2; 
            TQFLSHdll(&out.T,&out.q,const_cast<double*>(&z[0]),&MOLAR_QUALITY,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        case PQ_INPUTS:
            out.p = value1; out.q = value2;
            PQFLSHdll(&out.p,&out.q,const_cast<double*>(&z[0]),&MOLAR_QUALITY,&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case HmolarT_INPUTS:
        {
            out.h = value1; out.T = value2; 
            long kr;
            switch(option){
                case LOWER_DENSITY_ROOT: kr = 1; break;
                case HIGHER_DENSITY_ROOT: kr = 2; break;
                default:
                    out.ierr = 19783;
                    out.herr = "flash_input_option must be either LOWER_DENSITY_ROOT or HIGHER_DENSITY_ROOT when TH is input pair";
                    return out;
            }
            THFLSHdll(&out.T,&out.h,const_cast<double*>(&z[0]),&kr,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        }
        case TUmolar_INPUTS:
        {
            out.T = value1; out.u = value2;
            long kr;
            switch(option){
                case LOWER_DENSITY_ROOT: kr = 1; break;
                case HIGHER_DENSITY_ROOT: kr = 2; break;
                default:
                    out.ierr = 19783;
                    out.herr = "flash_input_option must be either LOWER_DENSITY_ROOT or HIGHER_DENSITY_ROOT when TU is input pair";
                    return out;
            }
            TEFLSHdll(&out.T,&out.u,const_cast<double*>(&z[0]),&kr,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        }
        case SmolarT_INPUTS:
        {
            out.s = value1; out.T = value2; 
            long kr;
            switch(option){
                case LOWER_DENSITY_ROOT: kr = 1; break;
                case HIGHER_DENSITY_ROOT: kr = 2; break;
                default:
                    out.ierr = 19783;
                    out.herr = "flash_input_option must be either LOWER_DENSITY_ROOT or HIGHER_DENSITY_ROOT when TS is input pair";
                    return out;
            }
            TSFLSHdll(&out.T,&out.s,const_cast<double*>(&z[0]),&kr,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        }
        default:
            out.ierr = 1000;
            out.herr = "Invalid pair to FlashMolar";
    }
    std::string buffer(herr_buffer, herr_buffer + errormessagelength);
	buffer += "\0"; // Null terminate
    out.herr = rtrim(buffer);
    return out;
}

FlashReturnStruct FlashMolarWithGuesses(input_pairs pair, double value1, double value2, const std::vector<double> &z, FlashReturnStruct &guess, flash_input_option option)
{
    FlashReturnStruct out;
    char herr_buffer[errormessagelength];
    
    long MOLAR_QUALITY = 1, MASS_QUALITY = 2;
    
    // Resize output mole fraction vectors
    out.x.resize(z.size());
    out.y.resize(z.size());
    out.ierr = 0;

    switch(pair){
        case PQ_INPUTS:{
            //out = guess;
            out.p = value1; out.q = value2; 
            out.T = guess.T; out.d = guess.d; out.dl = guess.dl; out.dv = guess.dv;
            long iFlsh = 4, iGuess = 1;
            out.ierr = 0;
            // SATTP (t,p,x,iFlsh,iGuess,d,Dl,Dv,xliq,xvap,q,ierr,herr)
            SATTPdll(&out.T, &out.p, const_cast<double*>(&z[0]), &iFlsh, &iGuess,
                  &out.d, &out.dl, &out.dv, &out.x[0], &out.y[0], &out.q, &out.ierr, herr_buffer, errormessagelength);
            std::cout << out.q;
            break;
        }
        case PT_INPUTS:{
            if (option == FORCE_VAPOR){
                out.p = value1; out.T = value2; 
                out.d = guess.d;
                long kph = 2; // Vapor solution
                long kguess = 1; // Guess provided
                TPRHOdll(&out.T, &out.p, const_cast<double*>(&z[0]), &kph, &kguess, &out.d, &out.ierr, herr_buffer, errormessagelength);
                break;
            }
            else{
                out.ierr = 101;
                strcpy(herr_buffer, "Invalid imposed phase");
                break;
            }
        }
        default:{
            out.ierr = 99;
            strcpy(herr_buffer, "Invalid pair to FlashMolarWithGuesses");
            std::cout << "Invalid pair to FlashMolarWithGuesses" <<std::endl;
        }
    }
    if (out.ierr != 0){
        std::string buffer(herr_buffer, herr_buffer + errormessagelength);
		buffer += "\0"; // Null terminate
        out.herr = rtrim(buffer);
    }
    return out;
}
FlashReturnStruct FlashMass(input_pairs pair, double value1, double value2, const std::vector<double> &z, flash_input_option option)
{
    double molar_mass = 0;
    WMOLdll(const_cast<double*>(&z[0]), &molar_mass);
    
    // Convert inputs to REFPROP molar units
    double molar_input_1 = 999999999, molar_input_2 = 9999999;
	mass_inputs_to_molar_inputs(pair, value1, value2, molar_mass, molar_input_1, molar_input_2);

    // Do the flash call with molar units
    FlashReturnStruct molar_outputs = FlashMolarRPSI(pair, molar_input_1, molar_input_2, z, option);
    
	// Convert outputs to mass units (TODO)
    return molar_outputs;
}
double REFPROP(const std::string &output, const std::string &name1, const double value1, const std::string &name2, const double value2, const std::string &fluid_string)
{
	std::vector<double> z(1, 1.0);
	SetupReturnStruct setup = Setup(fluid_string);

	// Convert strings to enumerations
	parameters para1 = name_to_parameter(name1);
	parameters para2 = name_to_parameter(name2);
	parameters paraoutput = name_to_parameter(output);

	// Construct input pair in REFPROP's modified SI units (no unit conversion needed)
	double in1 = 10000, in2 = 10000;
	input_pairs input_pair = build_input_pair(para1, value1, para2, value2, in1, in2);
	
	// Do the flash call with REFPROP's modified molar units
    FlashReturnStruct molar_outputs_RPSI = FlashMolarRPSI(input_pair, in1, in2, z);

	// Return the desired variable (no unit conversion needed)
	return molar_outputs_RPSI.keyed_output(paraoutput);
}
double REFPROPSI(const std::string &output, const std::string &name1, const double value1, const std::string &name2, const double value2, const std::string &fluid_string)
{
	std::vector<double> z(1,1.0);
	SetupReturnStruct setup = Setup(fluid_string);

	// Convert strings to enumerations
	parameters para1 = name_to_parameter(name1);
	parameters para2 = name_to_parameter(name2);
	parameters paraoutput = name_to_parameter(output);
	
	// Construct input pair in REFPROP's modified SI units
	double in1 = 10000, in2 = 10000;
	input_pairs input_pair = build_input_pair_RPSI_from_SI(para1, value1, para2, value2, in1, in2);
	
	// Do the flash call with REFPROP's modified molar units
    FlashReturnStruct molar_outputs_RPSI = FlashMolarRPSI(input_pair, in1, in2, z);

	// Return the desired variable converted back to base SI units
	return RPSI_to_SI(paraoutput, molar_outputs_RPSI.keyed_output(paraoutput));
}

} /* namespace REFPROP */