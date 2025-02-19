function setGlobalRefMatPy(path)
    %%
    % Declare global variables
    global RP iUnits iMass iFlag error_threshold; %#ok<GVMIS> 
    
    % Default REFPROP installation path
    defaultPath = 'C:\Program Files (x86)\REFPROP';
    
    % Use the provided path if given, otherwise use the default path
    if nargin < 1 || isempty(path)
        path = defaultPath;
    end
    
    % Initialize REFPROP Python library
    RP = py.ctREFPROP.ctREFPROP.REFPROPFunctionLibrary(path);
    
    % Set REFPROP parameters
    iUnits = RP.GETENUMdll(int8(0), 'MASS BASE SI').iEnum; % Get 'iUnits' index for mass-based SI units
    iMass = int8(1); % 0: molar fractions; 1: mass fractions
    iFlag = int8(0); % 0: don't call SATSPLN; 1: call SATSPLN
    error_threshold = 0;
    
    % Confirm the path being used
    fprintf('REFPROP path set to: %s\n', path);
end