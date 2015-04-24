#ifndef REFPROP_H
#define REFPROP_H

#include <string>
#include <vector>

class SetupReturnStruct{
public:
 long ierr; 
 std::string herr; 
 SetupReturnStruct() : ierr(0), herr(""){};
};

class SaturationSplineReturnStruct{
public:
 long ierr; 
 std::string herr; 
 std::vector<double> T, //< Temperature [K]
                     p, //< Pressure [kPa]
                     rhoy, //< Density of the "vapor" phase [mol/L] (this is the liquid phase when phases are inverted)
                     rhox; //< Density of the "liquid" phase [mol/L] (this is the vapor phase when phases are inverted)
 std::vector<std::vector<double> > x, // Molar composition of the "liquid" phase
                                   y; // Molar composition of the "vapor" phase
 SaturationSplineReturnStruct() : ierr(0), herr("") {};
};

class FlashReturnStruct{
public:
 long ierr;
 std::string herr; 
 std::vector<double> x,y;
 double T,p,d,dl,dv,q,u,h,s,cv,cp,w;
 FlashReturnStruct() :ierr(0), herr("") {};
};

double F2K(double);

SaturationSplineReturnStruct BuildSaturationSpline(const std::vector<double> &z);

enum flash_input_pair{INVALID_FLASH_INPUT_PAIR, TD_INPUTS, TP_INPUTS, PD_INPUTS,PH_INPUTS,PS_INPUTS,PU_INPUTS,PE_INPUTS,HS_INPUTS,US_INPUTS,ES_INPUTS,DH_INPUTS,DS_INPUTS,DU_INPUTS,DE_INPUTS,TQ_INPUTS,PQ_INPUTS,TH_INPUTS,TE_INPUTS,TS_INPUTS,TU_INPUTS};
enum flash_input_option{INVALID_FLASH_INPUT_OPTION, FORCE_LIQUID, FORCE_VAPOR, LOWER_DENSITY_ROOT, HIGHER_DENSITY_ROOT};

SetupReturnStruct Setup(long *INPUT, const std::string &fluids, const std::string &hmx_bnc, const std::string &reference_state);
SetupReturnStruct Setup(const std::string &fluids);

FlashReturnStruct FlashMolarWithGuesses(flash_input_pair pair, double value1, double value2, const std::vector<double> &z, FlashReturnStruct &guesses, flash_input_option option = INVALID_FLASH_INPUT_OPTION);
FlashReturnStruct FlashMolar(flash_input_pair pair, double value1, double value2, const std::vector<double> &z, flash_input_option = INVALID_FLASH_INPUT_OPTION);
FlashReturnStruct FlashMass(flash_input_pair pair, double value1, double value2, const std::vector<double> &z, flash_input_option = INVALID_FLASH_INPUT_OPTION);

#endif