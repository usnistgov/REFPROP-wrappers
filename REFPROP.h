#ifndef REFPROP_H
#define REFPROP_H

#include <string>
#include <vector>

namespace REFPROP{



	/// enumerated value, string representation, multiplicative conversion factor from base SI units to REFPROP SI units, description
	#define PARAMETERS \
		X(Dmolar_PARA, "Dmolar", 0.001, "Mole-based density") \
		X(Hmolar_PARA, "Hmolar", 1.0, "Mole-based enthalpy") \
		X(Smolar_PARA, "Smolar", 1.0, "Mole-based entropy") \
		X(Cpmolar_PARA, "Cpmolar", 1.0, "Mole-based constant-pressure specific heat") \
		X(Cp0molar_PARA, "Cp0molar", 1.0, "Mole-based ideal-gas constant-pressure specific heat") \
		X(Cvmolar_PARA, "Cvmolar", 1.0, "Mole-based constant-volume specific heat") \
		X(Umolar_PARA, "Umolar", 1.0, "Mole-based internal energy") \
		X(Gmolar_PARA, "Gmolar", 1.0, "Mole-based Gibbs energy") \
		X(Dmass_PARA, "Dmass", 0.001, "Mass-based density") \
		X(Hmass_PARA, "Hmass", 1.0, "Mass-based enthalpy") \
		X(Smass_PARA, "Smass", 1.0, "Mass-based entropy") \
		X(Cpmass_PARA, "Cpmass", 1.0, "Mass-based constant-pressure specific heat") \
		X(Cp0mass_PARA, "Cp0mass", 1.0, "Mass-based ideal-gas constant-pressure specific heat") \
		X(Cvmass_PARA, "Cvmass", 1.0, "Mass-based constant-volume specific heat") \
		X(Umass_PARA, "Umass", 1.0, "Mass-based internal energy") \
		X(Gmass_PARA, "Gmass", 1.0, "Mass-based Gibbs energy") \
		X(T_PARA, "T", 1.0, "Temperature") \
		X(P_PARA, "P", 0.001, "Pressure") \
		X(Q_PARA, "Q", 1.0, "Vapor quality") \

	#define INPUT_PAIRS \
		X(QT_INPUTS, Q_PARA, T_PARA, "Molar quality, Temperature in K") \
		X(PQ_INPUTS, P_PARA, Q_PARA, "Pressure in Pa, Molar quality") \
		X(QSmolar_INPUTS, Q_PARA, Smolar_PARA, "Molar quality, Entropy in J/mol/K") \
		X(QSmass_INPUTS, Q_PARA, Smass_PARA, "Molar quality, Entropy in J/kg/K") \
		X(HmolarQ_INPUTS, Hmolar_PARA, Q_PARA, "Enthalpy in J/mol, Molar quality") \
		X(HmassQ_INPUTS, Hmass_PARA, Q_PARA, "Enthalpy in J/kg, Molar quality") \
		X(PT_INPUTS, P_PARA, T_PARA, "Pressure in Pa, Temperature in K") \
		X(DmassT_INPUTS, Dmass_PARA, T_PARA, "Mass density in kg/m^3, Temperature in K") \
		X(DmolarT_INPUTS, Dmolar_PARA, T_PARA, "Molar density in mol/m^3, Temperature in K") \
		X(HmolarT_INPUTS, Hmolar_PARA, T_PARA, "Enthalpy in J/mol, Temperature in K") \
		X(HmassT_INPUTS, Hmass_PARA, T_PARA, "Enthalpy in J/kg, Temperature in K") \
		X(SmolarT_INPUTS, Smolar_PARA, T_PARA, "Entropy in J/mol/K, Temperature in K") \
		X(SmassT_INPUTS, Smass_PARA, T_PARA, "Entropy in J/kg/K, Temperature in K") \
		X(TUmolar_INPUTS, T_PARA, Umolar_PARA, "Temperature in K, Internal energy in J/mol") \
		X(TUmass_INPUTS, T_PARA, Umass_PARA, "Temperature in K, Internal energy in J/kg") \
		X(DmassP_INPUTS, Dmass_PARA, P_PARA, "Mass density in kg/m^3, Pressure in Pa") \
		X(DmolarP_INPUTS, Dmolar_PARA, P_PARA, "Molar density in mol/m^3, Pressure in Pa") \
		X(HmassP_INPUTS, Hmass_PARA, P_PARA, "Enthalpy in J/kg, Pressure in Pa") \
		X(HmolarP_INPUTS, Hmolar_PARA, P_PARA, "Enthalpy in J/mol, Pressure in Pa") \
		X(PSmass_INPUTS, P_PARA, Smass_PARA, "Pressure in Pa, Entropy in J/kg/K") \
		X(PSmolar_INPUTS, P_PARA, Smolar_PARA, "Pressure in Pa, Entropy in J/mol/K") \
		X(PUmass_INPUTS, P_PARA, Umass_PARA, "Pressure in Pa, Internal energy in J/kg") \
		X(PUmolar_INPUTS, P_PARA, Umolar_PARA, "Pressure in Pa, Internal energy in J/mol") \
		X(HmassSmass_INPUTS, Hmass_PARA, Smass_PARA, "Enthalpy in J/kg, Entropy in J/kg/K") \
		X(HmolarSmolar_INPUTS, Hmolar_PARA, Smolar_PARA, "Enthalpy in J/mol, Entropy in J/mol/K") \
		X(SmassUmass_INPUTS, Smass_PARA, Umass_PARA, "Entropy in J/kg/K, Internal energy in J/kg") \
		X(SmolarUmolar_INPUTS, Smolar_PARA, Umolar_PARA, "Entropy in J/mol/K, Internal energy in J/mol") \
		X(DmassHmass_INPUTS, Dmass_PARA, Hmass_PARA, "Mass density in kg/m^3, Enthalpy in J/kg") \
		X(DmolarHmolar_INPUTS, Dmolar_PARA, Hmolar_PARA, "Molar density in mol/m^3, Enthalpy in J/mol") \
		X(DmassSmass_INPUTS, Dmass_PARA, Smass_PARA, "Mass density in kg/m^3, Entropy in J/kg/K") \
		X(DmolarSmolar_INPUTS, Dmolar_PARA, Smolar_PARA, "Molar density in mol/m^3, Entropy in J/mol/K") \
		X(DmassUmass_INPUTS, Dmass_PARA, Umass_PARA, "Mass density in kg/m^3, Internal energy in J/kg") \
		X(DmolarUmolar_INPUTS, Dmolar_PARA, Umolar_PARA, "Molar density in mol/m^3, Internal energy in J/mol") \

