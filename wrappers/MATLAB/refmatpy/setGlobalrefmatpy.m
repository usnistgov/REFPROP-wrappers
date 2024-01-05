function setGlobalrefmatpy
%%
global RP iUnits iMass iFlag error_threshold;                % Assign Python-REPFROP library (RP), and REFPROP parameters (iUnits iMass iFlag), as global variables. Needed for calling properties in REFPROP.
RP = py.ctREFPROP.ctREFPROP.REFPROPFunctionLibrary('C:\Program Files (x86)\REFPROP');     %Initialize REFPROP python library by pointing to REFPROP root directory
iUnits = RP.GETENUMdll(int8(0),'MASS BASE SI').iEnum;   %Get the 'iUnits' index for the desired unit basis. For more information, see % https://refprop-docs.readthedocs.io/en/latest/DLL/high_level.html#f/_/REFPROPdll
iMass = int8(1); % 0: molar fractions; 1: mass fractions
iFlag = int8(0); % 0: don't call SATSPLN; 1: call SATSPLN
error_threshold = 0;
end