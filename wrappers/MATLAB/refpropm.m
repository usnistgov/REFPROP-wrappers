%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% refpropm  Thermophysical properties of pure substances and mixtures.
%   Calling sequence for pure substances:
%      result=refpropm(prop_req, spec1, value1, spec2, value2, substance1)
%
%   Calling predefined mixtures:
%      result=refpropm(prop_req, spec1, value1, spec2, value2, mixture1)
%
%   Calling user defined mixtures:
%      result=refpropm(prop_req, spec1, value1, spec2, value2,
%                                           substance1, substance2, ..., x)
%
%   where
%       prop_req    character string showing the requested properties
%                   Each property is represented by one character:
%                           0   Refprop DLL version number
%                           A   Speed of sound [m/s]
%                           B   Volumetric expansivity (beta) [1/K]
%                           C   Cp [J/(kg K)]
%                           D   Density [kg/m^3]
%                           F   Fugacity [kPa] (returned as an array)
%                           G   Gross heating value [J/kg]
%                           H   Enthalpy [J/kg]
%                           I   Surface tension [N/m]
%                           J   Isenthalpic Joule-Thompson coeff [K/kPa]
%                           K   Ratio of specific heats (Cp/Cv) [-]
%                           L   Thermal conductivity [W/(m K)]
%                           M   Molar mass [g/mol]
%                           N   Net heating value [J/kg]
%                           O   Cv [J/(kg K)]
%                           P   Pressure [kPa]
%                           Q   Quality (vapor fraction) (kg/kg)
%                           S   Entropy [J/(kg K)]
%                           T   Temperature [K]
%                           U   Internal energy [J/kg]
%                           V   Dynamic viscosity [Pa*s]
%                           X   Liquid phase & gas phase comp.(mass frac.)
%                           Y   Heat of Vaporization [J/kg]
%                           Z   Compressibility factor
%                           $   Kinematic viscosity [cm^2/s]
%                           %   Thermal diffusivity [cm^2/s]
%                           ^   Prandtl number [-]
%                           )   Adiabatic bulk modulus [kPa]
%                           |   Isothermal bulk modulus [kPa]
%                           =   Isothermal compressibility [1/kPa]
%                           ~   Cstar [-]
%                           `   Throat mass flux [kg/(m^2 s)]
%                           +   Liquid density of equilibrium phase
%                           -   Vapor density of equilibrium phase
%                           [   Liquid spinodal density [kg/m^3]
%                           ]   Vapor spinodal density [kg/m^3]
%
%                           E   dP/dT (along the saturation line) [kPa/K]
%                           #   dP/dT     (constant rho) [kPa/K]
%                           R   d(rho)/dP (constant T)   [kg/m^3/kPa]
%                           W   d(rho)/dT (constant p)   [kg/(m^3 K)]
%                           !   dH/d(rho) (constant T)   [(J/kg)/(kg/m^3)]
%                           &   dH/d(rho) (constant P)   [(J/kg)/(kg/m^3)]
%                           (   dH/dT     (constant P)   [J/(kg K)]
%                           @   dH/dT     (constant rho) [J/(kg K)]
%                           *   dH/dP     (constant T)   [J/(kg kPa)]
%
%       spec1           first input character:  T, P, H, D, C, R, or M
%                         T, P, H, D:  see above
%                         C:  properties at the critical point
%                         R:  properties at the triple point
%                         M:  properties at Tmax and Pmax
%                            (Note: if a fluid's lower limit is higher
%                             than the triple point, the lower limit will
%                             be returned)
%
%       value1          first input value
%
%       spec2           second input character:  P, D, H, S, U or Q
%
%       value2          second input value
%
%       substance1      file name of the pure fluid (or the first
%                       component of the mixture)
%
%       mixture1        file name of the predefined fluid mixture
%                       with the extension ".mix" included
%
%       substance2,substance3,...substanceN
%                       name of the other substances in the
%                       mixture. Up to 20 substances can be handled.
%                       Valid substance names are equal to the file names
%                       in the C:\Program Files\REFPROP\fluids\' directory.
%
%       x               vector with mass fractions of the substances
%                       in the mixture.
%
%   Examples:
%   1) P = refpropm('P','T',373.15,'Q',0,'water') gives
%      Vapor pressure of water at 373.15 K in [kPa]
%
%   2) [S Cp] = refpropm('SC','T',373.15,'Q',1,'water') gives
%      Entropy and Cp of saturated steam at 373.15 K
%
%   3) D = refpropm('D','T',323.15,'P',1e2,'water','ammonia',[0.9 0.1])
%      Density of a 10% ammonia/water solution at 100 kPa and 323.15 K.
%
%   4) [x y] = refpropm('X','P',5e2,'Q',0.4,'R134a','R32',[0.8, 0.2])
%      Temperature as well as gas and liquid compositions for a mixture
%      of two refrigerants at a certain pressure and quality.
%      Note that, when 'X' is requested, two variables must be sent, the
%      first contains the liquid phase composition and the second
%      the vapor phase composition.
%
%   5) T=refpropm('T','C',0,' ',0,'water')
%      Critical temperature
%
%   6) T=refpropm('T','M',0,' ',0,'r410a.mix')
%      Maximum temperature that can be used to call properties.
%      Shows how to call a predefined mixture.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Originally based on the file refpropm.f90.
%
% Credits:
%   Paul M. Brown, Ramgen Power Systems, Inc.               2004-05-17
%        Modified input parameters to make 'HS' calls
%        Interface now handles 'HP', 'HD' and 'HT' as well
%        Fixed P_rp calculation  for spec2='P' case (moved calc earlier)
%        Added property requests for Cv (O), gamma (K) and speed of sound (A)
%
%   Johannes Lux, German Aerospace Center                   2006-03-30
%       Modified input pressure unit back to [Pa]
%       Interface now works with Matlab R2006a (.mexw32 file format instead of .dll file format)
%       Continuation lines modified to be compatible with Compaq Visual Fortran 9.0
%       No wrong results return with the first call anymore
%       Changed name to "refpropm.f90" to avoid name conflicts with Matlab
%       Function call is for example:
%       refpropm(prop_req, spec1, value1, spec2, value2, substance1)
%       Fluid files are located in C:\Program Files\REFPROP\fluids\
%       new version 7.2 beta, compiled using Matlab R2006a (2006-10-08)
%       new version 7.2 beta (2006-10-24), compiled using Matlab R2006a
%       new version 8.0 beta (2007-01-18), compiled using Matlab R2006b
%       Modified input pressure unit back to [kPa] (2007-02-22)
%
%   Chris Muzny, NIST
%       made changes for 2009a compatibility and 64-bit execution
%
%   Eric Lemmon, NIST
%       allow .ppf files to be loaded
%       allow .mix files to be loaded
%       add molar mass, heating values
%       add HQ input, critical parameters
%       add fugacity, beta, dH/d(rho)
%
%   Keith Wait, Ph.D, GE Appliances                         2011-07-01
%   keith.wait@ge.com
%       Translated to Matlab native code, known to work against Matlab
%       2010b.  Fortran compiler no longer necessary to add new properties,
%       make other modifications.
%       Added outputs B, E, F, J, and R.
%       HQ input regressed.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function varargout = refpropm( varargin )
persistent RefpropLoadedState % use persistent variable to store the state

ierr = 0;
q = 999; % Vapor quality (0: saturated liquid, 1: saturated vapor)
e = 0; % Internal energy (J/mol)
h = 0; % Enthalpy (J/mol)
s = 0; % Entropy (J/mol/K)
cv = 0; % Constant-volume specific heat (J/mol/K)
cp = 0; % Constant-pressure specific heat (J/mol/K)
w = 0; % Speed of sound (m/s)
phaseFlag = 0;

libName = 'refprop';
dllName = 'REFPROP.dll';

% Input Sanity Checking
nc_base = 5;
if (nargin == 6)
    numComponents = 1;
elseif (nargin < 6)
    error('Too few input arguments, should be 6 or more');
elseif (nargin > 26)
    error('Too many input arguments');
else
    numComponents = nargin - nc_base - 1;
end

fluidType = [varargin{5+(1:numComponents)}];

% Load DLL
loaded = libisloaded(libName);
if ~loaded || isempty(RefpropLoadedState)
    FluidDir = 'FLUIDS/';
    switch computer
        case {'GLNXA64', 'GLNX86', 'MACI', 'MACI64', 'SOL64'}
            BasePath = strcat(getenv('HOME'),'/refprop/');
            dllName = 'librefprop.so';
            switch computer
                case {'MACI','MACI64'}
                    dllName = 'librefprop.dylib';
            end
            FluidDir = lower(FluidDir);

        otherwise
            if strcmp(computer, 'PCWIN64')
                BasePath = 'C:/Program Files (x86)/REFPROP/';
                dllName = 'REFPRP64.dll';
            else
                if strcmp(computer, 'PCWIN32') || strcmp(computer, 'PCWIN')
                    BasePath = 'C:/Program Files/REFPROP/';
                    dllName = 'REFPROP.dll';
                else
                    error(strcat('Architecture[',computer,'] is not understood'));
                end
            end
    end
    % v=char(calllib('REFPROP','RPVersion',zeros(255,1))'); % Useful for debugging...
    RefpropLoadedState = struct('FluidType', 'none', 'BasePath', BasePath, 'FluidDir', FluidDir, 'nComp', 0, 'mixFlag', 0, 'z_mix', 0);

    if ~loaded
        REFPROP_header='REFPROP.h';
        prototype = get_header_path();

        w = warning('query');
        warning('off')
        try
            % Start by trying system resolution to find library
            [~,~] = loadlibrary(dllName,prototype,'alias',libName);
        catch ME
            % the following returns:
            % 0 if REFPROP shared library does not exist,
            % 1 if refprop.dll is a variable name in the workspace,
            % 2 if C:\Program Files (x86)\REFPROP\refprop.dll exist, and 3 if refprop.dll exist but is a .dll file in the MATLAB path
            if ~ismember(exist(strcat(BasePath, dllName),'file'),[2 3])
                dllName = lower(dllName);
            end
            if ~ismember(exist(strcat(BasePath, dllName),'file'),[2 3])
                error(strcat(dllName,' could not be found at the absolute path: ',strcat(BasePath, dllName),'.  Please edit the refpropm.m file and add your path to the lines above this error message.'));
            end
            [~,~] = loadlibrary(strcat(BasePath,dllName),prototype,'alias',libName);
        end
        warning(w)
    end
end

% Uncomment this line to see what functions were exported, along
% with input/output arguments
% ---
%libfunctionsview refprop


% Prepare REFPROP
if ~strcmpi(fluidType, RefpropLoadedState.FluidType)
    fluidFile = '';
    RefpropLoadedState.FluidType = '';
    RefpropLoadedState.mixFlag = 0;
    setappdata(0, 'RefpropLoadedState', RefpropLoadedState);
    if strfind(lower(fluidType), '.mix') ~= 0
        RefpropLoadedState.mixFlag = 1;
        fluidName = fluidType;

        fluidFile = strcat(RefpropLoadedState.BasePath, 'mixtures/',fluidName);
        hmxnme = char(32*ones(1,255)); % Pad out the string with spaces (32: ASCII code for space)
        hmxnme(1:length(fluidFile)) = fluidFile;
        mixFile = strcat(RefpropLoadedState.BasePath, ...
            RefpropLoadedState.FluidDir, 'hmx.bnc');
        hmix = char(32*ones(1,255)); % Pad out the string with spaces (32: ASCII code for space)
        hmix(1:length(mixFile)) = mixFile;
        href = 'DEF';
        herr = char(32*ones(1,255)); % Pad out the string with spaces (32: ASCII code for space)
        ncc = 1;
        [~,~,~,nc,~,z,ierr,errTxt] = calllib(libName,'SETMIXdll',hmxnme,hmix,href,ncc,char(32*ones(1,10000)),zeros(1,20),ierr,herr,255,255,3,10e3,255);

    else
        for i = 1:numComponents
            fluidName=varargin{i+5};
            if isempty(strfind(lower(fluidName),'.fld'))
                if isempty(strfind(lower(fluidName),'.ppf'))
                    fluidName = strcat(fluidName,'.fld');
                end
            end
            fluidFile = strcat(fluidFile, RefpropLoadedState.BasePath, ...
                RefpropLoadedState.FluidDir,upper(fluidName),'|');
        end

        path = char(32*zeros(1,10000)); % Pad out the string with spaces (32: ASCII code for space)
        path(1:length(fluidFile)) = fluidFile;
        mixFile = strcat(RefpropLoadedState.BasePath, RefpropLoadedState.FluidDir, 'HMX.BNC');
        hmix = char(32*ones(1,255)); % Pad out the string with spaces (32: ASCII code for space)
        hmix(1:length(mixFile)) = mixFile;
        href = 'DEF';
        herr = char(32*ones(1,255)); % Pad out the string with spaces (32: ASCII code for space)
        [nc, ~,~,~,ierr,errTxt] = calllib(libName,'SETUPdll',numComponents,path,hmix,href,0,herr,10000,255,3,255);
        z = 1;
        % Uncomment the next line to enable the use of AGA EOS for all
        % components in the mixture
        % [ierr errTxt] = calllib(libName,'SETAGAdll',0,herr,255);
    end
    if (ierr > 0)
        error(errTxt);
    end
%Use the call to PREOSdll to change the equation of state to Peng Robinson for all calculations.
%To revert back to the normal REFPROP EOS and models, call it again with an input of 0.
%   [dummy] = calllib(libName,'PREOSdll',2);

%To enable better and faster calculations of saturation states, call the
%subroutine SATSPLN.  However, this routine takes several seconds, and
%should be disabled if changing the fluids regularly.
% herr = char(32*ones(1,255));
% [dummyx ierr errTxt] = calllib(libName,'SATSPLNdll', z, 0, herr, 255);

% Use the following line to calculate enthalpies and entropies on a reference state
% based on the currently defined mixture, or to change to some other reference state.
% The routine does not have to be called, but doing so will cause calculations
% to be the same as those produced from the graphical interface for mixtures.
%   [href dummy dummy dummy dummy dummy ierr2 errTxt] = calllib(libName, 'SETREFdll', href, 2, z, 0, 0, 0, 0, 0, 32*ones(255,1), 3, 255);

    RefpropLoadedState.z_mix = z;
    RefpropLoadedState.nComp = nc;
    RefpropLoadedState.FluidType = lower(fluidType);
end

numComponents = RefpropLoadedState.nComp;

% Extract Inputs from Varargin
propReq = lower(varargin{1});
propTyp1 = lower(varargin{2});
propTyp2 = lower(varargin{4});

propVal1 = varargin{3};
propVal2 = varargin{5};

if length(propVal1) ~= 1
    error('First input value must have a length of 1');
end
if length(propVal2) ~= 1
    error('Second input value must have a length of 1');
end

herr = char(32*ones(1,255));

if length(propReq)==2
    if propReq(2)=='>'
        propReq = propReq(1);
        phaseFlag=1;
    elseif propReq(2)=='<'
        propReq = propReq(1);
        phaseFlag=2;
    end
end

% Calculate Molar Mass
if numComponents == 1
    z = 1;
elseif RefpropLoadedState.mixFlag == 0
    z_kg = cell2mat(varargin(nargin));
    if length(z_kg) ~= numComponents
        error('Mass fraction must be given for all components');
    elseif abs(sum(z_kg)-1) > 1e-12
        error('Mass fractions must sum to 1');
    end
    [~,z,~] = calllib(libName,'XMOLEdll',z_kg,zeros(1,numComponents),0);
elseif RefpropLoadedState.mixFlag == 1
    z = RefpropLoadedState.z_mix;
end
[~,molw] = calllib(libName,'WMOLdll',z,0);
molw = molw*1e-3; % [kg/mol]

% Sanity Check Provided Property Types
if propTyp1 == propTyp2
    error('Provided values are the same type');
end

varargout = cell(size(propReq));

switch propTyp1
    case 'p'
        P_rp = propVal1;
    case 't'
        T = propVal1;
    case 'd'
        D_rp = propVal1 * 1e-3 / molw;
    case 'h'
        h = propVal1 * molw;
    case 'c'
      if numComponents == 1
     [~,~,~,~,T,P_rp,D_rp,~,~,~,~] = calllib(libName,'INFOdll', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      else
         [~,T,P_rp,D_rp,ierr,errTxt] = calllib(libName,'CRITPdll', z, 0, 0, 0, 0, herr, 255);
      end
      [~,~,~,~,e,h,s,cv,cp,w,~] = calllib(libName,'THERMdll', T, D_rp, z, 0, 0, 0, 0, 0, 0, 0, 0);
    case 'r'
      % Properties at the triple-point
      if numComponents == 1
         heos = 'EOS';
         [~,~,~,~,~,T,~,~,~,~,~] = calllib(libName,'LIMITXdll', heos, 300, 0, 0, z, 0, 0, 0, 0, 0, herr, 3, 255);
         if strncmp(RefpropLoadedState.FluidType,'water',5)
            [~,~,T,~,~,~,~,~,~,~,~] = calllib(libName,'INFOdll', 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
         end
         if propReq=='t'
            varargout(1) = {T};  %Exit early if only T required, not all fluids work at Ttrp.
            return
         end
         [~,~,~,~,P_rp,D_rp,Dl,Dv,x,y,e,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'TQFLSHdll', T, 0, z, 2, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
%        [dummy dummy dummy P_rp Dl Dv x y ierr errTxt] = calllib(libName, 'SATTdll', T, z, 1, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, herr, 255);
      else
        error('Triple point not known for mixtures');
      end
    case 'm'
       % Properties at (Tmax,pmax)
       heos = 'EOS';
       [~,~,~,~,~,~,T,~,P_rp,~,~] = calllib(libName,'LIMITXdll', heos, 300, 0, 0, z, 0, 0, 0, 0, 0, herr, 3, 255);
       [~,~,~,D_rp,Dl,Dv,x,y,q,e,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'TPFLSHdll', T, P_rp, z, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, 0, herr, 255);
    case '0'
       % Get the version number of REFPROP
       [~,~,~,~,ierr,~] = calllib(libName,'SETUPdll',-1,char(32*ones(1,255)),char(32*ones(1,255)),char(32*ones(1,3)),0,char(32*ones(1,255)),10000,255,3,255);
       varargout(1)={double(ierr)/10000};
       return
    otherwise
       error('Provided value 1 is not P, T, H, D, C, R, or M');
end

switch propTyp2
    case 'p'
        P_rp = propVal2;
    case 'd'
        D_rp = propVal2 * 1e-3 / molw;
    case 'h'
        h = propVal2 * molw;
    case 's'
        s = propVal2 * molw;
    case 'u'
        e = propVal2 * molw;
    case 'q'
        q = propVal2;
    otherwise
      if (propTyp1 ~= 'c' && propTyp1 ~= 'r' && propTyp1 ~= 'm' )
        error('Provided value 2 is not P, H, S, U, Q, or D');
      end
      propTyp2 = ' ';
end

% Call Appropriate REFPROP Flash Function According to Provided Property Types
if ((propTyp1 == 'p') && (propTyp2 == 'd')) || ((propTyp2 == 'p') && (propTyp1 == 'd'))
    [~,~,~,T,Dl,Dv,x,y,q,e,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'PDFLSHdll',P_rp, D_rp, z, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, 0, herr, 255);
elseif ((propTyp1 == 'p') && (propTyp2 == 'h')) || ((propTyp2 == 'p') && (propTyp1 == 'h'))
    if phaseFlag==0
        [~,~,~,T,D_rp,Dl,Dv,x,y,q,e,s,cv,cp,w,ierr,errTxt] = calllib(libName,'PHFLSHdll',P_rp, h, z, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
    else
        [~,~,~,~,T,D_rp,ierr,errTxt] = calllib(libName,'PHFL1dll',P_rp, h, z, phaseFlag, 0, 0, 0, herr, 255);
        [~,~,~,P_rp,e,h,s,cv,cp,w,~] = calllib(libName,'THERMdll', T, D_rp, z, 0, 0, 0, 0, 0, 0, 0, 0);
    end
elseif ((propTyp1 == 'p') && (propTyp2 == 't')) || ((propTyp2 == 'p') && (propTyp1 == 't'))
    if phaseFlag==0
        [~,~,~,D_rp,Dl,Dv,x,y,q,e,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'TPFLSHdll', T, P_rp, z, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, 0, herr, 255);
    else
        [~,~,~,~,~,D_rp,ierr,errTxt] = calllib(libName,'TPRHOdll', T, P_rp, z, phaseFlag, 0, 0, 0, herr, 255);
        [~,~,~,P_rp,e,h,s,cv,cp,w,~] = calllib(libName,'THERMdll', T, D_rp, z, 0, 0, 0, 0, 0, 0, 0, 0);
    end
elseif ((propTyp1 == 'h') && (propTyp2 == 'd')) || ((propTyp2 == 'h') && (propTyp1 == 'd'))
    [~,~,~,T,P_rp,Dl,Dv,x,y,q,e,s,cv,cp,w,ierr,errTxt] = calllib(libName,'DHFLSHdll', D_rp, h, z, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
elseif ((propTyp1 == 't') && (propTyp2 == 'd')) || ((propTyp2 == 't') && (propTyp1 == 'd'))
    if phaseFlag==0
        % Do a "blind" flash evaluation
        [~,~,~,P_rp,Dl,Dv,x,y,q,e,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'TDFLSHdll', T, D_rp, z, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, 0, herr, 255);
    else
        % Just use the given density to calculate everything else; evaluate the EOS directly
        [~,~,~,P_rp,e,h,s,cv,cp,w,~] = calllib(libName,'THERMdll', T, D_rp, z, 0, 0, 0, 0, 0, 0, 0, 0);
    end
elseif ((propTyp1 == 't') && (propTyp2 == 'h')) || ((propTyp2 == 't') && (propTyp1 == 'h'))
    [~,~,~,~,P_rp,D_rp,Dl,Dv,x,y,q,e,s,cv,cp,w,ierr,errTxt] = calllib(libName,'THFLSHdll', T, h, z, 1, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
else
    switch propTyp2
        case 's'
            switch propTyp1
                case 't'
                    [~,~,~,~,P_rp,D_rp,Dl,Dv,x,y,q,e,h,cv,cp,w,ierr,errTxt] = calllib(libName,'TSFLSHdll', T, s, z, 1, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
                case 'p'
                    [~,~,~,T,D_rp,Dl,Dv,x,y,q,e,h,cv,cp,w,ierr,errTxt] = calllib(libName,'PSFLSHdll', P_rp, s, z, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
                case 'h'
                    [~,~,~,T,P_rp,D_rp,Dl,Dv,x,y,q,e,cv,cp,w,ierr,errTxt] = calllib(libName,'HSFLSHdll', h, s, z, 0, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, herr, 255);
                case 'd'
                    [~,~,~,T,P_rp,Dl,Dv,x,y,q,e,h,cv,cp,w,ierr,errTxt] = calllib(libName,'DSFLSHdll', D_rp, s, z, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
            end
        case 'u'
            switch propTyp1
                case 't'
                    [~,~,~,~,P_rp,D_rp,Dl,Dv,x,y,q,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'TEFLSHdll', T, e, z, 1, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
                case 'p'
                    [~,~,~,T,D_rp,Dl,Dv,x,y,q,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'PEFLSHdll', P_rp, e, z, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
                case 'd'
                    [~,~,~,T,P_rp,Dl,Dv,x,y,q,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'DEFLSHdll', D_rp, e, z, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
                otherwise
                    error('HU not a supported combination');
            end
        case 'q'
            switch propTyp1
                case 't'
                    [~,~,~,~,P_rp,D_rp,Dl,Dv,x,y,e,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'TQFLSHdll', T, q, z, 2, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
                case 'p'
                    [~,~,~,~,T,D_rp,Dl,Dv,x,y,e,h,s,cv,cp,w,ierr,errTxt] = calllib(libName,'PQFLSHdll', P_rp, q, z, 2, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, 0, 0, 0, 0, 0, 0, herr, 255);
%                case 'd'
%                    [dummy dummy dummyx dummy T P_rp Dl Dv x y ierr errTxt] = calllib(libName,'DQFL2dll', D_rp, q, z, 1, 0, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, herr, 255);
                otherwise
                    error('HQ or DQ are not supported combinations');
            end
    end
end

if (ierr > 0)
    error(char(errTxt'));
end

if ~isempty(strfind(propReq,'g')) || ~isempty(strfind(propReq,'n'))
    if q>0 && q<1
        error('Heating value routines not valid for 2-phase states')
    end
    [~,~,~,hg,hn,ierr,errTxt] = calllib(libName,'HEATdll',T,D_rp,z,0,0,0,herr,255);
    if (ierr ~= 0)
        error(char(errTxt'));
    end
end

if ~isempty(strfind(propReq,'v')) || ~isempty(strfind(propReq,'l')) || ~isempty(strfind(propReq,'$')) || ~isempty(strfind(propReq,'%')) || ~isempty(strfind(propReq,'^'))
    if q>0 && q<1
        error('Transport routines not valid for 2-phase states')
    end
    [~,~,~,eta,tcx,ierr,errTxt] = calllib(libName,'TRNPRPdll',T,D_rp,z,0,0,0,herr,255);
    if (ierr ~= 0)
        error(char(errTxt'));
    end
end

% Construct Return Vector
% To add more property choices, just add a new case to this structure and
% follow the examples below.
for i = 1:length(propReq)
    switch propReq(i)
    case 't'
        varargout(i) = {T};
    case 'p'
        varargout(i) = {P_rp};
    case 'h'
        varargout(i) = {h/molw};
    case 's'
        varargout(i) = {s/molw};
    case 'u'
        varargout(i) = {e/molw};
    case 'd'
        varargout(i) = {D_rp*1e3*molw};
    case 'z'
        varargout(i) = {P_rp/D_rp/T/8.314472e0};
    case 'm'
        varargout(i) = {molw*1e3};
    case 'g'
        varargout(i) = {hg/molw};
    case 'n'
        varargout(i) = {hn/molw};
    case '+'
        [~,~,xmolw] = calllib(libName,'XMASSdll',x,zeros(1,numComponents),0);
        varargout(i) = {Dl*xmolw};
    case '-'
        [~,~,ymolw] = calllib(libName,'XMASSdll',y,zeros(1,numComponents),0);
        varargout(i) = {Dv*ymolw};
    case 'q'
        if ((q <= 0) || (q >= 1))
            varargout(i) = {q};
        else
            [~,wmol] = calllib(libName,'WMOLdll',y,0);
            varargout(i) = {q*wmol*1e-3/molw};
        end
    case 'x'
        [~,x_kg,~] = calllib(libName,'XMASSdll',x,zeros(1,numComponents),0);
        [~,y_kg,~] = calllib(libName,'XMASSdll',y,zeros(1,numComponents),0);
%       varargout(i) = {[x_kg ;y_kg]};
        if length(propReq)>1
            error('Only one input is allowed when using property input X since two outputs are returned (liquid and vapor compositions).');
        end
        varargout(i) = {x_kg'};
        varargout(i+1) = {y_kg'};
    case 'f'
        if ((q < 0) || (q > 1))
           [~,~,~,f] = calllib(libName,'FGCTYdll',T,D_rp,z,zeros(1,numComponents));
        else
           %Liquid and vapor fugacties are identical, use liquid phase here
           [~,~,~,f] = calllib(libName,'FGCTYdll',T,Dl,x,zeros(1,numComponents));
        end
        varargout(i) = {f'};
    case 'i'
        if ((q >= 0) && (q <= 1))
            [~,~,~,~,~,sigma,ierr,errTxt] = calllib(libName,'SURTENdll',T,Dl,Dv,x,y,0,0,herr,255);
            if (ierr ~= 0)
                error(char(errTxt'));
            end
            varargout(i) = {sigma};
        else
            error('Surface tension can only be calculated for saturated conditions.');
        end
    case 'e'
        if numComponents > 1
            error('dP/dT (sat) not supported for mixtures');
        end
        [~,~,~,~,~,~,dpdtSat,ierr,errTxt] = calllib(libName,'DPTSATKdll',1,T,1,0,0,0,0,0,herr,255);
        varargout(i) = {dpdtSat};
    case 'y'
        if numComponents > 1
            error('Heat of Vaporization not supported for mixtures');
        end
        [~,~,~,P_rp,Dl,Dv,x,y,ierr,errTxt] = calllib(libName, 'SATTdll', T, z, 1, 0, 0, 0, zeros(1,numComponents), zeros(1,numComponents), 0, herr, 255);
        if (ierr ~= 0)
          error(char(errTxt'));
        end
        [~,~,~,~,~,hl,~,~,~,~,~] = calllib(libName,'THERMdll', T, Dl, z, 0, 0, 0, 0, 0, 0, 0, 0);
        [~,~,~,~,~,hv,~,~,~,~,~] = calllib(libName,'THERMdll', T, Dv, z, 0, 0, 0, 0, 0, 0, 0, 0);
        varargout(i) = {(hv-hl)/molw};
    case '['
        [~,~,rho_molL,~,~] = calllib(libName,'LIQSPNDLdll', T, z, 0, 0, herr, 255);
        varargout(i) = {rho_molL*molw*1000}; % (mol/L)*(kg/mol)*(1000 L/m^3)
    case ']'
        [~,~,rho_molL,~,~] = calllib(libName,'VAPSPNDLdll', T, z, 0, 0, herr, 255);
        varargout(i) = {rho_molL*molw*1000}; % (mol/L)*(kg/mol)*(1000 L/m^3)
    otherwise

        if (q>0 && q<1)
            error('Property not available for 2-phase states.');
        else
            switch propReq(i)
            case 'c'
                varargout(i) = {cp/molw};
            case 'o'
                varargout(i) = {cv/molw};
            case 'k'
                varargout(i) = {cp/cv};
            case 'a'
                varargout(i) = {w};
            case 'v'
                varargout(i) = {eta*1e-6};
            case 'l'
                varargout(i) = {tcx};
            case 'b'
                [~,~,~,P_rp,e,h,s,cv,cp,w,~,~,~,~,~,beta,~,~,~,~,~,~,~,~,~] = calllib(libName,'THERM2dll',T,D_rp,z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
                varargout(i) = {beta};
            case 'w'
                [~,~,~,P_rp,e,h,s,cv,cp,w,~,~,~,~,~,~,~,~,~,drhodT,~,~,~,~,~] = calllib(libName,'THERM2dll',T,D_rp,z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
                varargout(i) = {drhodT*molw*1000};
            case 'j'
                [~,~,~,P_rp,e,h,s,cv,cp,w,hjt] = calllib(libName,'THERMdll', T, D_rp, z, 0, 0, 0, 0, 0, 0, 0, 0);
                varargout(i) = {hjt};
            case 'r'
                [~,~,~,drhodP] = calllib(libName,'DDDPdll', T, D_rp, z, 0);
                varargout(i) = {drhodP*molw*1000};
            case '!'
                [~,~,~,~,~,dhdd_t,~,~,~] = calllib(libName,'DHD1dll',T,D_rp,z,0,0,0,0,0,0);
                varargout(i) = {dhdd_t/molw/molw/1000};
            case '@'
                [~,~,~,dhdt_d,~,~,~,~,~] = calllib(libName,'DHD1dll',T,D_rp,z,0,0,0,0,0,0);
                varargout(i) = {dhdt_d/molw};
            case '*'
                [~,~,~,~,~,~,~,dhdp_t,~] = calllib(libName,'DHD1dll',T,D_rp,z,0,0,0,0,0,0);
                varargout(i) = {dhdp_t/molw};
            case '('
                [~,~,~,~,dhdt_p,~,~,~,~] = calllib(libName,'DHD1dll',T,D_rp,z,0,0,0,0,0,0);
                varargout(i) = {dhdt_p/molw};
            case '&'
                [~,~,~,~,~,~,dhdd_p,~,~] = calllib(libName,'DHD1dll',T,D_rp,z,0,0,0,0,0,0);
                varargout(i) = {dhdd_p/molw/molw/1000};
            case '#'
                [~,~,~,P_rp,e,h,s,cv,cp,w,~,~,~,~,~,~,~,~,dPT,~,~,~,~,~,~] = calllib(libName,'THERM2dll',T,D_rp,z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
                varargout(i) = {dPT};
            case ')'
                [~,~,~,~,~,~,~,~,bs,~,~,~,~] = calllib(libName,'THERM3dll',T,D_rp,z,0,0,0,0,0,0,0,0,0,0);
                varargout(i) = {bs};
            case '='
                [~,~,~,xkappa,~,~,~,~,~,~,~,~,~] = calllib(libName,'THERM3dll',T,D_rp,z,0,0,0,0,0,0,0,0,0,0);
                varargout(i) = {xkappa};
            case '|'
                [~,~,~,~,~,~,~,~,~,xkkt,~,~,~] = calllib(libName,'THERM3dll',T,D_rp,z,0,0,0,0,0,0,0,0,0,0);
                varargout(i) = {xkkt};
            case '$'
                varargout(i) = {eta/D_rp/molw/100/1000};
            case '%'
                varargout(i) = {tcx/D_rp/cp*10};
            case '^'
                varargout(i) = {eta*cp/tcx/molw/1000/1000};
            case '~'
                v = 0;
                [~,~,~,~,cs,~,~,~,~,ierr,errTxt] = calllib(libName,'CSTARdll',T,P_rp,v,z,0,0,0,0,0,0,herr,255);
                varargout(i) = {cs};
            case '`'
                v = 0;
                [~,~,~,~,cs,~,~,~,~,ierr,errTxt] = calllib(libName,'CSTARdll',T,P_rp,v,z,0,0,0,0,0,0,herr,255);
                tmf = 1000*cs*P_rp*sqrt(molw/8.3144621/T);
                varargout(i) = {tmf};
            case '0'
                [~,~,~,~,ierr,~] = calllib(libName,'SETUPdll',-1,char(32*ones(1,255)),char(32*ones(1,255)),char(32*ones(1,3)),0,char(32*ones(1,255)),10000,255,3,255);
                varargout(i)={double(ierr)/10000};
                return
            otherwise
                error('Unknown property type requested.');
            end
        end
    end
    if (ierr > 0)
        error(char(errTxt'));
    end
end


function proto = get_header_path()
    % Systematic lookup method to find possible header file
    % Start with the BasePath, then look through unix standard places
    % and end by looking in MATLAB path.
    %
    % If a prototype file is needed (deployment of compiled code or usage
    % on computers with no valid compilers for MATLAB), run the following
    % command:
    % >> loadlibrary('REFPRP64.dll', 'REFPROP.h', 'mfilename', 'rp_proto64.m')
    if exist('rp_proto64', 'file')
        if nargin(@rp_proto64) == 0
           proto = @rp_proto64;
        else
            proto = @() rp_proto64(BasePath);
        end
    elseif ~isdeployed
        if exist(strcat(BasePath,REFPROP_header), 'file')
            proto = strcat(BasePath,REFPROP_header);
        elseif exist(strcat('/usr/include/',REFPROP_header), 'file')
            proto = strcat('/usr/include/',REFPROP_header);
        elseif exist(strcat('/usr/local/include/',REFPROP_header), 'file')
            proto = strcat('/usr/local/include/',REFPROP_header);
        elseif exist(REFPROP_header, 'file')
            proto = REFPROP_header;
        else
            error('Could not find header file for loading library')
        end
    else
        error('Could not find header file for loading library')
    end
end

end
