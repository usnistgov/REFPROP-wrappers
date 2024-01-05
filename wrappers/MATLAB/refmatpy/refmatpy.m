function [varargout] = refmatpy(hFld,hIn,hOut,a,b,z)

global RP iUnits iMass iFlag error_threshold                %Get the PYTHON library (RP) and the REFPROP configuration parameters: iUnits, iMass, iFlag, error_threshold

%% 
%Call property data
r = RP.REFPROPdll(hFld,hIn,hOut,iUnits,iMass,iFlag,a,b,z);  %Call the REFPROP DLL to get the properties
output = double(r.Output);                                  %Format properties as double-precision numbers
varargout{nargout} = 0;                                     %Allocate the output buffer to the max output index to start
for i=1:nargout                                             %Get as many properties as requested in function call
    varargout{i} = output(i);                               %Write properties to the varargout array, which adjusts size based on number of properties requested
end 

%Handle error codes if the error code ierr is greater than the threshold
error_number = int64(r.ierr);                               %Format 'ierr' to a 64-bit signed integer
if error_number > error_threshold
    error_string = string(r.herr);                          %Format 'herr' error 
    error(error_string)
end

end % refpropm_PY.m


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Example use
%---------------------------------------------------------------------
% In a separate *.m file, or in the Matlab Command Line:
% First need to set the Global Variables for the Python-REPFROP library
% setGlobalRP;                          % Initialize the REFPROP-Python interface and the REFPROP settings, using global variables

% Then, the refpropm_PY can be called to get properties from REFPROP
% [h1] = refmatpy('CO2','TP','H',290,4500e3,{1}); %enthalpy of CO2 at temperature 290 K, pressure 4500 kPa, [J/kg]
% [mu2, k2, rho2, Pr2] = refmatpy('CO2','TQ','VIS,TCX,D,PRANDTL',290,0,{1}); %viscosity [Pa-s], thermal conductivity [W/m-K], density [kg/m3], Prandtl number [-] of CO2, evaluated at 290 K and vapor quality of 0 (saturated liquid)
% [P3, h3, rho3] = refmatpy('CO2;isobutane','TQ','P,H,D',290,1,{0.85 0.15}); %pressure [Pa], enthalpy [J/kg], density [kg/m3] of a mixture of CO2/isobutane 0.85/0.15 (% mass, when iMass=1), evaluated at 290 K and vapor quality of 1 (saturated vapor)
%_____________________________________________________________________

% Description
%---------------------------------------------------------------------
% This function is a REFEPROP-Matlab wrapper that uses the Python interface.
% It computes fluid thermophysical properties using REFPROP, given inputs 
% (generally two) to define the state.
%
% Python is the only officially supported MATLAB-REFPROP interface as of 
% REFPROP 10. To run this wrapper, the user
% must first: (1) Install REFPROP, (2) Install a version of Python compatible
% with the version of Matlab, (3) Install ctREFPROP (a ctypes-based interface to REFPROP)
% into Python. Resources for each of these items are listed below. 
% REFPROP though Python.
%
% (1) Location to purchase REFPROP.
% https://www.nist.gov/srd/refprop
%
% (2) Location to download Python.
% https://www.python.org/downloads/
% Matlab-Python compatability listing:
% https://www.mathworks.com/support/requirements/python-compatibility.html
%
% (3) Description of how to install and use REFPROP in MatLab though Python, 
% including how to install ctREFPROP.
% https://github.com/usnistgov/REFPROP-wrappers/tree/master/wrappers/MATLAB
%
% (4) For more information about REFPROP, see:
%https://refprop-docs.readthedocs.io/en/latest/DLL/high_level.html#f/_/REFPROPdll

% Harrison Skye, NIST, and John Dyreby - Isentropic Development (john@isentropic.dev), 6/15/2023

% inputs
%---------------------------------------------------------------------
% hFld - name of fluid. For example, 'CO2', 'R134a', 'R410A', 'R134a;R32'
% hIn - input string of properties sent to REFPROP. For example, 'TP' for temperature and pressure, or 'PQ' for pressure and quality.
% hOut - Output string of properties to be calculated, where each property is separated by a comma. For example, 'H' for enthalpy, 'S,D,VIS' for entropy, density, viscosity.
% a - numerical value of input property 1
% b - numerical value of input property 2
% z - cell array (need to use the {} brackets) of mixture constituent fractions (molar or mass, depending on the iMass flag value set in this function)

