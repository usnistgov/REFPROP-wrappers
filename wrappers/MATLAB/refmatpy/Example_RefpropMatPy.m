clear;clc                           % Clear Workspace, Clear command window

% Initialize REFPROP
REFPROP_Path = 'C:\Program Files (x86)\REFPROP';
setGlobalRefMatPy(REFPROP_Path)                         % Assign Python-REPFROP library (RP), and
% REFPROP parameters (iUnits iMass iFlag), as global variables. Needed for calling properties in REFPROP.

% Get properties from REFPROP
[h1] = refmatpy('CO2','TP','H',290,4500e3,{1}) %enthalpy of CO2 at temperature 290 K, pressure 4500 kPa, [J/kg]
[mu2, k2, rho2, Pr2] = refmatpy('CO2','TQ','VIS,TCX,D,PRANDTL',290,0,{1}) %viscosity [Pa-s], thermal conductivity [W/m-K], density [kg/m3], Prandtl number [-] of CO2, evaluated at 290 K and vapor quality of 0 (saturated liquid)
[P3, h3, rho3] = refmatpy('CO2;isobutane','TQ','P,H,D',290,1,{0.85 0.15}) %pressure [Pa], enthalpy [J/kg], density [kg/m3] of a mixture of CO2/isobutane 0.85/0.15 (% mass, when iMass=1), evaluated at 290 K and vapor quality of 1 (saturated vapor)