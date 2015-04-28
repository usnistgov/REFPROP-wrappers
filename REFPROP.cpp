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

double F2K(double T_F)
{
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

/** Utility function to convert a single input value from REFPROP mass-based units to REFPROP mole-based units
 */
double RPmass_to_RPmolar(char input, double val, double molar_mass)
{
    switch (input){
        case 'T':
        case 'P':
            return val;
        case 'D':
            return val/molar_mass; // [kg/m^3 -> mol/L]
        case 'H':
        case 'S':
        case 'U':
            return val*(molar_mass/1000);// [J/kg -> J/mol]
    }
}

/** Utility function to convert a single input value from base-SI mass-based units to REFPROP mole-based units
 */
double SImass_to_RPmolar(char input, double val, double molar_mass)
{
    switch (input){
        case 'T':
            return val; // [no conversion]
        case 'P':
            return val/1000; // [Pa -> kPa]
        case 'D':
            return val/molar_mass; // [kg/m^3 -> mol/L]
        case 'H':
        case 'S':
        case 'U':
            return val*(molar_mass/1000); // [J/kg -> J/mol]
    }
}

/** Utility function to split two letter flash into two separate characters, one for each part
 */
void split_flash_input_pair(flash_input_pair pair, char &i1, char &i2)
{
    switch (pair){
        case TQ_INPUTS: i1 = 'T'; i2 = 'Q'; break;
        case PQ_INPUTS: i1 = 'P'; i2 = 'Q'; break;
        case TP_INPUTS: i1 = 'T'; i2 = 'P'; break;
        case PD_INPUTS: i1 = 'P'; i2 = 'D'; break;
        case TD_INPUTS: i1 = 'T'; i2 = 'D'; break;
        case TH_INPUTS: i1 = 'T'; i2 = 'H'; break;
        case TU_INPUTS: i1 = 'T'; i2 = 'U'; break;
        case TS_INPUTS: i1 = 'T'; i2 = 'S'; break;
        case PH_INPUTS: i1 = 'P'; i2 = 'H'; break;
        case PS_INPUTS: i1 = 'P'; i2 = 'S'; break;
        case PU_INPUTS: i1 = 'P'; i2 = 'U'; break;
        case HS_INPUTS: i1 = 'H'; i2 = 'S'; break;
        case US_INPUTS: i1 = 'U'; i2 = 'S'; break;
        case DH_INPUTS: i1 = 'D'; i2 = 'H'; break;
        case DS_INPUTS: i1 = 'D'; i2 = 'S'; break;
        case DU_INPUTS: i1 = 'D'; i2 = 'U'; break;
    }
}

/** Utility function to convert from REFPROP mass-based units to REFPROP molar-based units
 * @param value1 The first input parameter in REFPROP mass-based units (kg/m^3, kPa, etc.)
 * @param value2 The second input parameter in REFPROP mass-based units (kg/m^3, kPa, etc.)
 * @param molar_mass The molar mass in g/mol
 * @param output1 The first input parameter in REFPROP molar units (mol/L, kPa, etc.)
 * @param output2 The second output parameter in REFPROP molar units (mol/L, kPa, etc.)
 */
void inputs_mass_to_molar(flash_input_pair pair, double value1, double value2, double molar_mass, double &output1, double &output2)
{
    if (molar_mass > 1000 || molar_mass < 1){
        std::cout << "molar mass is in the wrong unit system" << std::endl;
    }
    char c1, c2;
    split_flash_input_pair(pair, c1, c2);
    output1 = RPmass_to_RPmolar(c1, value1, molar_mass); 
    output2 = RPmass_to_RPmolar(c2, value2, molar_mass); 
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
TODO: Molar fractions to mass fractions
*/
FlashReturnStruct FlashMolar(flash_input_pair pair, double value1, double value2, const std::vector<double> &z, flash_input_option option)
{
    FlashReturnStruct out;
    char herr_buffer[errormessagelength];
    
    long MOLAR_QUALITY = 1, MASS_QUALITY = 2;
    
    // Resize output mole fraction vectors
    out.x.resize(z.size());
    out.y.resize(z.size());
      
    switch(pair){
        case TP_INPUTS:
            out.T = value1; out.p = value2;
            TPFLSHdll(&out.T,&out.p,const_cast<double*>(&z[0]),&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength); 
            break;
        case TD_INPUTS:
            out.T = value1; out.d = value2;
            TDFLSHdll(&out.T,&out.d,const_cast<double*>(&z[0]),&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength); 
            break;
        case PD_INPUTS:
            out.p = value1; out.d = value2;
            PDFLSHdll(&out.p,&out.d,const_cast<double*>(&z[0]),&out.T,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case PH_INPUTS:
            out.p = value1; out.h = value2;
            PHFLSHdll(&out.p,&out.h,const_cast<double*>(&z[0]),&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case PS_INPUTS:
            out.p = value1; out.s = value2;
            PSFLSHdll(&out.p,&out.s,const_cast<double*>(&z[0]),&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case PU_INPUTS:
        case PE_INPUTS:
            out.p = value1; out.u = value2;
            PEFLSHdll(&out.p,&out.u,const_cast<double*>(&z[0]),&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case HS_INPUTS:
            out.h = value1; out.s = value2;
            HSFLSHdll(&out.h,&out.s,const_cast<double*>(&z[0]),&out.T,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case US_INPUTS:
        case ES_INPUTS:
            out.u = value1; out.s = value2;
            ESFLSHdll(&out.u,&out.s,const_cast<double*>(&z[0]),&out.T,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.h,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case DH_INPUTS:
            out.d = value1; out.h = value2;
            DHFLSHdll(&out.d,&out.h,const_cast<double*>(&z[0]),&out.T,&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case DS_INPUTS:
            out.d = value1; out.s = value2;
            DSFLSHdll(&out.d,&out.s,const_cast<double*>(&z[0]),&out.T,&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.u,&out.h,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        case DU_INPUTS:
        case DE_INPUTS:
            out.d = value1; out.u = value2;
            DEFLSHdll(&out.d,&out.u,const_cast<double*>(&z[0]),&out.T,&out.p,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.q,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        case TQ_INPUTS:
            out.T = value1; out.q = value2;
            TQFLSHdll(&out.T,&out.q,const_cast<double*>(&z[0]),&MOLAR_QUALITY,&out.p,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;    
        case PQ_INPUTS:
            out.p = value1; out.q = value2;
            PQFLSHdll(&out.p,&out.q,const_cast<double*>(&z[0]),&MOLAR_QUALITY,&out.T,&out.d,&out.dl,&out.dv,&out.x[0],&out.y[0],&out.u,&out.h,&out.s,&out.cv,&out.cp,&out.w,&out.ierr,herr_buffer,errormessagelength);
            break;
        case TH_INPUTS:
        {
            out.T = value1; out.h = value2;
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
        case TU_INPUTS:
        case TE_INPUTS:
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
        case TS_INPUTS:
        {
            out.T = value1; out.s = value2;
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

FlashReturnStruct FlashMolarWithGuesses(flash_input_pair pair, double value1, double value2, const std::vector<double> &z, FlashReturnStruct &guess, flash_input_option option)
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
        case TP_INPUTS:{
            if (option == FORCE_VAPOR){
                out.T = value1; out.p = value2;
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
FlashReturnStruct FlashMass(flash_input_pair pair, double value1, double value2, const std::vector<double> &z, flash_input_option option)
{
    double molar_mass = 0;
    WMOLdll(const_cast<double*>(&z[0]), &molar_mass);
    double molar_input_1 = 999999999, molar_input_2 = 9999999;
    // Convert inputs to REFPROP molar units
    inputs_mass_to_molar(pair, value1, value2, molar_mass, molar_input_1, molar_input_2);
    // Do the flash call
    FlashReturnStruct molar_outputs = FlashMolar(pair, molar_input_1, molar_input_2, z, option);
    // Convert outputs to molar units (TODO)
    return molar_outputs;
}