	 // Use preprocessor to create the Enum
	 enum input_pairs{
		INPUT_PAIR_INVALID = 0, 
	  #define X(Enum, Para1, Para2, String)       Enum,
	   INPUT_PAIRS
	  #undef X
	};

	enum parameters{
		INVALID_PARAMETER = 0,
	  #define X(Enum, String, SI_TO_RPSI, Desc)       Enum,
	   PARAMETERS
	  #undef X
	};


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
	 double keyed_output(parameters key){
		 switch(key)
		 {
			case Hmolar_PARA: return h;
			case Smolar_PARA: return s;
			case Umolar_PARA: return u;
			case Dmolar_PARA: return d;
			case Cpmolar_PARA: return cp;
			case Cvmolar_PARA: return cv;
			case T_PARA: return T;
			case P_PARA: return p;
			default: return -1;
		 }
	 }
	};

	double F2K(double);

	SaturationSplineReturnStruct BuildSaturationSpline(const std::vector<double> &z);

	enum flash_input_option{INVALID_FLASH_INPUT_OPTION, FORCE_LIQUID, FORCE_VAPOR, LOWER_DENSITY_ROOT, HIGHER_DENSITY_ROOT};

	SetupReturnStruct Setup(long *INPUT, const std::string &fluids, const std::string &hmx_bnc, const std::string &reference_state);
	SetupReturnStruct Setup(const std::string &fluids);

	FlashReturnStruct FlashMolarWithGuesses(input_pairs pair, double value1, double value2, const std::vector<double> &z, FlashReturnStruct &guesses, flash_input_option option = INVALID_FLASH_INPUT_OPTION);
	FlashReturnStruct FlashMolarRPSI(input_pairs pair, double value1, double value2, const std::vector<double> &z, flash_input_option = INVALID_FLASH_INPUT_OPTION);
	FlashReturnStruct FlashMass(input_pairs pair, double value1, double value2, const std::vector<double> &z, flash_input_option = INVALID_FLASH_INPUT_OPTION);

	double REFPROP(const std::string &output, 
		           const std::string &name1, 
				   const double value1, 
				   const std::string &name2, 
				   const double val2, 
				   const std::string &fluid_string);
	std::vector<double> REFPROP(const std::string &output, 
		                        const std::string &name1, 
								const std::vector<double> &value1, 
								const std::string &name2, 
								const std::vector<double> &val2, 
								const std::vector<std::string> &fluids, 
								std::vector<double> z);

	double REFPROPSI(const std::string &output, 
					 const std::string &name1, 
					 double value1, 
					 const std::string &name2, 
					 const double val2, 
					 const std::string &fluid_string);
	std::vector<double> REFPROPSI(const std::string &output, 
								  const std::string &name1, 
								  const std::vector<double> value1, 
								  const std::string &name2, 
								  const std::vector<double> &val2, 
								  const std::vector<std::string> &fluids, 
								  std::vector<double> z);

} /* namespace REFPROP */
#endif