% outputs
%---------------------------------------------------------------------
% varargout - vector of outputs. Number of outputs determined by number specified in 'hOut' input
% For more information on inputs & outputs, see:
% https://refprop-docs.readthedocs.io/en/latest/DLL/high_level.html#f/_/REFPROPdll

% iUnits explanation
%--------------------------------------------------------
% set the units basis. The units base is selected using the iUnits number,
% which corresponds to a string in the iEnum. For example, the 'iEnum: MASS
% BASE SI' has an iUnits=21. This relationship has the potential to change
% in future versions of REFPROP, so it is important to get the iUnits
% number that corresponds to the iEnum being used.
% See REFPROP website for more options.
% https://refprop-docs.readthedocs.io/en/latest/DLL/high_level.html#f/_/REFPROPdll

% iEnum            DEFAULT     MOLAR SI    MASS SI       SI WITH C
% iUnits --->      0           1           2             3
% Temperature      K           K           K             C
% Pressure         kPa         MPa         MPa           MPa
% Density          mol/dm^3    mol/dm^3    kg/m^3        kg/m^3
% Enthalpy         J/mol       J/mol       J/g           J/g
% Entropy          (J/mol)/K   (J/mol)/K   (J/g)/K       (J/g)/K
% Speed            m/s         m/s         m/s           m/s
% Kinematic vis.   cm^2/s      cm^2/s      cm^2/s        cm^2/s
% Viscosity        uPa-s       uPa-s       uPa-s         uPa-s
% Thermal cond.    W/(m-K)     mW/(m-K)    mW/(m-K)      mW/(m-K)
% Surface tension  N/m         mN/m        mN/m          mN/m
% Molar Mass       g/mol       g/mol       g/mol         g/mol
% 
%                  MOLAR       MASS
% iEnum            BASE SI     BASE SI     ENGLISH       MOLAR ENGLISH
% iUnits --->      20          21          5             6
% Temperature      K           K           F             F
% Pressure         Pa          Pa          psia          psia
% Density          mol/m^3     kg/m^3      lbm/ft^3      lbmol/ft^3
% Enthalpy         J/mol       J/kg        Btu/lbm       Btu/lbmol
% Entropy          (J/mol)/K   (J/kg)/K    (Btu/lbm)/R   (Btu/lbmol)/R
% Speed            m/s         m/s         ft/s          ft/s
% Kinematic vis.   m^2/s       m^2/s       ft^2/s        ft^2/s
% Viscosity        Pa-s        Pa-s        lbm/(ft-s)    lbm/(ft-s)
% Thermal cond.    W/(m-K)     W/(m-K)     Btu/(h-ft-R)  Btu/(h-ft-R)
% Surface tension  N/m         N/m         lbf/ft        lbf/ft
% Molar Mass       kg/mol      kg/mol      lbm/lbmol     lbm/lbmol
% 
% iEnum            MKS         CGS         MIXED         MEUNITS
% iUnits --->      7           8           9             10
% Temperature      K           K           K             C
% Pressure         kPa         MPa         psia          bar
% Density          kg/m^3      g/cm^3      g/cm^3        g/cm^3
% Enthalpy         J/g         J/g         J/g           J/g
% Entropy          (J/g)/K     (J/g)/K     (J/g)/K       (J/g)/K
% Speed            m/s         cm/s        m/s           cm/s
% Kinematic vis.   cm^2/s      cm^2/s      cm^2/s        cm^2/s
% Viscosity        uPa-s       uPa-s       uPa-s         cpoise
% Thermal cond.    W/(m-K)     mW/(m-K)    mW/(m-K)      mW/(m-K)
% Surface tension  mN/m        dyne/cm     mN/m          mN/m
% Molar Mass       g/mol       g/mol       g/mol         g/mol
% 
% iEnum            USER (can be changed by calling the REFPROP subroutine)
% iUnits --->      11
% Temperature      C
% Pressure         psig
% Density          kg/m^3
% Enthalpy         J/g
% Entropy          (J/g)/K
% Speed            m/s
% Kinematic vis.   cm^2/s
% Viscosity        mPa-s
% Thermal cond.    W/(m-K)
% Surface tension  N/m
% Molar Mass       g/mol


%hIn explination
%--------------------------------------------------------
%Valid codes are T, P, D, E, H, S, and Q (temperature, pressure, density,
% energy, enthalpy, entropy, and quality). Two of these should be sent 
% together to identify the contents of the a and b variables. For example,
%TP’ would indicate inputs of temperature and pressure, and ‘TQ’ would
% indicate inputs of temperature and quality. A value of 0 for the quality
% will return a saturated liquid state, and a value of 1 will return a
% saturated vapor state. A value between 0 and 1 will return a two-phase
% state. Valid inputs are: TP, TD, TE, TH, TS, TQ, PD, PE, PH, PS, PQ, DE,
% DH, DS, DQ, ES, EQ, HS, HQ, SQ (or the inverse of any of these, e.g., QT)
% (hIn is not case sensitive, e.g., ‘TQ’ = ‘tq’). When q is >0 and <1,
% then the quality uses a molar basis when iMass=0, and a mass basis 
% when iMass=1. The value of iUnits has no effect on the value of q 
% (as either an input or output). The shortcuts Tsat and Psat can be used
% to specify a saturation state for the liquid for a pure fluid. To 
% return, for example, the saturated vapor density, Dvap would be used 
% as an output variable. The order of the properties being sent to the 
% routine in the variables a and b has to be the same as the letters sent
% to hIn; for example, if hIn is ‘QT’, then a=q and b=T.

%hOut parameters - typical
%--------------------------------------------------------
% Label 	Description 
% T 	Temperature 
% P 	Pressure 
% D 	Density 
% V 	Volume 
% E 	Internal energy 
% H 	Enthalpy 
% S 	Entropy 
% CV 	Isochoric heat capacity 
% CP 	Isobaric heat capacity 
% CP/CV 	Heat capacity ratio 
% W 	Speed of sound 
% Z 	Compressibility factor 
% JT 	Isenthalpic Joule-Thomson coefficient 
% A 	Helmholtz energy 
% G 	Gibbs energy 
% R 	Gas constant 
% M 	Molar mass (or of the mixture) 
% TC 	Critical temperature of a pure fluid 
% PC 	Critical pressure of a pure fluid 
% DC 	Critical density of a pure fluid 
% VIS 	Viscosity 
% TCX 	Thermal conductivity 
% PRANDTL 	Prandlt number 
% TD 	Thermal diffusivity 
% KV 	Kinematic Viscosity 
% STN 	Surface tension 
% DE 	Dielectric constant 

%hOut parameters - Extended
% For more information, see https://refprop-docs.readthedocs.io/en/latest/DLL/high_level.html#f/_/REFPROPdll
%--------------------------------------------------------
% Label 	Description 	SI Molar Units 	SI Mass Units
% Regular properties
% T 	Temperature 	[K] 	[K]
% P 	Pressure 	[kPa] 	[kPa]
% D 	Density 	[mol/dm^3] 	[kg/m^3]
% V 	Volume 	[dm^3/mol] 	[m^3/kg]
% E 	Internal energy 	[J/mol] 	[kJ/kg]
% H 	Enthalpy 	[J/mol] 	[kJ/kg]
% S 	Entropy 	[J/(mol*K)] 	[(kJ/kg)/K]
% CV 	Isochoric heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% CP 	Isobaric heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% CP/CV 	Heat capacity ratio 	[-] 	[-]
% W 	Speed of sound 	[m/s] 	[m/s]
% Z 	Compressibility factor 	[-] 	[-]
% JT 	Isenthalpic Joule-Thomson coefficient 	[K/kPa] 	[K/kPa]
% A 	Helmholtz energy 	[J/mol] 	[kJ/kg]
% G 	Gibbs energy 	[J/mol] 	[kJ/kg]
% R 	Gas constant 	[J/(mol*K)] 	[(kJ/kg)/K]
% M 	Molar mass (or of the mixture) 	[g/mol] 	[g/mol]
% QMASS 	Quality (not implemented, q not known) 	N.A. 	[kg/kg]
% QMOLE 	Quality (not implemented, q not known) 	[mol/mol] 	N.A.
% Not so regular properties
% KAPPA 	Isothermal compressibility 	[1/kPa] 	[1/kPa]
% BETA 	Volume expansivity 	[1/K] 	[1/K]
% ISENK 	Isentropic expansion coefficient 	[-] 	[-]
% KT 	Isothermal expansion coefficient 	[-] 	[-]
% BETAS 	Adiabatic compressibility 	[1/kPa] 	[1/kPa]
% BS 	Adiabatic bulk modulus 	[kPa] 	[kPa]
% KKT 	Isothermal bulk modulus 	[kPa] 	[kPa]
% THROTT 	Isothermal throttling coefficient 	[dm^3/mol] 	[m^3/kg]
% Derivatives
% DPDD 	dP/dD at constant T 	[kPa/(dm^3/mol)] 	[kPa/(m^3/kg)]
% DPDT 	dP/dT at constant D 	[kPa/K] 	[kPa/K]
% DDDP 	dD/dP at constant T 	[(mol/dm^3)/kPa] 	[(kg/m^3)/kPa]
% DDDT 	dD/dT at constant P 	[(mol/dm^3)/K] 	[(kg/m^3)/K]
% DTDP 	dT/dP at constant D 	[K/kPa] 	[K/kPa]
% DTDD 	dT/dD at constant P 	[(dm^3/mol)*K] 	[(m^3/kg)*K]
% D2PDD2 	d^2P/dD^2 at constant T 	[kPa/(dm^3/mol)^2] 	[kPa/(m^3/kg)^2]
% D2PDT2 	d^2P/dT^2 at constant D 	[kPa/K^2] 	[kPa/K^2]
% D2PDTD 	d^2P/dTdD 	[kPa/(dm^3/mol)/K] 	[kPa/(m^3/kg)/K]
% D2DDP2 	d^2D/dP^2 at constant T 	[(mol/dm^3)/kPa^2] 	[(kg/m^3)/kPa^2]
% D2DDT2 	d^2D/dT^2 at constant P 	[(mol/dm^3)/K^2] 	[(kg/m^3)/K^2]
% D2DDPT 	d^2D/dPdT 	[(mol/dm^3)/(kPa*K)] 	[(kg/m^3)/[kPa*K]]
% D2TDP2 	d^2T/dP^2 at constant D 	[K/kPa^2] 	[K/kPa^2]
% D2TDD2 	d^2T/dD^2 at constant P 	[(dm^3/mol)^2*K] 	[(m^3/kg)^2*K]
% D2TDPD 	d^2T/dPdD 	[K/(dm^3/mol)/kPa] 	[K/(m^3/kg)/kPa]
% Enthalpy derivatives
% DHDT_D 	dH/dT at constant D 	[(J/mol)/K] 	[(kJ/kg)/K]
% DHDT_P 	dH/dT at constant P 	[(J/mol)/K] 	[(kJ/kg)/K]
% DHDD_P 	dH/dD at constant P 	[(J/mol)*(dm^3/mol)] 	[(kJ/kg)*(m^3/kg)]
% DHDD_T 	dH/dD at constant T 	[(J/mol)*(dm^3/mol)] 	[(kJ/kg)*(m^3/kg)]
% DHDP_T 	dH/dP at constant T 	[(J/mol)/kPa] 	[(kJ/kg)/kPa]
% DHDP_D 	dH/dP at constant D 	[(J/mol)/kPa] 	[(kJ/kg)/kPa]
% Entropy derivatives
% DSDT_D 	dS/dT at constant D 	[(J/mol)/K^2] 	[(kJ/kg)/K^2]
% DSDT_P 	dS/dT at constant P 	[(J/mol)/K^2] 	[(kJ/kg)/K^2]
% DSDD_T 	dS/dD at constant T 	[(J/mol)*(dm^3/mol)/K] 	[(kJ/kg)*(m^3/kg)/K]
% DSDD_P 	dS/dD at constant P 	[(J/mol)*(dm^3/mol)/K] 	[(kJ/kg)*(m^3/kg)/K]
% DSDP_T 	dS/dP at constant T 	[(J/mol)/(kPa*K)] 	[(kJ/kg)/[kPa*K]]
% DSDP_D 	dS/dP at constant D 	[(J/mol)/(kPa*K)] 	[(kJ/kg)/[kPa*K]]
% Virial Coefficients
% Bvir 	Second virial coefficient 	[dm^3/mol] 	[m^3/kg]
% Cvir 	Third virial coefficient 	[(dm^3/mol)^2] 	[(m^3/kg)^2]
% Dvir 	Fourth virial coefficient 	[(dm^3/mol)^3] 	[(m^3/kg)^3]
% Evir 	Fifth virial coefficient 	[(dm^3/mol)^4] 	[(m^3/kg)^4]
% dBvirdT 	1st derivative of B with respect to T 	[(dm^3/mol)/K] 	[(m^3/kg)/K]
% d2BvirdT2 	2nd derivative of B with respect to T 	[(dm^3/mol)/K^2] 	[(m^3/kg)/K^2]
% dCvirdT 	1st derivative of C with respect to T 	[(dm^3/mol)^2/K] 	[(m^3/kg)^2/K]
% d2CvirdT2 	2nd derivative of C with respect to T 	[(dm^3/mol)^2/K^2] 	[(m^3/kg)^2/K^2]
% dDvirdT 	1st derivative of D with respect to T 	[(dm^3/mol)^3/K] 	[(m^3/kg)^3/K]
% d2DvirdT2 	2nd derivative of D with respect to T 	[(dm^3/mol)^3/K^2] 	[(m^3/kg)^3/K^2]
% BA 	Second acoustic virial coefficient 	[dm^3/mol] 	[m^3/kg]
% CA 	Third acoustic virial coefficient 	[(dm^3/mol)^2] 	[(m^3/kg)^2]
% EOS testing properties
% GRUN 	Gruneisen parameter 	[-] 	[-]
% PIP 	Phase identification parameter 	[-] 	[-]
% RIEM 	Thermodyn. curvature (nm^3/molecule) 	  	 
% (Z-1)/D 	(Z-1) over the density 	[dm^3/mol] 	[m^3/kg]
% (Z-1)/P 	(Z-1) over the pressure 	[1/kPa] 	[1/kPa]
% P*V 	Pressure times volume 	[(dm^3/mol)*kPa] 	[(m^3/kg)*kPa]
% S*D 	Entropy times density 	[J/(mol*K)*(mol/dm^3)] 	[(kJ/kg)*(kg/m^3)/K]
% N1/T 	Negative reciprocal temperature 	[1/K] 	[1/K]
% RD 	Rectilinear diameter (Dl+Dv)/2 	[mol/dm^3] 	[kg/m^3]
% Properties from ancillary equations
% ANC-TP 	Vapor pressure from ancillary given T 	[kPa] 	[kPa]
% ANC-TDL 	Sat. liquid dens. from ancillary given T 	[mol/dm^3] 	[kg/m^3]
% ANC-TDV 	Sat. vapor dens. from ancillary given T 	[mol/dm^3] 	[kg/m^3]
% ANC-PT 	Vapor temp. from ancillary given P 	[K] 	[K]
% ANC-DT 	Vapor temp. from ancillary given D 	[K] 	[K]
% MELT-TP 	Melting pressure given T 	[kPa] 	[kPa]
% MELT-PT 	Melting temperature given P 	[K] 	[K]
% SUBL-TP 	Sublimation pressure given T 	[kPa] 	[kPa]
% SUBL-PT 	Sublimation temperature given P 	[K] 	[K]
% Less common saturation properties
% CSAT 	Saturated heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% CV2P 	Isochoric two-phase heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% DPDTSAT 	dP/dT along the saturation line 	[kPa/K] 	[kPa/K]
% DHDZSAT 	dH/dZ along the sat. line (Waring) 	[J/mol] 	[kJ/kg]
% LIQSPNDL 	Density at the liquid spinodal 	[mol/dm^3] 	[kg/m^3]
% VAPSPNDL 	Density at the vapor spinodal 	[mol/dm^3] 	[kg/m^3]
% Excess properties
% VE 	Excess volume 	[dm^3/mol] 	[m^3/kg]
% EE 	Excess energy 	[J/mol] 	[kJ/kg]
% HE 	Excess enthalpy 	[J/mol] 	[kJ/kg]
% SE 	Excess entropy 	[J/(mol*K)] 	[(kJ/kg)/K]
% AE 	Excess Helmholtz energy 	[J/mol] 	[kJ/kg]
% GE 	Excess Gibbs energy 	[J/mol] 	[kJ/kg]
% B12 	B12 	[dm^3/mol] 	[m^3/kg]
% Ideal gas properties
% P0 	Ideal gas pressure 	[kPa] 	[kPa]
% E0 	Ideal gas internal energy 	[J/mol] 	[kJ/kg]
% H0 	Ideal gas enthalpy 	[J/mol] 	[kJ/kg]
% S0 	Ideal gas entropy 	[J/(mol*K)] 	[(kJ/kg)/K]
% CV0 	Ideal gas isochoric heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% CP0 	Ideal gas isobaric heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% CP0/CV0 	Ideal gas heat capacity ratio 	[-] 	[-]
% W0 	Ideal gas speed of sound 	[m/s] 	[m/s]
% A0 	Ideal gas Helmholtz energy 	[J/mol] 	[kJ/kg]
% G0 	Ideal gas Gibbs energy 	[J/mol] 	[kJ/kg]
% P-P0 	Pressure minus ideal gas pressure 	[kPa] 	[kPa]
% Residual properties
% PR 	Residual pressure (P-D*Rxgas*T) 	[kPa] 	[kPa]
% ER 	Residual internal energy 	[J/mol] 	[kJ/kg]
% HR 	Residual enthalpy 	[J/mol] 	[kJ/kg]
% SR 	Residual entropy 	[J/(mol*K)] 	[(kJ/kg)/K]
% CVR 	Residual isochoric heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% CPR 	Residual isobaric heat capacity 	[J/(mol*K)] 	[(kJ/kg)/K]
% AR 	Residual Helmholtz energy 	[J/mol] 	[kJ/kg]
% GR 	Residual Gibbs energy 	[J/mol] 	[kJ/kg]
% Ideal-gas contributions to the Helmholtz energy
% PHIG00 	Red. IG Helmholtz energy A0/RT 	[-] 	[-]
% PHIG10 	tau*[d(A0/RT)/d(tau)] 	[-] 	[-]
% PHIG20 	tau^2*[d^2(A0/RT)/d(tau)^2] 	[-] 	[-]
% PHIG30 	tau^3*[d^3(A0/RT)/d(tau)^3] 	[-] 	[-]
% PHIG01 	del*[d(A0/RT)/d(del)] 	[-] 	[-]
% PHIG02 	del^2*[d^2(A0/RT)/d(del)^2] 	[-] 	[-]
% PHIG03 	del^3*[d^3(A0/RT)/d(del)^3] 	[-] 	[-]
% PHIG11 	tau*del*[d^2(A0/RT)/d(tau)d(del)] 	[-] 	[-]
% PHIG12 	tau*del^2*[d^3(A0/RT)/d(tau)d(del)^2] 	[-] 	[-]
% PHIG21 	tau^2*del*[d^3(A0/RT)/d(tau)^2d(del)] 	[-] 	[-]
% Residual contributions to the Helmholtz energy
% PHIR00 	Red. resid. Helmholtz energy Ar/RT 	[-] 	[-]
% PHIR10 	tau*[d(Ar/RT)/d(tau)] 	[-] 	[-]
% PHIR20 	tau^2*[d^2(Ar/RT)/d(tau)^2] 	[-] 	[-]
% PHIR30 	tau^3*[d^3(Ar/RT)/d(tau)^3] 	[-] 	[-]
% PHIR01 	del*[d(Ar/RT)/d(del)] 	[-] 	[-]
% PHIR02 	del^2*[d^2(Ar/RT)/d(del)^2] 	[-] 	[-]
% PHIR03 	del^3*[d^3(Ar/RT)/d(del)^3] 	[-] 	[-]
% PHIR11 	tau*del*[d^2(Ar/RT)/d(tau)d(del)] 	[-] 	[-]
% PHIR12 	tau*del^2*[d^3(Ar/RT)/d(tau)d(del)^2] 	[-] 	[-]
% PHIR21 	tau^2*del*[d^3(Ar/RT)/d(tau)^2d(del)] 	[-] 	[-]
% Critical point and P,T maximums along isopleth (see above)
% TC 	Critical temperature of a pure fluid 	[K] 	[K]
% PC 	Critical pressure of a pure fluid 	[kPa] 	[kPa]
% DC 	Critical density of a pure fluid 	[mol/dm^3] 	[kg/m^3]
% TCEST 	Estimated critical temperature 	[K] 	[K]
% PCEST 	Estimated critical temperature 	[kPa] 	[kPa]
% DCEST 	Estimated critical density 	[mol/dm^3] 	[kg/m^3]
% TMAXT 	Temperature at cricondentherm 	[K] 	[K]
% PMAXT 	Pressure at cricondentherm 	[kPa] 	[kPa]
% DMAXT 	Density at cricondentherm 	[mol/dm^3] 	[kg/m^3]
% TMAXP 	Temperature at cricondenbar 	[K] 	[K]
% PMAXP 	Pressure at cricondenbar 	[kPa] 	[kPa]
% DMAXP 	Density at cricondenbar 	[mol/dm^3] 	[kg/m^3]
% Reducing parameters
% TRED 	Reducing temperature 	[K] 	[K]
% DRED 	Reducing density 	[mol/dm^3] 	[kg/m^3]
% TAU 	Tc/T (or Tred/T) 	[-] 	[-]
% DEL 	D/Dc (or D/Dred) 	[-] 	[-]
% Limits
% TMIN 	Minimum temperature of the EOS 	[K] 	[K]
% TMAX 	Maximum temperature of the EOS 	[K] 	[K]
% DMAX 	Maximum density of the EOS 	[mol/dm^3] 	[kg/m^3]
% PMAX 	Maximum pressure of the EOS 	[kPa] 	[kPa]
% Transport, etc.
% VIS 	Viscosity 	[uPa*s] 	[uPa*s]
% TCX 	Thermal conductivity 	[W/(m*K)] 	[W/(m*K)]
% PRANDTL 	Prandlt number 	[-] 	[-]
% TD 	Thermal diffusivity 	[cm^2/s] 	[cm^2/s]
% KV 	Kinematic Viscosity 	[cm^2/s] 	[cm^2/s]
% STN 	Surface tension 	[mN/m] 	[mN/m]
% DE 	Dielectric constant 	[-] 	[-]
% Heating values
% SPHT 	Specific heat input 	[J/mol] 	[kJ/kg]
% HFRM 	Heat of formation 	[J/mol] 	[kJ/kg]
% HG 	Gross (or superior) heating value 	[J/mol] 	[kJ/kg]
% HN 	Net (or inferior) heating value 	[J/mol] 	[kJ/kg]
% HGLQ 	Gross Heat. Val. (Liquid) 	[J/mol] 	[kJ/kg]
% HNLQ 	Net Heat. Val. (Liquid) 	[J/mol] 	[kJ/kg]
% HGVOL 	Gross HV (Ideal gas volume basis) 	[MJ/m^3] 	[MJ/m^3]
% HNVOL 	Net HV (Ideal gas volume basis) 	[MJ/m^3] 	[MJ/m^3]
% HEATVAPZ 	Heat of vaporization (for pure fluids) 	[J/mol] 	[kJ/kg]
% HEATVAPZ_T 	…at constant temperature (for mixtures) 	[J/mol] 	[kJ/kg]
% HEATVAPZ_P 	…at constant pressure (for mixtures) 	[J/mol] 	[kJ/kg]
% HEATVALUE 	Heating value (mass or molar basis) 	[J/mol] 	[kJ/kg]
% Other properties
% PINT 	Internal pressure 	[kPa] 	[kPa]
% PREP 	Repulsive part of pressure 	[kPa] 	[kPa]
% PATT 	Attractive part of pressure 	[kPa] 	[kPa]
% EXERGY 	Flow Exergy 	[J/mol] 	[kJ/kg]
% CEXERGY 	Closed System Exergy 	[J/mol] 	[kJ/kg]
% CSTAR 	Critical flow factor 	[-] 	[-]
% TMF 	Throat mass flux 	[kg/(m^2*s)] 	[kg/(m^2*s)]
% FPV 	Supercompressibility 	[-] 	[-]
% SUMFACT 	Summation Factor 	[-] 	[-]
% RDAIR 	Relative Density in air (specific gravity) 	[-] 	[-]
% RDH2O 	Relative Density in water (specific gravity) 	[-] 	[-]
% API 	API Gravity 	[-] 	[-]
% Fluid fixed points for mixtures
% At the “true” critical point of the EOS dP/dD=0 and d^P/dD^2=0 at constant temperature
% TCRIT 	Critical temperature of component i 	[K] 	[K]
% PCRIT 	Critical pressure of component i 	[kPa] 	[kPa]
% DCRIT 	Critical density of component i 	[mol/dm^3] 	[kg/m^3]
% TCTRUE 	True EOS critical temp. of component i 	[K] 	[K]
% DCTRUE 	True EOS critical density of component i 	[mol/dm^3] 	[kg/m^3]
% TTRP 	Triple point temperature of component i 	[K] 	[K]
% PTRP 	Triple point pressure of component i 	[kPa] 	[kPa]
% DTRP 	Triple point density of component i 	[mol/dm^3] 	[kg/m^3]
% TNBP 	Normal boiling point temp. of comp. i 	[K] 	[K]
% REOS 	Gas constant of component i for EOS 	[J/(mol*K)] 	[(kJ/kg)/K]
% MM 	Molar mass of component i 	[g/mol] 	[g/mol]
% ACF 	Acentric factor of component i 	[-] 	[-]
% DIPOLE 	Dipole moment of component i 	[debye] 	[debye]
% TREF 	Ref. state temperature of component i 	[K] 	[K]
% DREF 	Ref. state pressure of component i 	[kPa] 	[kPa]
% HREF 	Ref. state enthalpy of comp. i at T0 and P0 	[J/mol] 	[kJ/kg]
% SREF 	Ref. state entropy of comp. i at T0 and P0 	[J/(mol*K)] 	[(kJ/kg)/K]
% Transport properties as a function of component number
% Viscosity=ETA0+ETAB2+ETAR+ETAC
% Thermal conductivity=TCX0+TCXR+TCXC
% ETA0 	Dilute gas viscosity of component i 	[uPa*s] 	[uPa*s]
% ETAB2 	2nd virial viscosity of component i 	[uPa*s] 	[uPa*s]
% ETAR 	Residual viscosity of component i 	[uPa*s] 	[uPa*s]
% ETAC 	Viscosity critical enhance. of comp. i 	[uPa*s] 	[uPa*s]
% TCX0 	Dilute gas thermal cond. of comp. i 	[W/(m*K)] 	[W/(m*K)]
% TCXR 	Residual (background) cond. of comp. i 	[W/(m*K)] 	[W/(m*K)]
% TCXC 	Cond. crit. enhancement of comp. i 	[W/(m*K)] 	[W/(m*K)]
% Mixture properties as a function of component number
% K 	K value (y/x) (not implemented, y unknown) 	[-] 	[-]
% F 	Fugacities 	[kPa] 	[kPa]
% FC 	Fugacity coefficients 	[-] 	[-]
% CPOT 	Chemical potentials 	[J/mol] 	[kJ/kg]
% DADN 	n*partial(alphar)/partial(ni) 	[-] 	[-]
% DNADN 	partial(n*alphar)/partial(ni) 	[-] 	[-]
% XMOLE 	Composition on a mole basis 	[-] 	[-]
% XMASS 	Composition on a mass basis 	[-] 	[-]
% FIJMIX 	Binary parameters (see REFPROP)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%