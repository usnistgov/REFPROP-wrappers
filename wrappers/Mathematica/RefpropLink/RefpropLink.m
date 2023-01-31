(* ::Section:: *)
(* Wolfram Language Package *)

BeginPackage["RefpropLink`",{"NETLink`", "RefpropLink`RefpropUnits`"}]
Unprotect @@ Names["RepropLink`*"];
ClearAll @@ Names["RepropLink`*"];

RefpropLink::usage = "RefpropLink calls functions from REFPRP64.dll included in the REFPROP application";
(* ====================================================================================================== *)
(* ==================================== Legacy REFPROP API functions ==================================== *)
setup::usage = "setup[hFld] loads fluid/mixture files specified in hFld (list) using default reference state, hRef = \"DEF\"" <> 
               "\nsetup[hFld, hRef] loads fluid/mixture files specified in hFld (list) using reference state specified in hRef (string). " <>
               "Number of components is computed from the fluid string, hFld.  " <>
               "See Help and RefProp documentation for hRef options. For custom reference states, use setref[].";
setmix::usage = "setmix[hMixNme, hRef] loads a mixture file (.mix) specified in hMixNme (string) using reference state specified in hRef (string). " <> 
               "See Help and RefProp documentation for hRef options. For custom reference states, use setref[]." <> 
               "\nsetmix[hMixNme] loads a mixture file (.mix) specified in hMixNme (string) using default reference state, hRef = \"DEF\"";
getversion::usage = "getversion[] returns a formatted string containing the loaded RefProp version.";
info::usage = "info[] provides fluid constants for first component (pure fluid)" <> 
              "\ninfo[icomp] returns fluid constants for specified component.";
wmoli::usage = "wmoli[icomp] returns the molar mass (molecular weight [g/mol]) for component (icomp). The parameter icomp can be ommitted to get the molar mass for the first component.";
wmol::usage = "wmol[] returns the molar mass (molecular weight [g/mol]) for the first or only component.\nwmol[z] returns the molar mass (molecular weight [g/mol]) for a mixture of a specified composition in parameter z as a list of reals.";
name::usage = "name[icomp] returns the fluid name from the fluid file loaded in position [icomp]." <>
              "\nname[] returns the fluid name from the fluid file loaded in the first positon (pure fluid loaded)."
longname::usage = "longname[icomp] returns the long fluid name from the fluid file loaded in position [icomp]. To return the file name used when setup[] was called (without path), send -icomp." <>
              "\nlongname[] returns the long fluid name from the fluid file loaded in the first positon (pure fluid loaded)."
casn::usage = "casn[icomp] returns the fluid CASN (Chemical Abstracts Service Number) from the fluid file loaded in position [icomp]." <>
              "\ncasn[] returns the fluid CASN (Chemical Abstracts Service Number) from the fluid file loaded in the first positon (pure fluid loaded)."
limits::usage = "limits[htype, z] provides the temperature, density, and pressure limits of the loaded equations. " <>
                "Parameter htype is optional and specifies the equation set (\"EOS\" (default), \"ETA\" - Viscosity, \"TCX\" - Thermal Conductivity, or \"STN\" - Surface Tension).  " <>
                "The molar composition, z, shoud be provided for mixtures, but can be omitted for pure fluids.";
tprho::usage = "tprho[T,P,kph] iterates for pure fluid density as a function of temperature [K] and pressure [kPa] for a specified phase (kph = 1 [liquid] or 2 [vapor])." <> 
               "\ntprho[T,P,kph,D] iterates for pure fluid density, as above, but D provides an initial guess for molar density [mol/L]." <> 
               "\ntprho[T,P,z,kph] iterates for mixture density as a function of temperature [K], pressure [kPa], and molar composition for a specified phase (kph = 1 [liquid] or 2 [vapor])." <> 
               "\ntprho[T,P,z,kph,D] iterates for mixture density, as above, but D provides an initial guess for molar density [mol/L].";
therm::usage = "therm[T,D] computes all thermodynamic properties as a function of temperature [K] and density [mol/L] for a pure fluid using core functions (Helmholtz free energy, ideal gas heat capacity and various derivatives and integrals)." <> 
               "\ntherm[T,D,z] computes mixture thermal quantities as a function of temperature [K], density [mol/L], and molar composition, z, using core functions (Helmholtz free energy, ideal gas heat capacity and various derivatives and integrals).";
press::usage = "press[T,D] computes pure fluid pressure as a function of temperature [K] and molar density [mol/L]." <> 
               "\npress[T,D,z] computes mixture pressure as a function of temperature [K], molar density [mol/L], and molar composition, z.";
enthal::usage = "enthal[T,D] computes pure fluid enthalpy [J/mol] as a function of temperature [K] and molar density [mol/L]." <> 
               "\nenthal[T,D,z] computes mixture enthalpy [J/mol] as a function of temperature [K], molar density [mol/L], and molar composition, z.";
entro::usage = "entro[T,D] computes pure fluid entropy [J/mol-K] as a function of temperature [K] and molar density [mol/L]." <> 
               "\nentro[T,D,z] computes mixture entropy [J/mol-K] as a function of temperature [K], molar density [mol/L], and molar composition, z.";
cvcp::usage = "cvcp[T,D] computes pure fluid heat capacities [J/mol-K] as a function of temperature [K] and molar density [mol/L]." <> 
               "\ncvcp[T,D,z] computes mixture heat capacities [J/mol-K] as a function of temperature [K], molar density [mol/L], and molar composition, z.";
satp::usage = "satp[P] Calculates pure fluid saturation properties at a given pressure, P [kPa]." <>
              "\nsatp[P,z,kph] Calculate mixture saturation properties at a given pressure, P [kPa], and mole fraction list, z, in the liquid (kph=1) or vapor (kph=2) phase.";
satt::usage = "satt[T] Calculates pure fluid saturation properties at a given temperature, T [K]." <>
              "\nsatt[T,z,kph] Calculate mixture saturation properties at a given temperature, T [K], and mole fraction list, z, in the liquid (kph=1) or vapor (kph=2) phase.";
sats::usage = "sats[s,z,kph] Iterate for temperature, pressure, and density given entropy along the saturation boundary and the composition. " <>
              "kph is set to 0 - Return all VLE roots (defult); 1 - liquid VLE root; 2 - vapor VLE root; 3 - liquid SLE root; 4 - vapor SVE root.";
sath::usage = "sath[h,z,kph] Iterate for temperature, pressure, and density given enthalpy along the saturation boundary and the composition. " <>
              "kph is set to 0 - Return all roots (defult); 1 - liquid VLE root; 2 - vapor VLE root; 3 - liquid SLE root; 4 - vapor SVE root.";
tpflsh::usage = "tpflsh[T,P] is a general flash calculation given temperature [K] and pressure [kPa] for pure fluids. " <> 
                "This routine accepts both single-phase and two-phase states as inputs; for single-phase calculations, the subroutine tprho is much faster." <>
                "\ntpflsh[T,P,z] is a general flash calculation given temperature [K], pressure [kPa], and bulk (molar) composition. " <> 
                "This routine accepts both single-phase and two-phase states as inputs; for single-phase calculations, the subroutine tprho is much faster.";
tdflsh::usage = "tdflsh[T,D] is a general flash calculation given temperature [K] and molar Density [mol/L] for pure fluids. " <> 
                "This routine accepts both single-phase and two-phase states as inputs." <>
                "\ntpflsh[T,P,z] is a general flash calculation given temperature [K], molar Density [mol/L], and bulk (molar) composition. " <> 
                "This routine accepts both single-phase and two-phase states as inputs.";
pdflsh::usage = "phflsh[P,D] is a general flash calculation given temperature [K] and molar Density [mol/L] for pure fluids. " <> 
                "This routine accepts both single-phase and two-phase states as inputs." <>
                "\ntpflsh[T,P,z] is a general flash calculation given temperature [K], molar Density [mol/L], and bulk (molar) composition. " <> 
                "This routine accepts both single-phase and two-phase states as inputs.";
thflsh::usage = "thflsh[t, h, kr] is a general flash calculation given Temperature [K] and Enthalpy [J/mol] for pure fluids. " <> 
                "This routine accepts both single-phase and two-phase states as inputs." <>
                "Often, in the liquid, two solutions exist; one in the two phase region. " <>
                "If this is the case, call thflsh with kr=2 to get the single-phase state.  If omitted, kr=1 (2-phase) is assumed." <> 
                "\nthflsh[t, h, z, kr] is a general flash calculation given Temperature [K], Enthalpy [J/mol], and bulk (molar) composition. " <> 
                "This routine accepts both single-phase and two-phase states as inputs." <>
                "Often, in the liquid, two solutions exist; one in the two phase region. " <>
                "If this is the case, call thflsh with kr=2 to get the single-phase state.  If omitted, kr=1 (2-phase) is assumed.";
tsflsh::usage = "tsflsh[t, s] is a general flash calculation given Temperature [K] and Entropy [J/mol-K] for pure fluids. " <>
                "This routine accepts bot single-phase and two-phase inputs." <>
                "\ntsflsh[t, s, kr] same as tsflsh[t, s], but supplies a root flag when multiple roots exist." <>
                "use kr=2 to get the upper (liquid, single phase) root or kr=1 (default) to get the lower (2-phase) root. " <>
                "This routine accepts bot single-phase and two-phase inputs." <>
                "\ntsflsh[t, s, z] is a general flash calculation given Temperature [K], Entropy [J/mol-K], and bulk (molar) composition for mixtures. " <> 
                "This routine accepts both single-phase and two-phase states as inputs." <>
                "\ntsflsh[t, s, z, kr] same as tsflsh[t, s, z], but supplies a root flag when multiple roots exist. " <>
                "Use kr=2 to get the upper (liquid, single phase) root or kr=1 (default) to get the lower (2-phase) root.";
phflsh::usage = "phflsh[p, h] is a general flash calculation given Pressure [kPa] and Enthalpy [J/mol] for pure fluids. " <> 
                "This routine accepts both single-phase and two-phase states as inputs." <>
                "\nphflsh[p, h, z] is a general flash calculation given Pressure [kPa], Enthalpy [J/mol], and bulk (molar) composition. " <> 
                "This routine accepts both single-phase and two-phase states as inputs.";
psflsh::usage = "psflsh[p, s] is a general flash calculation given Pressure [kPa] and Entropy [J/mol-K] for pure fluids. " <>
                "This routine accepts bot single-phase and two-phase inputs." <>
                "\npsflsh[p, s, z] is a general flash calculation given Pressure [kPa], Entropy [J/mol-K], and bulk (molar) composition for mixtures. " <> 
                "This routine accepts both single-phase and two-phase states as inputs.";
trnprp::usage = "trnprp[t, d] Calculates the single-phase viscosity (mu [\[Mu]Pa-s] and thermal conductivity (k [W/m-K]) for pure fluids" <>
                " as a function of temperature [K] and density [mol/L]. " <>
                "\ntrnprp[t, d, z] Calculates the single-phase viscosity (mu [\[Mu]Pa/s] and thermal conductivity (k [W/m-K]) for mixtures" <>
                " as a function of temperature [K], density [mol/L], and molar composition.";
stn::usage = "stn[t,dl,dv] Computes surface tension [N/m] of pure fluids as a function of saturation temperature, t [K], and the" <>
             "saturated liquid and saturated vapor molar densities, dl and dv [mol/L]." <>
             "\nstn[t,dl,dv,x,y] Computes surface tension [N/m] for mixtures as a function of saturation temperature, t [K], " <>
             "saturated liquid and saturated vapor molar densities, dl and dv [mol/L], and saturated liqud and saturated vapor" <>
             "compositions, x and y."; 
surft::usage = "surft[t] Computes surface tension [N/m] of pure fluids as a function of saturation temperature, t [K]. " <>
             "\nsurft[t,z] Computes surface tension [N/m] for mixtures as a function of saturation temperature, t [K] " <>
             "and the liquid molar composition, z."; 
meltp::usage = "meltp[p] computes the melting temperature [K] as a function of pressure [kPa] for a pure fluid." <>
               "\nmeltp[p,z] computes the melting temperature [K] as a function of pressure [kPa] and mixture composition.";
meltt::usage = "meltt[t] computes the melting pressure [kPa] as a function of temperature [K] for a pure fluid." <>
               "\nmeltt[t,z] computes the melting pressure [kPa] as a function of temperature [K] and mixture composition.";
sublp::usage = "sublp[p] computes the sublimation temperature [K] as a function of pressure [kPa] for a pure fluid." <>
               "\nsublp[p,z] computes the sublimation temperature [K] as a function of pressure [kPa] and mixture composition.";
sublt::usage = "sublt[t] computes the sublimation pressure [kPa] as a function of temperature [K] for a pure fluid." <>
               "\nsublt[t,z] computes the sublimation pressure [kPa] as a function of temperature [K] and mixture composition.";
dielec::usage = "dialec[t, d] Calculate the dimensionless dielectric constant for a pure fluid based on the temperature [K] and molar density [mol/L]." <>
                "\ndialec[t, d, z] Calculate the dimensionless dielectric constant for a mixture  based on the temperature [K], molar density [mol/L], and molar composition.";
fugacity::usage = "fugacity[t, d, z] computes fugacity [kPa] for each of the nc components and repots them back in an array.  The molar composition array, z, may" <>
               " be omitted for pure fluids.";
virial::usage = "virial[t] computes the virial coefficients of a pure fluid as a function of temperature [K]." <>
                "\nvirial[t,z] computes the virial coefficients of a mixture as a function of temperature [K] and molar composition.";

xmass::usage = "xmass[xMol] converts the mole fraction array (List) to a mass fraction array for the currently loaded mixture. " <>
               "The molar mass [g/mol or kg/kmol] of the mixture is also returned.";

xmole::usage = "xmole[xMass] converts the mass fraction array (List) to a mole fraction array for the currently loaded mixture. " <>
               "The molar mass [g/mol or kg/kmol] of the mixture is also returned.";

purefld::usage = "purefluid[icomp] tells REFPROP to use component icomp as a Pure Fluid.";

setref::usage = "setref[hRef] Sets the standard reference state for a pure fluids.  " <>
                "hRef is a 3-character string, either \"DEF\", \"NBP\",\"ASH\", or \"IIR\".  " <>
                "Use \"???\" to return current reference state." <>
                "\nsetref[hRef, x0]  Sets the standard reference state to mixture with composition specified in List x0." <>
                "\nsetref[hRef, x0, h0, s0, T0, P0]  Sets the real gas (hRef = \"OTH\") or ideal gas (hRef = \"OT0\") reference state " <>
                "to a user defined value where h0 and s0 are specified at x0 (if provided for a mixture), T0 [K], and P0 [kPa].";

satspln::usage = "satspln[z] Calculates the phase boundary of a mixture at a given composition, along with the critical point, " <>
                 "cricondentherm, and cricondenbar.";

maxp::usage = "maxp[z] Returns the values at the maximum Pressure along the saturation line (cricondenbar), " <> 
              "calculated from the last call to SATSPLN at the molar composition, z.";

maxt::usage = "maxt[z] Returns the values at the maximum Temperature along the saturation line (cricondentherm), " <>
              "calculated from the last call to SATSPLN at the molar composition, z.";

critp::usage = "critp[z] calculates the critical mixture parameters as a function of molar composition.";

(* Legacy Derivative functions not implemented in version 1.0 of RefpropLink.
   They could easily be added in a later version.  
   All available derivatives can be retrieved through the high-level interface.
====================================================================================================================================================================
dpdd::usage = "dpdd[t,d,z] compute partial derivative of pressure w.r.t. density at constant temperature as a function of temperature, density, and composition";
dpdd2::usage = "dpdd2[t,d,z] compute second partial derivative of pressure w.r.t. density at const temperature as a function of temperature, density, and composition";
dpdt::usage = "dpdt[t,d,z] compute partial derivative of pressure w.r.t. temperature at constant density as a function of temperature, density, and composition";
dhd1::usage = "dhd1[t,d,z] Compute partial derivatives of enthalpy w.r.t. t, p, and rho at constant t, p, or rho as a function of temperature, density, and composition";
*)

(* RFPI::usage = "Brings up a an interface to run set up"; *)
(* checkErrorCodes::usage= "Checks error codes during set up"; *)
(* ==================================== NIST RefProp 10 API functions ================================ *)

ErrMsg::usage = "ErrMsg[ierr] returns the error message string for the last ierr returned.";

SetFluids::usage = "SetFluids[hFld] loads fluid component file(s) specified in the hFld string (setup replacement).  Load a single pure fluid, or load multiple fluids " <>
                   "separated by a pipe symbol \"|\", a semicolon \";\", or an asterisk \"*\".";
                   
SetMixture::usage = "SetMixture[hMixNme] loads a predefined mixture file (.mix) specified in the hMixNme string (setmix replacement).";

Flags::usage = "Flags[hFlag, jFlag] set/get flags for desired behavior from RefProp. Specify the flag to be set in the hFlag string and the flag value " <>
               "in jFlag. Use jFlag=-999 to get current state. If there are no errors, the current/set value is returned.";
               
GetEnum::usage = "GetEnum[hEnum] translate a character string, hEnum, into an enumerated integer value. " <>
                 "\nGetEnum[iFlag, hEnum] translate a character string, hEnum, into an enumerated integer value. The optional first parameter, iFlag, " <>
                 "specifies which type of enumerated value to return (see help for iFlag options).";
                 
RefProp::usage = "RefProp[hFld,hIn,hOut,a,b,z] is a multi-purpose function which takes up to two inputs, a & b, as designated by the string, hIn. " <>
                 "Fluid properties, specified in hOut, are calculated for the fluid specified in hFld. If hFld is a mixture, the molar " <>
                 "composition is passed in z." <>
                 "\nRefProp[hFld,hIn,hOut,a,b] is the same as above, but for pure fluids z can be omitted." <>
                 "\nRefProp[hIn,hOut,a,b,z] is the same as above, but subsequent calls for the same fluid/mixture can omit the fluid string, hFld.";

SetDLL::usage = "SetDLL[] Open a dialog to allow the user to specify which REFPROP 64.bit DLL to use (REFPRP64.DLL).  If cancelled " <>
                "or selected file does not end with \"*64.DLL\", current location remains unchanged." <> 
                "\nSetDLL[permSet] Same behavior, except that setting the optional argument, permSet, to 0 (default) or omitting it" <>
                "will temporarily set the DLL location for this session; but permSet = 1+ will save the setting for future sessions.";

SetPath::usage = "SetPath[] Set the fluids files directory with a pop-up System File Dialog." <> 
                 "\nSetPath[hPth,permSet] Set fluids files directory.  If hPth string input is specified, the File Dialog is not " <> 
                 "presented.  If permSet is omitted or zero (0), setting is temporary; permSet = 1+ saves setting for future sessions.";

Begin["`Private`"]

InstallNET[];

(* ::Section:: *)
(* Initialization *)

(***************************************************************************************)
(*                            Package initialization                                   *)
(***************************************************************************************)
(*   $var - Indicates a Global variable within the package                             *)
(*   var1 - Indicates a .NETLink object that must be created, used, released.          *)
(***************************************************************************************)
$ncmax=20;                      (* maximum number of components                        *)
$nc=0;                          (* Initialize number of components to zero             *)
$refpropcharlength=255;         (* number of characters reserved for refprop strings   *)
$referencelength=3;             (* number of characters reserved for reference string  *)
$errormessagelength=255;        (* number of characters reserved for error strings     *)
$componentstringlength=10000;   (* number of characters reserved for component strings *)
$hnamlength=12;                 (* number of characters reserved for fluid name        *)
$hnam80length=80;               (* number of characters reserved for long fluid name   *)
$hcasnlength=12;                (* number of characters reserved for CASN string       *)
$RPVersion="Unknown";           (* RefProp version is initially "Unknown"              *)
$fluidmixComponents="";         (* Buffer to pass the Fluid Mixture Components string  *)
$vMajor=0;                      (* Cannot find version until setup0[] defined below    *)
                                (* Check at end of definitions & print version string. *)
$iUnits=0;                      (* Initialize unit system variable in case ver<10.     *)
General::refprop = "`1`";       (* General error message structure for RefProp errors. *)
General::mixed = "Inputs cannot be mixed Quantity/non-Quantity values.";

(* Clear Paths and components in case they are already set                             *)
Clear[$dllPath,$fluidsPath,$mixturesCoeffPath,$fluidmixComponents];

(***************************************************************************************)
(*                Get Default REFPROP Paths from "Filepaths.txt"                       *)
(* Contains two comma separated paths on a single line:                                *)
(*  1. Full path to REFPRP64.DLL                                                       *)
(*     (default: C:\Program Files (x86)\REFPROP\REFPRP64.dll                           *)
(*  2. Parent path above \fluids & \mixtrues directory                                 *)
(*     (default: C:\Program Files (x86)\REFPROP                                        *)
(* If the text file exists, use it.  If not, set default directory paths.              *)
(***************************************************************************************)
$filepath = "";                                              (* Set default file paths *)
$dllPath = "C:\\Program Files (x86)\\REFPROP\\REFPRP64.dll";
$NISTPath = "C:\\Program Files (x86)\\REFPROP"
(* Check for FilePaths Resource, which will overwrite these paths                      *)
$hPaths = PacletManager`PacletResource["RefpropLink","FilePaths"];
If[$hPaths === Null,                                            (* Can't find Resource *)
    hEclPaths = FileNameJoin[{$HomeDirectory, "eclipse-workspace", "RefpropLink", "RefpropLink", "TextResources", "Filepaths.txt"}];
    If[FileExistsQ[hEclPaths],         (* See if there's an Eclipse workspace resource *)
        $hPaths = hEclPaths;           (*            IF so, set file paths from there. *)
        (* Print["Using Eclipse-Working Resources. hPaths = ",$hPaths]; *)
    ];
];
If[FileExistsQ[$hPaths],
    (* Print["Found a Filepaths.txt.  Not using defaults."]; *)
    $filepath = StringSplit[Import[$hPaths],","];
    $dllPath = $filepath[[1]];                                           
    $NISTPath = $filepath[[2]];
    (* Print["$dllPath = ",$dllPath]; *)
    (* Print["$NISTPath = ",$NISTPath]; *)
(*, ELSE  ==> ONLY needed if printing debug line below. *)
    (* Use the default DLL and Fluids paths set above.                          *)
    (* Print["Filespaths.txt not found.  Using default DLL and Fluids paths."]; *)
];


(***************************************************************************************)
(* Setup critical directory paths for later use                                        *)
(***************************************************************************************)
$fluidsPath=FileNameJoin[{$NISTPath,"fluids"}];                  (* $REFPROP/fluids    *)
$mixturesPath=FileNameJoin[{$NISTPath,"mixtures"}];              (* $REFPROP/mixtures  *)
$mixturesCoeffPath=FileNameJoin[{$NISTPath,"fluids","HMX.BNC"}]; (* mix coefficients   *)

$DLL = FileExistsQ[$dllPath];
If[NOT[$DLL],
   Print["REFPRP64.dll not found."];
   Print["Make sure NIST REFPROP is installed on the machine."];
   Print["If installed in other than the default location [C:\\Program Files (x86)\\REFPROP],"];
   Print["  call SetDLL[1] to locate the REFPRP64.DLL file on the machine and store it in"];
   Print["  the RefpropLink paclet TextResource for subsequent sessions."];
   ];
   
(***************************************************************************************)
(*            Setup global variable arrays and error string for re-use.                *)
(***************************************************************************************)
(*  Make .NET Objects for getting parameters back from DLLs                            *)
(*  It can take up to a ms to Make and Release these for each call. Making these       *)
(*  objects permanent, package globals is an attempt to speed up the function calls.   *)
(*  Conversely, it takes around a nano-second to attach/release the DLL calls from the *)
(*  REFPRP64.DLL, so those are left to be created/destroyed in each function call.     *)
(***************************************************************************************)
(*  Text buffers created a StringBuilder types per NetLink User Guide, pp 90-91        *)
(***************************************************************************************)
(* Error codes/strings *****************************************************************)
        $herr1=NETNew["System.Text.StringBuilder",255];   (* Error String Buffer       *)
        $hUnits1=NETNew["System.Text.StringBuilder",255]; (* RefProp String Buffer     *)
(* Return .NETLink Real Objects *****************************************************  *)
        $fug1=MakeNETObject[PadRight[Take[{1.0}, UpTo[$ncmax]], $ncmax]];  (* Fugacity *)
        $xliq1=MakeNETObject[PadRight[Take[{1.0}, UpTo[$ncmax]], $ncmax]]; (* Liq. frx *)
        $xvap1=MakeNETObject[PadRight[Take[{1.0}, UpTo[$ncmax]], $ncmax]]; (* Vap. frx *)
        $x3=MakeNETObject[PadRight[Take[{1.0}, UpTo[$ncmax]], $ncmax]];    (* dummy    *)
        $Output1=MakeNETObject[PadRight[Take[{0.0}, UpTo[200]], 200]];     (* Output   *)
(* Return .NETLink String Objects (Text buffer per NetLink User Guide, pp 90-91)       *)
        $hname1=NETNew["System.Text.StringBuilder",12];  (* Component Name Buffer      *)
        $hlong1=NETNew["System.Text.StringBuilder",80];  (* Component Long Name Buffer *)
        $hcasn1=NETNew["System.Text.StringBuilder",12];  (* Component CASN Buffer      *)
        $href1=NETNew["System.Text.StringBuilder",3];    (* in/out hRef Buffer         *)

(***************************************************************************************)
(*                     Mathematica 12.2 fix for .NET/Link.                             *)
(* This is a workaround provided by Wolfram.  It appears that a change in the default  *)
(* behavior of ValueQ causes an error in the calls to .NET/Link CreateDLLFunction.     *)
(* Luckily, they left the legacy behavior in the code and it can be activated by       *)
(* setting the option Method -> "Legacy" for all calls to ValueQ.                      *)
(* A permanent fix should be available in 12.3 or an update to .NET/Link and this      *)
(* workaround won't be necessary                                                       *)
(***************************************************************************************)
If[ $VersionNumber == 12.2,                     (* ONLY if Mathematica version 12.2    *)
    SetOptions[ValueQ,Method->"Legacy"]         (*   Set option on ValueQ function     *)
];                                              (*   for all calls of ValueQ           *)

(***************************************************************************************)
(*               Mathematica 13.2 fix for R ("DegreesRankine") Units                   *)
(* This is a workaround provided by Wolfram.  It appears that a change in the delta    *)
(* temperature units causes "DegreesRankine" to be handled as a non-absolute quantity. *)
(* For version 13.2, the legacy behavior in the code can be activated by with the      *)
(* command below that sets R as an absolute temperature type.                          *)
(* A permanent fix should be available in 13.3 and this workaround won't be necessary. *)
(***************************************************************************************)
If[ $VersionNumber == 13.2,                     (* ONLY if Mathematica version 13.2    *)
    QuantityUnits`Private`hasNonZeroTempUnitQ["DegreesRankine"]=True;  (*  Workaround  *)
];                                              (* Set R as absolute quantity          *)

(***************************************************************************************)
(*                            Package initialization Complete                          *)
(***************************************************************************************)

(* ::Section:: *)
(* Legacy API *)

(***************************************************************************************)
(*                   Define Legacy (Pre-RefProp 10) API Functions                      *)
(***************************************************************************************)

(* ::SubSection:: *)
(* Setup Functions *)

$hExts = {"*.FLD", "*.PPF"};
chkFLD[a_String] := Module[{},
   If[!StringMatchQ[StringTrim[a], $hExts, IgnoreCase -> True],
     Return[StringTrim[a] <> ".FLD"],
     Return[StringTrim[a]]
     ];
   ];

setup::NoMix = "Mixture file detected [`1`]. Must use the function setmix for pre-defined mixtures.";

setup[ fluidComponents_List, nComp_Integer:1, refState_String:"DEF" ]:=
    Module[ {loadSETUPdll, ncomps, fcList, ierr=0 } ,
        $herr1@Clear[];                                                  (* Clear text buffer for error string           *)
        If[ nComp<0,                                                     (* IF nComp < 0                                 *)
            ncomps=nComp                                                 (*    Set ncomp = nComp (using special flag)    *)
        ,                                                                (* OTHERWISE                                    *)
            ncomps=Length[fluidComponents]                               (*    Determine value from fluids list          *)
        ];                                                               (* END IF                                       *)
        
        fcList = chkFLD/@fluidComponents;                                (* Check for missing .FLD extensions if RP v9   *)
        If[ncomps>1,
            $fluidmixComponents=StringRiffle[fcList,"|"],                (* Combine multiple components if more than 1   *)
            $fluidmixComponents=fcList[[1]];                             (* Otherwise, use raw string                    *)
            If[StringMatchQ[$fluidmixComponents, {"*.MIX"}, IgnoreCase -> True],
                Message[setup::NoMix,$fluidmixComponents]                (* Warn if trying to set predefined mixture     *)
            ];
        ];
        (* Print["Fluid String = ",$fluidmixComponents];  *)
        loadSETUPdll=DefineDLLFunction["SETUPdll",$dllPath,"void",{"long*","char*","char*","char*","long*","System.Text.StringBuilder", "long","long","long","long"}];
        loadSETUPdll[ncomps,$fluidmixComponents,$mixturesCoeffPath,refState,ierr,$herr1,$componentstringlength,$refpropcharlength,$referencelength,$errormessagelength];

        If[ncomps>0,                                                     (* IF ncomp>0, ierr holds actual error code     *)
            If[Abs[ierr]>0,                                              (*    IF non-zero error code                    *)
                Message[setup::refprop,StringTrim[$herr1@ToString[]]],   (*       Print the error string                 *)
            (* ELSE *)                                                   (*    ELSE                                      *)
                $nc=ncomps                                               (*       Set global number of components        *)
            ]                                                            (*    END IF                                    *)
        ];                                                               (* END IF                                       *)
        ReleaseNETObject[loadSETUPdll];                                  (* Release the .NET DLL objects from memory     *)
        Return[ierr];                                                    (* Return integer value of ierr                 *)
    ]
    

setmix[hMixNme_String, refState_String:"DEF"]:=
    Module[{loadSETMIXdll,herr,ierr=0,ncc=0,hFiles1,z1,x,hFiles,z},
        hFiles1=NETNew["System.Text.StringBuilder",10000];              (* Text buffer for mixture File names            *)
        z1=MakeNETObject[PadRight[Take[{0.0}, UpTo[$ncmax]], $ncmax]];   (* Initialize z1 buffer for mole fractions      *)
        $herr1@Clear[];                                                  (* Clear text buffer for error string           *)
        loadSETMIXdll=DefineDLLFunction["SETMIXdll",$dllPath,"void",
            {   "char*",                                (*  Character*255   hmxnme *)
                "char*",                                (*  Character*255   hfmix  *)
                "char*",                                (*  Character*3     hrf    *)
                "long*",                                (*  Integer         ncc    *)
                "System.Text.StringBuilder",            (*  Character*10000 hFiles *)
                "double[]",                             (*  Double          z[ncc] *)
                "long*",                                (*  Integer         ierr   *)
                "System.Text.StringBuilder",            (*  Character*255   hErr   *)
                "long",                                 (*  Integer hmxnme_length  *)
                "long",                                 (*  Integer hfmix_length   *)
                "long",                                 (*  Integer hrf_length     *)
                "long",                                 (*  Integer hFiles_length  *)
                "long"                                  (*  Integer hErr_length    *)
            }];
        Quiet[loadSETMIXdll[hMixNme,                    (*  Character*255          *)
                      $mixturesCoeffPath,               (*  Character*255          *)
                      refState,                         (*  Character*3            *)
                      ncc,
                      hFiles1,                          (*  Character*10000        *)
                      z1,
                      ierr,
                      $herr1,                           (*  Character*255          *)
                      $refpropcharlength,               (*    255                  *)
                      $refpropcharlength,               (*    255                  *)
                      $referencelength,                 (*      3                  *)
                      $componentstringlength,           (*  10000                  *)
                      $errormessagelength               (*    255                  *)
                      ]];
        If[ ierr != 0 ,                                                  (* IF non-zero error code                       *)
            herr = StringTrim[$herr1@ToString[]],                        (*     Extract Error Message if there is one    *)
        (* ELSE *)                                                       (* ELSE                                         *)
            $nc = ncc;                                                   (*    Set global number of components           *)
            x=NETObjectToExpression[z1];                                 (*    Extract the liquid mole frxn array        *)
            z=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];          (*    Remove any trailing zeroes                *)
            hFiles = StringSplit[                                        (*    Split fluid files                         *)
                        StringReplace[
                            StringTrim[                                  (*     After trimming string                    *)
                                StringDrop[hFiles1@ToString[],-1]        (*       and dropping very last null character  *)
                            ],
                            ".FLD"->""                                   (*      Removing ending ".FLD" extensions       *)
                        ],
                        "|"                                              (*      Splitting at the pipe | characters      *)
                      ];
            hFiles = Last[FileNameSplit[#]]&/@hFiles;                    (*    Remove full path if present               *)
        ];                                                               (* END IF                                       *)
        ReleaseNETObject[loadSETMIXdll];                                 (* Release the .NET DLL objects from memory     *)
        ReleaseNETObject[hFiles1];
        ReleaseNETObject[z1];
        If[ ierr != 0,                                                   (* IF ierr1 != 0                                *)
            Message[setmix::refprop,herr];                               (*     Print the error string                   *)
            Return[{"ierr"->ierr}],                                      (*     Return integer value of ierr1            *)
        (*ELSE*)                                                         (* ELSE                                         *)
            Return[{"hFiles"->hFiles, "z"->z}]                           (*     Return Fluids List and mole fractions    *)
        ]
    ]

General::PurSet = "INFO: Reference State [`1`] has been set for pure fluid.";
General::MixSet = "INFO: Reference State [`1`] has been set for the mixture composition.";

setref[hRef_String:"???",x0_List:{1.0},h0_:0,s0_:0,T0_:0,P0_:0]:=
    Module[{loadSETREFdll,ixFlag=1,x20,ierr=0,href},
        $href1@Clear[];                                 (* Clear $href String Buffer                    *)
        $href1@Append[hRef];                            (* put hRef in String Buffer                    *)
        ixFlag = Min[Length[x0],2];                     (* ixFlag = 1 (pure fluid) or 2 (mixture)       *)
        If[mixedUnits[T0,P0] || mixedUnits[T0,s0] ||
           mixedUnits[T0,h0],                           (* Check for mixed Quantity/non-Quantity inputs *)
            Message[RefProp::mixed];Return[];           (*   Issue message and return                   *)
        ];
        x20=PadRight[Take[x0, UpTo[$ncmax]], $ncmax];   (* Trim & pad mole fraction list                *)
        loadSETREFdll=DefineDLLFunction[
            "SETREFdll",                                (* Function name                                *)
            $dllPath,                                   (* Full path to DLL                             *)
            "void",                                     (* No Return value                              *)
                {"System.Text.StringBuilder",           (* $href1 - passed as a NETObject buffer (3)    *)    
                 "long*",                               (* ixFlag (1 or 2)                              *)
                 "double[]",                            (* x0 array, length = 20                        *)
                 "double*",                             (* h0 - Reference Enthalpy                      *)
                 "double*",                             (* s0 - Reference Entropy                       *)
                 "double*",                             (* T0 - Reference state Temperature [K]         *)
                 "double*",                             (* P0 - Reference state Pressure [kPa]          *)
                 "long*",                               (* ierr - return error code                     *)
                 "System.Text.StringBuilder",           (* $herr1 - global error message buffer         *)
                 "long",                                (* $referencelength = 3                         *)
                 "long"                                 (* $errormessagelength = 255                    *)
                }
            ];
                                                        (* Call PUREFLUIDdll with component ic          *)
        loadSETREFdll[$href1,                           (* $href1 - passed as a NETObject buffer (3)    *)
                      ixFlag,                           (* ixFlag (1 or 2)                              *)
                      x20,                              (* mole fractions                               *)
                      rpUnits[h0],                      (* Reference State Enthalpy                     *)
                      rpUnits[s0],                      (* Reference State Enthalpy                     *)
                      rpUnits[T0],                      (* Reference state Temperature [K]              *)
                      rpUnits[P0],                      (* Reference state Pressure [kPa]               *)
                      ierr,                             (* Return error code                            *)
                      $herr1,                           (* error message buffer                         *)
                      $referencelength,
                      $errormessagelength
        ];
        ReleaseNETObject[loadSETREFdll];                (* Release .NET DLL Object                      *)
        
        If[ierr > 0,                                     (* IF non-zero error code returned             *)
            Message[setref::refprop,$herr1@ToString[]];  (*   Message the RefProp error string          *)
            Return[{"ierr"->ierr}]                       (*   Return the error code                     *)
        ];
        If[ierr < 0,                                     (* IF non-zero warning code returned           *)
            Message[setref::refprop,$herr1@ToString[]];  (*   Message the RefProp warning string        *)
        ];
        If[hRef === "???",                               (* ELSE IF Current Ref. State requested        *)
            href = StringTrim[$href1@ToString[]];        (*   Extract and return ref state              *)
            If[href === "???",
                Return[StringTrim[$herr1@ToString[]]],   (*   RP10+ uses herr                            *)
                Return[href]                             (*   RP9 uses href                             *)
            ];
        ];
        If[ixFlag == 1,                                  (* ELSE IF setting for pure fluid              *)
            Message[setref::PurSet,$href1@ToString[]],   (*   Message pure fluid confirmation           *)
            Message[setref::MixSet,$href1@ToString[]]    (*   Message mixtures confirmation             *)
        ];                                               (* ELSE                      *)
    ]


(* ::SubSection:: *)
(* Utility Functions *)

(*************************************************************************************************************************)
(* Utility Functions                                                                                                     *)
(*************************************************************************************************************************)

getversion[]:=
   Module[{iversion,vMinor,vPatch,vRev},
      If[$DLL,
         iversion = setup[{"nitrogen"}, -1,"DEF"];       (* Call setup to retrieve RefProp version number (ncomp = -1)   *)
         If[iversion>1000,                               (* Returned ierr value is version number * 10000                *)
            $vMajor=Quotient[iversion,10000];            (*     Strip off and store major version globally for later use *)
            iversion=iversion-$vMajor*10000;
            vMinor=Quotient[iversion,1000];              (*     Get minor version for version string                     *)
            iversion=iversion-vMinor*1000;
            vPatch=Quotient[iversion,100];               (*     Get patch version for version string                     *)
            vRev=iversion-vPatch*100;                    (*     Get patch revision for version string                    *)
            $RPVersion=TextString[$vMajor] <> "." <> TextString[vMinor] <> "." <> TextString[vPatch] <> "." <> TextString[vRev];
         ],
      (* ELSE *)                                         (* Otherwise, value returned was a real error code              *)
         $vMajor=0;                                      (*     Major version is unknown (0), print failure message      *)
         Print[Framed["NIST RefProp DLL not found. Version cannot be determined.",Background-> LightYellow]]
      ];
      Return[StringTrim[$RPVersion]]                     (* Return the trimmed version string.                           *)
   ]

SetDLL::badDLL = "Invalid DLL selected [`1`]. \nMust select the 64-bit DLL from REFPROP (e.g. REFPRP64.DLL).";

SetDLL[permSet_Integer:0]:=
    Module[{hInit = $dllPath, hDLL},
        hDLL=SystemDialogInput["FileOpen", hInit, WindowTitle -> "Pick the REFPROP 64-bit DLL to use (REFPRP64.DLL)..."];
        If[(hDLL=!=$Canceled),                                                  (* IF user did NOT hit Cancel            *)
            If[StringMatchQ[hDLL, {"*REFPRP64.DLL"}, IgnoreCase -> True],       (*   If filename ends with *64.DLL       *)
                $dllPath = hDLL;                                                (*     Save file path to global          *)
                $DLL = FileExistsQ[$dllPath];                                   (*     has to exist since it was picked  *)
                SetPath[FileNameJoin[Drop[FileNameSplit[$dllPath],-1]],         (*     Set Fluids path as well,          *)
                    permSet]//Quiet;                                            (*       both permanently, and quietly   *)
                Print[Framed["Selected NIST REFPROP Version: " <>               (*     Show version of REFPROP DLL       *)
                      getversion[],FrameStyle->None,Background-> LightGreen]],
            (* ELSE *)                                                          (*   Otherwise                           *)
                Message[SetDLL::badDLL,hDLL];                                   (*     Warn user, bad DLL file selected  *)
            ];                                                                  (* Otherwise (Cancelled),                *)
        ];                                                                      (*   Do nothing                          *)
    ]

SetPath::badDIR = "Invalid fluids directory specification [`1`].";
SetPath::goodDIR = "INFO: Valid fluids directory found under [`1`].";
SetPath::saveDIR = "INFO: Fluids directory saved.";

SetPath[hpth_String:"",permSet_Integer:0]:=
    Module[{loadSETPATHdll,hFldPth, hSplit},
        If[StringLength[hpth]==0,                                                (* IF no path, call FileOpen Dialog     *)
            hFldPth=SystemDialogInput["Directory", $NISTPath, WindowTitle -> "Pick the REFPROP fluids folder..."],
        (* ELSE *)
            hFldPth=hpth                                                         (* OTHERWISE, use passed string (quiet) *)
        ];
        If[hFldPth === $Canceled, Return[]];                                     (* IF FileOpen Dialog canceled, abort   *)
        If[DirectoryQ[hFldPth],                                                  (* IF directory exists,                 *)
            hSplit = FileNameSplit[hFldPth];
            If[StringMatchQ[Last[hSplit],{"Fluids"},IgnoreCase->True],           (*   Remove Fluids\ folder from end     *)
                hFldPth = FileNameJoin[Drop[hSplit,-1]]];
                Message[SetPath::goodDIR,hFldPth];
        ,
            Message[SetPath::badDIR,hFldPth];                                    (* OTHERWISE, throw error & return      *)
            Return[]
        ];
        (* Print["Setting Fluids Path to:",hFldPth]; *)                          (*** DEBUG fluid path string            *)
        $NISTPath = hFldPth;                                                     (* Set NISTPath                         *)
        $mixturesCoeffPath=FileNameJoin[{$NISTPath,"fluids","HMX.BNC"}];         (* Set mix coefficients path also       *)
        loadSETPATHdll=DefineDLLFunction["SETPATHdll",$dllPath,                  (* Define SETPATHdll                    *)
                                         "void",                                 (*    No return values                  *)
                                         {"char*",                               (*    Input Fluid String                *)
                                         "long"}];                               (*    lenth of Fluid String buffer      *)
        loadSETPATHdll[hFldPth,$refpropcharlength];                              (* Call SETPATHdll                      *)
        ReleaseNETObject[loadSETPATHdll];                                        (* Release the DLL pointer from .NET    *)
        If[FileExistsQ[$hPaths]&&(permSet>0),                                    (* IF flagged,                          *)
          Export[$hPaths,{$dllPath<>","<>$NISTPath}];                            (*   update resource w/ current values  *)
          Message[SetPath::saveDIR];                                             (*   provide INFO sucess meessage       *)
        ];                                                                       (* Nothing is returned                  *)
    ]

ncBound[icomp_Integer]:=
   If[icomp<1,                  (* Bound icomp between first component...   *)
      Return[1],
   (* else *)
      If[icomp>$nc,             (* ...and current number of components, $nc *)
         Return[$nc],
      (* else *)
         Return[icomp]
      ]                         (* Otherwise use parameter value            *)
   ]

Options[xmass] = {SuppressUnits->False};
   
xmass[xmol_List:{1.0}, OptionsPattern[]]:=Module[{loadXMASSdll,xmol20,xkg20,x,xkg,wmix=0.},
    xmol20 = PadRight[Take[xmol, UpTo[$ncmax]], $ncmax];                  (* Pad mole fractions list to $ncmax (20)          *)
    xkg20 = MakeNETObject[PadRight[Take[{0.0}, UpTo[$ncmax]], $ncmax]];   (* Initialize xkg20 buffer for mole fractions      *)
    loadXMASSdll=DefineDLLFunction["XMASSdll",$dllPath,"void",{"double[]","double[]","double*"}];
    loadXMASSdll[xmol20,xkg20,wmix];
    ReleaseNETObject[loadXMASSdll];                                       (* Release the DLL pointer from .NET               *)
    x=NETObjectToExpression[xkg20];                                       (*    Extract the maxx frxn array                  *)
    xkg=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];                 (*    Remove any trailing zeroes                   *)
    ReleaseNETObject[xkg20];                                              (* Release the DLL pointer from .NET               *)
    If[OptionValue[SuppressUnits],
        Return[{"mass frxns"->xkg,"wmix [g/mol]"->wmix}],                 (* Return the mass frxn array and mixture mol. Wt. *)
    (*ELSE*)
        Return[{"mass frxns"->xkg,"wmix"->rpUnits[wmix,"MM"]}]            (* Return the mass frxn array and mixture mol. Wt. *)
    ]
];
    
Options[xmole] = {SuppressUnits->False};

xmole[xkg_List:{1.0}, OptionsPattern[]]:=Module[{loadXMOLEdll,xkg20,xmol20,x,xmol,wmix=0.},
    xkg20 = PadRight[Take[xkg, UpTo[$ncmax]], $ncmax];                    (* Pad mole fractions list to $ncmax (20)          *)
    xmol20 = MakeNETObject[PadRight[Take[{0.0}, UpTo[$ncmax]], $ncmax]];  (* Initialize xkg20 buffer for mole fractions      *)
    loadXMOLEdll=DefineDLLFunction["XMOLEdll",$dllPath,"void",{"double[]","double[]","double*"}];
    loadXMOLEdll[xkg20,xmol20,wmix];
    ReleaseNETObject[loadXMOLEdll];                                       (* Release the DLL pointer from .NET               *)
    x=NETObjectToExpression[xmol20];                                      (*    Extract the maxx frxn array                  *)
    xmol=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];                (*    Remove any trailing zeroes                   *)
    ReleaseNETObject[xmol20];                                             (* Release the DLL pointer from .NET               *)
    If[OptionValue[SuppressUnits],
        Return[{"mole frxns"->xmol,"wmix [g/mol]"->wmix}],                (* Return the mass frxn array and mixture mol. Wt. *)
    (*ELSE*)
        Return[{"mole frxns"->xmol,"wmix"->rpUnits[wmix,"MM"]}]           (* Return the mass frxn array and mixture mol. Wt. *)
    ]
];

purefld::fldset = "INFO: Component `1` (`2`) has been selected as a Pure Fluid.\n              Call with component 0 to reset mixture.";
purefld::mreset = "INFO: Mixture properties are being applied for all components.";
purefld::ierror = "ERROR: Illegal fluid component specified.";

purefld[icomp_Integer:0]:=
    Module[{loadPUREFLDdll,ic},
        ic = 0;
        If[icomp < 0, Message[purefluid::ierror];Return[]]; 
        If[icomp > 0,                               (* IF not resetting mixture properties          *)
            ic = ncBound[icomp]];                   (*    Bound icomp between 1 and $nc             *)
        loadPUREFLDdll=DefineDLLFunction["PUREFLDdll",$dllPath,"void",{"long*"}];
        loadPUREFLDdll[ic];                         (* Call PUREFLUIDdll with component ic          *)
        ReleaseNETObject[loadPUREFLDdll];           (* Release .NET DLL Object                      *)
        If[ic>0,                                    (* IF setting pure fluid comonent               *)
            Message[purefld::fldset,ic,name[ic]]    (*    Inform component and name being set       *)
        ,                                           (* ELSE - Resetting mixture properties          *)
            Message[purefld::mreset]                (*    Return a quantity with units              *)
        ];
    ]

    
(* ::SubSection:: *)
(* Information *)

(*************************************************************************************************************************)
(* Fluid Information Functions                                                                                           *)
(*************************************************************************************************************************)

Options[info] = {SuppressUnits->False};

info[icomp_Integer:1, OptionsPattern[]]:=            (* Overloaded info function: takes no parameters for single fluid cases (icomp=1) *)
    Module[{loadINFOdll,ic,wmm=0.,ttp=0.,tnbp=0.,tc=0.,pc=0.,dc=0.,zc=0.,acf=0.,dip=0.,rgas=0.},
        (* ic=ncBound[icomp];    *)                       (* Bound icomp between 1 and $nc                *)
        ic = icomp;
        loadINFOdll=DefineDLLFunction["INFOdll",$dllPath,"void",{"long*","double*","double*","double*","double*","double*", "double*","double*","double*","double*","double*"}];
        loadINFOdll[ic,wmm,ttp,tnbp,tc,pc,dc,zc,acf,dip,rgas];
        ReleaseNETObject[loadINFOdll];               (* Release the DLL pointer from .NET            *)
        If[OptionValue[SuppressUnits],
            Return[{"wmm [g/mol]"->wmm,              (* Unitless return rules:*)
                    "Ttriple [K]"->ttp,
                    "Tnbpt [K]"->tnbp,
                    "Tcrit [K]"->tc,
                    "Pcrit [kPa]"->pc,
                    "Dcrit [mol/L]"->dc,
                    "Zcrit"->zc,
                    "acf"->acf,
                    "dip [debye]"->dip,
                    "Rgas [J/mol-K]"->rgas}]
        , (* ELSE *)
            Return[{"wmm"->rpUnits[wmm,"MM"],
                    "Ttriple"->rpUnits[ttp,"T"],
                    "Tnbpt"->rpUnits[tnbp,"T"],
                    "Tcrit"->rpUnits[tc,"T"],
                    "Pcrit"->rpUnits[pc,"P"],
                    "Dcrit"->rpUnits[dc,"D"],
                    "Zcrit"->zc,
                    "acf"->acf,
                    "dip"->rpUnits[dip,"DM"],
                    "Rgas"->rpUnits[rgas,"S"]}]
        ]
    ]

Options[wmoli] = {SuppressUnits->False};

wmoli[icomp_Integer:1, OptionsPattern[]]:=       (* Overloaded wmoli function: 
                                                    Takes no parameters for single fluid cases (icomp=1)
                                                    Setting option SuppressUnits->True will return unitless values *)
    Module[{loadWMOLIdll,ic,wmm=0.,wm=0.},
        ic = ncBound[icomp];                     (* Bound icomp between 1 and $nc                *)
        loadWMOLIdll=DefineDLLFunction["WMOLIdll",$dllPath,"double",{"long*","double*"}];
        wmm = loadWMOLIdll[ic,wm];
        ReleaseNETObject[loadWMOLIdll];          (* Release .NET DLL Object                      *)
        If[OptionValue[SuppressUnits],           (* IF SupressUnits option is true               *)
            Return[{"wmm [g/mol]"->wmm}]         (*    Return unitless value                     *)
        ,                                        (* ELSE                                         *)
            Return[{"wmm"->rpUnits[wmm,"MM"]}]   (*    Return a quantity with units              *)
        ]
    ]

Options[wmol] = {SuppressUnits->False};           (* Allow user to suppress unit quantities      *)

wmol[z_List:{1.0}, OptionsPattern[]]:=            (* Overloaded wmol function: 
                                                     Takes no parameters for single fluid cases ( z = {1.0} )
                                                     Setting option SuppressUnits->True will return unitless values  *)
    Module[{loadWMOLdll,z20,wmm=0.,wm=0.},
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];   (* Pad mole fractions list to $ncmax (20) *)
        loadWMOLdll=DefineDLLFunction["WMOLdll",$dllPath,"double",{"double[]","double*"}];
        wmm = loadWMOLdll[z20,wm];                (* Get fluid/mixture molar mass                *)
        ReleaseNETObject[loadWMOLdll];            (* Release .NET DLL Object                     *)
        If[OptionValue[SuppressUnits],            (* IF SupressUnits option is true              *)
            Return[{"wmm [g/mol]"->wmm}]          (*     Return unitless value                   *)
        ,                                         (* ELSE                                        *)
            Return[{"wmm"->rpUnits[wmm,"MM"]}]    (*     Return a quantity with units            *)
        ]
    ]

name0[icomp_Integer,iname_Integer]:=      (* Return names strings for components.  Not callable directly by user. *)
      Module[{loadNAMEdll,hRet},
         $hname1@Clear[];                 (* Clear the StringBuilder if it already holds a value *)
         $hlong1@Clear[];                 (* Clear the StringBuilder if it already holds a value *)
         $hcasn1@Clear[];                 (* Clear the StringBuilder if it already holds a value *)
         loadNAMEdll=DefineDLLFunction["NAMEdll",$dllPath,"void",{"long*","System.Text.StringBuilder","System.Text.StringBuilder","System.Text.StringBuilder","long","long", "long"}];
         loadNAMEdll[icomp,$hname1,$hlong1,$hcasn1,$hnamlength,$hnam80length,$hcasnlength];  (* Get fluid names *)
         ReleaseNETObject[loadNAMEdll];   (* Release .NET DLL Object                             *)
         Switch[iname,                    (* Return the requested name string from calling fn    *)
            1, hRet = StringTrim[$hname1@ToString[]],
            2, hRet = StringTrim[$hlong1@ToString[]],
            3, hRet = StringTrim[$hcasn1@ToString[]]
         ];
         If[$vMajor<10,                                          (* Clean up REFPROP 9 strings   *)
             hRet = First[StringSplit[hRet,"  "]];                (* Remove any non-Characters    *)
         ];
         Return[hRet];
      ]

name[icomp_Integer:1]:=
    Module[{},
        name0[icomp,1]
    ]

longname[icomp_Integer:1]:=
    Module[{},
        name0[icomp,2]
    ]

casn[icomp_Integer:1]:=
    Module[{},
        name0[icomp,3]
    ]    

Options[limits] = {SuppressUnits->False};         (* Allow user to suppress unit quantities                         *)

limits[htype_String:"EOS",                        (* Overloaded limits function:                                    *)
       z_List:{1.0},                              (* Takes no composition for single fluid cases (z={1.0}).         *)
       OptionsPattern[]]:=                        (* No htype defaults to "EOS".                                    *)
                                                  (* Setting option SuppressUnits->True will return unitless values *)
    Module[{loadLIMITSdll,z20,tmin=0.,tmax=0.,dmax=0.,pmax=0.},
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];  (* Trim component list and pad to $ncmax elems *) 
        loadLIMITSdll=DefineDLLFunction["LIMITSdll",$dllPath,"void",{"char*","double[]","double*","double*","double*","double*","long"}];
        loadLIMITSdll[htype,z20,tmin,tmax,dmax,pmax,$referencelength];  (* Call LIMISdll  *)
        ReleaseNETObject[loadLIMITSdll];          (* Release .NET DLL Object            *)
        If[OptionValue[SuppressUnits],            (* IF SupressUnits option is true               *)
            Return[{"Tmin [K]"->tmin,                (*    Unitless return rules: *)
                    "Tmax [K]"->tmax,
                    "Dmax [mol/L]"->dmax,
                    "Pmax [kPa]"->pmax}]
        ,                                                            (* ELSE *)
            Return[{"Tmin"->rpUnits[tmin,"T"],  (* Return Quantities with units *)
                    "Tmax"->rpUnits[tmax,"T"],
                    "Dmax"->rpUnits[dmax,"D"],
                    "Pmax"->rpUnits[pmax,"P"]}]
        ]
    ]

SetAttributes[{info, wmoli, name, longname, casn}, {Listable}];  (* Can be made Listable since don't take a z List.      *)

(* ::SubSection:: *)
(* Single Phase *)

(*************************************************************************************************************************)
(* Single Phase Property Functions                                                                                       *)
(*************************************************************************************************************************)

tprho[t_,p_,z_List:{1.0},kph_Integer:1,D_:0.0,Dq_:0.0*mol/L]:=
    Module[{loadTPRHOdll,z20,kguess,ierr=0,Dmolar,suFlag},
        If[mixedUnits[t,p],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];     (* Trim mole fraction list and pad with zeroes to 20 elements   *)
        Dmolar = rpUnits[D] + rpUnits[Dq];
        If[Dmolar==0.0, kguess = 1, kguess = 0];         (* If D is provided, non-zero, set initial guesss flag          *)
        $herr1@Clear[];                                  (* Clear the error message buffer in case one comes back        *)
        loadTPRHOdll=DefineDLLFunction["TPRHOdll",$dllPath,"void",{"double*","double*","double[]","long*","long*","double*","long*","System.Text.StringBuilder","long"}];
        loadTPRHOdll[rpUnits[t],rpUnits[p],z20,kph,kguess,Dmolar,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadTPRHOdll];                  (* Release the DLL pointer object                               *)
        If[ierr>0,                                       (* IF non-zero error code returned                              *)
            Message[tprho::refprop,$herr1@ToString[]];   (*   Message the RefProp error string                           *)
            Return[{"ierr"->ierr}]                       (* Return the error code                                      *)
        ];                                               (* ELSE                                                         *)
        If[suFlag,                                       (*     IF SupressUnits option is true                           *)
            Return[{"Dmolar [mol/L]"->Dmolar}]           (*         Return unitless molar density                        *)
        ,                                                (*     ELSE                                                     *)
            Return[{"Dmolar"->rpUnits[Dmolar,"D"]}]      (*         Return a quantity with units                         *)
        ]
   ]

(* Calculate pressure (p), internal energy (e), enthalpy (h), entropy (s), isochoric (cv) and isobaric (cp) heat         *)
(* capacities, speed of sound (w), and Joule-Thomson coefficient (hjt) from t,d,z. Subroutines THERM2 and THERM3 contain *)
(* more properties, see PROP_SUB.FOR.                                                                                    *)
(* returns P,E,H,S,Cv,Cp,W,hjt                                                                                           *)
therm[t_,d_,z_List:{1.0}]:=
    Module[{loadTHERMdll,z20,p=0.,e=0.,h=0.,s=0.,cv=0.,cp=0.,w=0.,hjt=0.,suFlag},
        If[mixedUnits[t,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];     (* Trim mole fraction list and pad with zeroes to 20 elements   *)
        loadTHERMdll=DefineDLLFunction["THERMdll",$dllPath,"void",{"double*","double*","double[]","double*","double*","double*","double*","double*","double*","double*","double*"}];
        loadTHERMdll[rpUnits[t],rpUnits[d],z20,p,e,h,s,cv,cp,w,hjt];
        ReleaseNETObject[loadTHERMdll];                  (* Release the DLL pointer object                               *)
        
        If[suFlag,                                       (*     IF SupressUnits option is true                           *)
                                                         (*         Return unitless molar density                        *)
            Return[{"P [kPa]"->p,
                    "U [J/mol]"->e,
                    "H [J/mol]"->h,
                    "S [J/mol-K]"->s,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w,
                    "hjt [K/kPa]"->hjt}]
        ,                                                (*     ELSE                                                     *)
                                                         (*         Return a quantity with units                         *)
            Return[{"P"->rpUnits[p,"P"],
                    "U"->rpUnits[e,"E"],
                    "H"->rpUnits[h,"E"],
                    "S"->rpUnits[s,"S"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"W"],
                    "hjt"->rpUnits[hjt,"JT"]}]
        ]
    ]

press[t_,d_,z_List:{1.0}]:=
    Module[{loadPRESSdll,z20,p=0.,suFlag},
        If[mixedUnits[t,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];
        loadPRESSdll=DefineDLLFunction["PRESSdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
        loadPRESSdll[rpUnits[t],rpUnits[d],z20,p];
        ReleaseNETObject[loadPRESSdll];
        If[suFlag,                                        (*  IF SupressUnits option is true                    *)
            Return[{"P [kPa]"->p}]                        (*      Return unitless molar density                 *)
        ,                                                 (*  ELSE                                              *)
            Return[{"P"->rpUnits[p,"P"]}]                 (*      Return a quantity with units                  *)
        ]
    ]

enthal[t_,d_,z_List:{1.0}]:=
    Module[{loadENTHALdll,z20,h=0.,suFlag},
        If[mixedUnits[t,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];
        loadENTHALdll=DefineDLLFunction["ENTHALdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
        loadENTHALdll[rpUnits[t],rpUnits[d],z20,h];
        ReleaseNETObject[loadENTHALdll];
        If[suFlag,                                       (*  IF SupressUnits option is true                    *)
            Return[{"H [J/mol]"->h}]                     (*      Return unitless molar density                 *)
        ,                                                (*  ELSE                                              *)
            Return[{"H"->rpUnits[h,"E"]}]                (*      Return a quantity with units                  *)
        ]
    ]

entro[t_,d_,z_List:{1.0}]:=
    Module[{loadENTROdll,z20,s=0.,suFlag},
        If[mixedUnits[t,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];
        loadENTROdll=DefineDLLFunction["ENTROdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
        loadENTROdll[rpUnits[t],rpUnits[d],z20,s];
        ReleaseNETObject[loadENTROdll];
        If[suFlag,                                       (*  IF SupressUnits option is true                    *)
            Return[{"S [J/mol-K]"->s}]                   (*      Return unitless molar density                 *)
        ,                                                (*  ELSE                                              *)
            Return[{"S"->rpUnits[s,"S"]}]                (*      Return a quantity with units                  *)
        ]
    ]

cvcp[t_,d_,z_List:{1.0}]:=
    Module[{loadCVCPdll,z20,cv=0.,cp=0.,suFlag},
        If[mixedUnits[t,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];
        loadCVCPdll=DefineDLLFunction["CVCPdll",$dllPath,"void",{"double*","double*","double[]","double*","double*"}];
        loadCVCPdll[rpUnits[t],rpUnits[d],z20,cv,cp];
        ReleaseNETObject[loadCVCPdll];
        If[suFlag,                                        (*  IF SupressUnits option is true                  *)
            Return[{"Cv [J/mol-K]"->cv,                   (*      Return unitless molar density               *)
                    "Cp [J/mol-K]"->cp}]
        ,                                                 (*  ELSE                                            *)
            Return[{"Cv"->rpUnits[cv,"S"],                (*      Return a quantity with units                *)
                    "Cp"->rpUnits[cp,"S"]}]
        ]
    ]

(* ::SubSection:: *)
(* Saturation *)

(*************************************************************************************************************************)
(* Saturation State Functions                                                                                            *)
(*************************************************************************************************************************)

satspln::splines = "INFO: Successfully created mixture phase boundary splines.";

satspln[z_List:{1.0}]:=
    Module[{loadSATSPLNdll,z20,ierr=0},
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];     (* Trim mole fraction list and pad with zeroes to 20 elements   *)
        $herr1@Clear[];                                  (* Clear the error message buffer in case one comes back        *)
        loadSATSPLNdll=DefineDLLFunction["SATSPLNdll",$dllPath,"void",{"double[]","long*","System.Text.StringBuilder","long"}];
        loadSATSPLNdll[z20,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSATSPLNdll];                (* Release the DLL pointer object                               *)
        If[ierr>0,                                       (* IF non-zero error code returned                              *)
            Message[satspln::refprop,$herr1@ToString[]], (*   Message the RefProp error string                           *)
                                                         (* ELSE                                                         *)
            Message[satspln::splines];                   (*   Issue confirmation Message that can be Quiet-ed            *)
        ];                                               (* No return values                                             *)
   ]

Options[maxp] = {SuppressUnits->False};         (* Allow user to suppress unit quantities                         *)

maxp[z_List:{1.0}, OptionsPattern[]]:=
    Module[{loadMAXPdll, z20, ierr=0, Tm=0, Pm=0, Dm=0},
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];     (* Trim mole fraction list and pad with zeroes to 20 elements   *)
        $herr1@Clear[];                                  (* Clear the error message buffer in case one comes back        *)
        loadMAXPdll=DefineDLLFunction["MAXPdll",$dllPath,"void",{"double[]","double*","double*","double*","long*","System.Text.StringBuilder","long"}];
        loadMAXPdll[z20,Tm,Pm,Dm,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadMAXPdll];                   (* Release the DLL pointer object                               *)
        If[ierr != 0,                                    (* IF non-zero error code returned                              *)
            Message[maxp::refprop,$herr1@ToString[]]     (*   Message the RefProp error string                           *)
        ];
        If[OptionValue[SuppressUnits],                                            (* IF SupressUnits option is true      *)
            Return[{"\!\(\*SubscriptBox[\(T\), \(maxP\)]\) [K]"->Tm,                (*    Unitless return rules:           *)
                    "\!\(\*SubscriptBox[\(P\), \(maxP\)]\) [kPa]"->Pm,
                    "\!\(\*SubscriptBox[\(D\), \(maxP\)]\) [mol/L]"->Dm}]
        ,                                                                         (* ELSE                                *)
            Return[{"\!\(\*SubscriptBox[\(T\), \(maxP\)]\)"->rpUnits[Tm,"T"],        (* Return Quantities with units        *)
                    "\!\(\*SubscriptBox[\(P\), \(maxP\)]\)"->rpUnits[Pm,"P"],
                    "\!\(\*SubscriptBox[\(D\), \(maxP\)]\)"->rpUnits[Dm,"D"]}]
        ]
   ]

Options[maxt] = {SuppressUnits->False};         (* Allow user to suppress unit quantities                         *)

maxt[z_List:{1.0}, OptionsPattern[]]:=
    Module[{loadMAXTdll, z20, ierr=0, Tm=0, Pm=0, Dm=0},
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];     (* Trim mole fraction list and pad with zeroes to 20 elements   *)
        $herr1@Clear[];                                  (* Clear the error message buffer in case one comes back        *)
        loadMAXTdll=DefineDLLFunction["MAXTdll",$dllPath,"void",{"double[]","double*","double*","double*","long*","System.Text.StringBuilder","long"}];
        loadMAXTdll[z20,Tm,Pm,Dm,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadMAXTdll];                   (* Release the DLL pointer object                               *)
        If[ierr != 0,                                    (* IF non-zero error code returned                              *)
            Message[maxt::refprop,$herr1@ToString[]]     (*   Message the RefProp error string                           *)
        ];
        If[OptionValue[SuppressUnits],                                            (* IF SupressUnits option is true      *)
            Return[{"\!\(\*SubscriptBox[\(T\), \(maxT\)]\) [K]"->Tm,                (*    Unitless return rules:           *)
                    "\!\(\*SubscriptBox[\(P\), \(maxT\)]\) [kPa]"->Pm,
                    "\!\(\*SubscriptBox[\(D\), \(maxT\)]\) [mol/L]"->Dm}]
        ,                                                                         (* ELSE                                *)
            Return[{"\!\(\*SubscriptBox[\(T\), \(maxT\)]\)"->rpUnits[Tm,"T"],        (* Return Quantities with units        *)
                    "\!\(\*SubscriptBox[\(P\), \(maxT\)]\)"->rpUnits[Pm,"P"],
                    "\!\(\*SubscriptBox[\(D\), \(maxT\)]\)"->rpUnits[Dm,"D"]}]
        ]
   ]

Options[critp] = {SuppressUnits->False};                (* Allow user to suppress unit quantities                         *)

critp[z_List:{1.0}, OptionsPattern[]]:=
    Module[{loadCRITPdll, z20, ierr=0, Tc=0, Pc=0, Dc=0},
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];      (* Trim mole fraction list and pad with zeroes to 20 elements   *)
        $herr1@Clear[];                                   (* Clear the error message buffer in case one comes back        *)
        loadCRITPdll=DefineDLLFunction["CRITPdll",$dllPath,"void",{"double[]","double*","double*","double*","long*","System.Text.StringBuilder","long"}];
        loadCRITPdll[z20,Tc,Pc,Dc,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadCRITPdll];                   (* Release the DLL pointer object                               *)
        If[ierr != 0,                                     (* IF non-zero error code returned                              *)
            Message[critp::refprop,$herr1@ToString[]]     (*   Message the RefProp error string                           *)
        ];
        If[OptionValue[SuppressUnits],                                             (* IF SupressUnits option is true      *)
            Return[{"\!\(\*SubscriptBox[\(T\), \(C\)]\) [K]"->Tc,                     (*    Unitless return rules:           *)
                    "\!\(\*SubscriptBox[\(P\), \(C\)]\) [kPa]"->Pc,
                    "\!\(\*SubscriptBox[\(D\), \(C\)]\) [mol/L]"->Dc}]
        ,                                                                          (* ELSE                                *)
            Return[{"\!\(\*SubscriptBox[\(T\), \(C\)]\)"->rpUnits[Tc,"T"],         (* Return Quantities with units        *)
                    "\!\(\*SubscriptBox[\(P\), \(C\)]\)"->rpUnits[Pc,"P"],
                    "\!\(\*SubscriptBox[\(D\), \(C\)]\)"->rpUnits[Dc,"D"]}]
        ]
   ]


satp[p_,z_List:{1.0},kph_Integer:1]:=
    Module[{loadSATPdll,z20,i,t=0.,dl=0.,dv=0.,x,xliq,xvap,suFlag,ierr=0},
        suFlag=!QuantityQ[p];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadSATPdll=DefineDLLFunction["SATPdll",$dllPath,"void",{"double*","double[]","long*","double*","double*","double*","double[]","double[]","long*","System.Text.StringBuilder","long"}];
        loadSATPdll[rpUnits[p],z20,kph,t,dl,dv,$xliq1,$xvap1,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSATPdll];                                   (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[satp::refprop,$herr1@ToString[]];                    (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];
        If[suFlag,                                                       (*  IF SupressUnits option is true              *)
            Return[{"Tsat [K]"->t,                                  (*      Return unitless results                 *)
                    "Dliq [mol/L]"->dl,
                    "Dvap [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap}]
        ,                                                                (*  ELSE                                        *)
            Return[{"Tsat"->rpUnits[t,"T"],                              (*      Return a quantity with units            *)
                    "Dliq"->rpUnits[dl,"D"],
                    "Dvap"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap}]
        ]
    ]

satt[t_,z_List:{1.0},kph_Integer:1]:=
    Module[{loadSATTdll,z20,i,x,xliq,xvap,suFlag,ierr=0,p=0.,dl=0.,dv=0.},
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadSATTdll=DefineDLLFunction["SATTdll",$dllPath,"void",{"double*","double[]","long*","double*","double*","double*","double[]","double[]","long*","System.Text.StringBuilder","long"}];
        loadSATTdll[rpUnits[t],z20,kph,p,dl,dv,$xliq1,$xvap1,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSATTdll];                                   (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[satt::refprop,$herr1@ToString[]];                    (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes from the list  *)
        ];
        If[suFlag,                                                       (*  IF SupressUnits option is true              *)
            Return[{"Psat [kPa]"->p,                                     (*      Return unitless results                 *)
                    "Dliq [mol/L]"->dl,
                    "Dvap [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap}]
        ,                                                                (*  ELSE                                        *)
            Return[{"Psat"->rpUnits[p,"P"],                              (*      Return a quantity with units            *)
                    "Dliq"->rpUnits[dl,"D"],
                    "Dvap"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap}]
        ]
    ]

sats[s_,z_List:{1.0},kph_Integer:0]:=
    Module[{loadSATSdll,z20,nroot=0,k1=0,T1=0.,P1=0.,D1=0.,k2=0,T2=0.,P2=0.,D2=0.,k3=0,T3=0.,P3=0.,D3=0.,suFlag,ierr=0},
        suFlag=!QuantityQ[s];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadSATSdll=DefineDLLFunction["SATSdll",$dllPath,"void",         (* No return value                              *)
                {"double*","double[]","long*","long*",                   (* h, z[20], kph, nroot,                        *)
                "long*","double*","double*","double*",                   (* k1, T1, P1, D1,                              *)
                "long*","double*","double*","double*",                   (* k2, T2, P2, D2,                              *)
                "long*","double*","double*","double*",                   (* k3, T3, P3, D3,                              *)
                "long*","System.Text.StringBuilder","long"}];            (* ierr, herr, herr_length                      *)
        loadSATSdll[rpUnits[s],z20,kph,nroot,
                    k1,T1,P1,D1,
                    k2,T2,P2,D2,
                    k3,T3,P3,D3,
                    ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSATSdll];                                   (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[sats::refprop,$herr1@ToString[]];                    (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];
        If[suFlag,                                                       (*  IF SupressUnits option is true              *)
            Return[{"nroot"->nroot,                                      (*      Return unitless results                 *)
                    "k1"->k1,
                    "T1 [K]"->T1,
                    "P1 [kPa]"->P1,
                    "D1 [mol/L]"->D1,
                    "k2"->k2,
                    "T2 [K]"->T2,
                    "P2 [kPa]"->P2,
                    "D2 [mol/L]"->D2,
                    "k3"->k3,
                    "T3 [K]"->T3,
                    "P3 [kPa]"->P3,
                    "D3 [mol/L]"->D3
                    }]
        ,                                                                (*  ELSE                                        *)
            Return[{"nroot"->nroot,                                      (*      Return results as Quantities            *)
                    "k1"->k1,
                    "T1 [K]"->rpUnits[T1,"T"],
                    "P1 [kPa]"->rpUnits[P1,"P"],
                    "D1 [mol/L]"->rpUnits[D1,"D"],
                    "k2"->k2,
                    "T2 [K]"->rpUnits[T2,"T"],
                    "P2 [kPa]"->rpUnits[P2,"P"],
                    "D2 [mol/L]"->rpUnits[D2,"D"],
                    "k3"->k3,
                    "T3 [K]"->rpUnits[T3,"T"],
                    "P3 [kPa]"->rpUnits[P3,"P"],
                    "D3 [mol/L]"->rpUnits[D3,"D"]
                    }]
        ]
    ]

sath[h_,z_List:{1.0},kph_Integer:0]:=
    Module[{loadSATHdll,z20,nroot=1,k1=0,T1=0.,P1=0.,D1=0.,k2=0,T2=0.,P2=0.,D2=0.,suFlag,ierr=0},
        suFlag=!QuantityQ[h];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadSATHdll=DefineDLLFunction["SATHdll",$dllPath,"void",         (* No return value                              *)
                {"double*","double[]","long*","long*",                   (* h, z[20], kph, nroot,                        *)
                "long*","double*","double*","double*",                   (* k1, T1, P1, D1,                              *)
                "long*","double*","double*","double*",                   (* k2, T2, P2, D2,                              *)
                "long*","System.Text.StringBuilder","long"}];            (* ierr, herr, herr_length                      *)
        loadSATHdll[rpUnits[h],z20,kph,nroot,
                    k1,T1,P1,D1,
                    k2,T2,P2,D2,
                    ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSATHdll];                                   (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[sath::refprop,$herr1@ToString[]];                    (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];
        If[suFlag,                                                       (*  IF SupressUnits option is true              *)
            Return[{"nroot"->nroot,                                      (*      Return unitless results                 *)
                    "k1"->k1,
                    "T1 [K]"->T1,
                    "P1 [kPa]"->P1,
                    "D1 [mol/L]"->D1,
                    "k2"->k2,
                    "T2 [K]"->T2,
                    "P2 [kPa]"->P2,
                    "D2 [mol/L]"->D2
                    }]
        ,                                                                (*  ELSE                                        *)
            Return[{"nroot"->nroot,                                      (*      Return results as Quantities            *)
                    "k1"->k1,
                    "T1 [K]"->rpUnits[T1,"T"],
                    "P1 [kPa]"->rpUnits[P1,"P"],
                    "D1 [mol/L]"->rpUnits[D1,"D"],
                    "k2"->k2,
                    "T2 [K]"->rpUnits[T2,"T"],
                    "P2 [kPa]"->rpUnits[P2,"P"],
                    "D2 [mol/L]"->rpUnits[D2,"D"]
                    }]
        ]
    ]

(* ::Subsection:: *)
(* 2-Phase *)

(*************************************************************************************************************************)
(* General Flash Calculations                                                                                            *)
(*************************************************************************************************************************)
(*   First two characters specify the inputs needed.                                                                     *)
(*   Composiiton parameter, z, is optional; assumes pure fluid if ommitted.                                              *)
(*   Returns: [T|P|D], Dliq, Dvap, xliq, yvap, Q, U, H, S, Cv, Cp, W                                                     *)
(*   NOTE 1: 1 or 2-phase. Use xxflsh1[] if known to be 1-phase                                                          *)
(*   NOTE 2: tpflsh cannot determine vapor quality for pure fluids or non-azeotrope mixtures                             *)
(*   NOTE 3: tprho function much faster for single-phase only calls than tpflsh                                          *)
(*************************************************************************************************************************)

tpflsh[T_,P_,z_List:{1.0}]:=
    Module[{loadTPFLSHdll,z20,x,xliq,xvap,suFlag,ierr=0,d=0.,dl=0.,dv=0.,q=0.,e=0.,h=0.,s=0.,cv=0.,cp=0.,w=0.},
        If[mixedUnits[T,P],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[T];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call TPFLSHdll function *)
        loadTPFLSHdll=DefineDLLFunction["TPFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*",
            "double*","double[]","double[]","double*","double*","double*","double*","double*","double*","double*","long*",
            "System.Text.StringBuilder","long"}];
        loadTPFLSHdll[rpUnits[T],rpUnits[P],z20,d,dl,dv,$xliq1,$xvap1,q,e,h,s,cv,cp,w,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadTPFLSHdll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[tpflsh::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"D [mol/L]"->d,                                      (*       Return unitless results                *)
                    "Dl [mol/L]"->dl,
                    "Dv [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U [J/mol]"->e,
                    "H [J/mol]"->h,
                    "S [J/mol-K]"->s,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w}
            ]
        ,                                                                (*   ELSE                                       *)
            Return[{"D"->rpUnits[d,"D"],                                 (*       Return a quantity with units           *)
                    "Dl"->rpUnits[dl,"D"],
                    "Dv"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U"->rpUnits[e,"E"],
                    "H"->rpUnits[h,"E"],
                    "S"->rpUnits[s,"S"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"W"]
                    }
            ]
        ]
    ]

tdflsh[T_,d_,z_List:{1.0}]:=
    Module[{loadTDFLSHdll,z20,x,xliq,xvap,suFlag,ierr=0,p=0.,dl=0.,dv=0.,q=0.,e=0.,h=0.,s=0.,cv=0.,cp=0.,w=0.},
        If[mixedUnits[T,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[T];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call TDFLSHdll function *)
        loadTDFLSHdll=DefineDLLFunction["TDFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*",
            "double*","double[]","double[]","double*","double*","double*","double*","double*","double*","double*","long*",
            "System.Text.StringBuilder","long"}];
        loadTDFLSHdll[rpUnits[T],rpUnits[d],z20,p,dl,dv,$xliq1,$xvap1,q,e,h,s,cv,cp,w,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadTDFLSHdll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[tdflsh::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"P [kPa]"->p,                                        (*       Return unitless results                *)
                    "Dl [mol/L]"->dl,
                    "Dv [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U [J/mol]"->e,
                    "H [J/mol]"->h,
                    "S [J/mol-K]"->s,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w}
            ]
        ,                                                                (*   ELSE                                       *)
            Return[{"P"->rpUnits[p,"P"],                                 (*       Return a quantity with units           *)
                    "Dl"->rpUnits[dl,"D"],
                    "Dv"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U"->rpUnits[e,"E"],
                    "H"->rpUnits[h,"E"],
                    "S"->rpUnits[s,"S"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"V"]
                    }
            ]
        ]
    ]


thflsh[T_,H_,z_List:{1.0},kr_Integer:1]:=
    Module[{loadTHFLSHdll,z20,x,xliq,xvap,suFlag,p=0.,d=0.,dl=0.,dv=0.,q=0.,e=0.,s=0.,cv=0.,cp=0.,w=0.,ierr=0},
        If[mixedUnits[T,H],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[T];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call THFLSHdll function *)
        loadTHFLSHdll=DefineDLLFunction["THFLSHdll",$dllPath,"void",{"double*","double*","double[]","long*","double*",
            "double*","double*","double*","double[]","double[]","double*","double*","double*","double*","double*","double*",
            "long*","System.Text.StringBuilder","long"}];
        loadTHFLSHdll[rpUnits[T],rpUnits[H],z20,kr,p,d,dl,dv,$xliq1,$xvap1,q,e,s,cv,cp,w,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadTHFLSHdll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[thflsh::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"P [kPa]"->p,                                        (*       Return unitless results                *)
                    "D [mol/L]"->d,
                    "Dl [mol/L]"->dl,
                    "Dv [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U [J/mol]"->e,
                    "S [J/mol-K]"->s,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w}
            ]
        ,                                                                (*   ELSE                                       *)
            Return[{"P"->rpUnits[p,"P"],                                 (*       Return a quantity with units           *)
                    "D"->rpUnits[d,"D"],
                    "Dl"->rpUnits[dl,"D"],
                    "Dv"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U"->rpUnits[e,"E"],
                    "S"->rpUnits[s,"S"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"V"]
                    }
            ]
        ]
    ]

tsflsh[T_,S_,z_List:{1.0},kr_Integer:1]:=
    Module[{loadTSFLSHdll,z20,x,xliq,xvap,suFlag,p=0.,d=0.,dl=0.,dv=0.,q=0.,e=0.,h=0.,cv=0.,cp=0.,w=0.,ierr=0},
        If[mixedUnits[T,S],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[T];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call TSFLSHdll function *)
        loadTSFLSHdll=DefineDLLFunction["TSFLSHdll",$dllPath,"void",{"double*","double*","double[]","long*","double*",
            "double*","double*","double*","double[]","double[]","double*","double*","double*","double*","double*","double*",
            "long*","System.Text.StringBuilder","long"}];
        loadTSFLSHdll[rpUnits[T],rpUnits[S],z20,kr,p,d,dl,dv,$xliq1,$xvap1,q,e,h,cv,cp,w,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadTSFLSHdll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[thflsh::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"P [kPa]"->p,                                        (*       Return unitless results                *)
                    "D [mol/L]"->d,
                    "Dl [mol/L]"->dl,
                    "Dv [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U [J/mol]"->e,
                    "H [J/mol-K]"->h,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w}
            ]
        ,                                                                (*   ELSE                                       *)
            Return[{"P"->rpUnits[p,"P"],                                 (*       Return a quantity with units           *)
                    "D"->rpUnits[d,"D"],
                    "Dl"->rpUnits[dl,"D"],
                    "Dv"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U"->rpUnits[e,"E"],
                    "H"->rpUnits[h,"H"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"V"]
                    }
            ]
        ]
    ]


pdflsh[P_,d_,z_List:{1.0}]:=
    Module[{loadPDFLSHdll,z20,x,xliq,xvap,suFlag,t=0.,dl=0.,dv=0.,q=0.,e=0.,h=0.,s=0.,cv=0.,cp=0.,w=0.,ierr=0},
        If[mixedUnits[P,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[P];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call TDFLSHdll function *)
        loadPDFLSHdll=DefineDLLFunction["PDFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*",
            "double*","double[]","double[]","double*","double*","double*","double*","double*","double*","double*","long*",
            "System.Text.StringBuilder","long"}];
        loadPDFLSHdll[rpUnits[P],rpUnits[d],z20,t,dl,dv,$xliq1,$xvap1,q,e,h,s,cv,cp,w,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadPDFLSHdll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[thflsh::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"T [K]"->t,                                          (*       Return unitless results                *)
                    "D [mol/L]"->d,
                    "Dl [mol/L]"->dl,
                    "Dv [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U [J/mol]"->e,
                    "H [J/mol-K]"->h,
                    "S [J/mol-K]"->s,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w}
            ]
        ,                                                                (*   ELSE                                       *)
            Return[{"T"->rpUnits[t,"T"],                                 (*       Return a quantity with units           *)
                    "D"->rpUnits[d,"D"],
                    "Dl"->rpUnits[dl,"D"],
                    "Dv"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U"->rpUnits[e,"E"],
                    "H"->rpUnits[h,"H"],
                    "S"->rpUnits[s,"S"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"V"]
                    }
            ]
        ]
    ]

phflsh[P_,H_,z_List:{1.0}]:=
    Module[{loadphflshDll,z20,xliq,xvap,suFlag,t=0.,d=0.,dl=0.,dv=0.,q=0.,e=0.,s=0.,cv=0.,cp=0.,w=0.,ierr=0},
        If[mixedUnits[P,H],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[P];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call PHFLSHdll function *)
        loadphflshDll=DefineDLLFunction["PHFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*",
            "double*","double*","double[]","double[]","double*","double*","double*","double*","double*","double*","long*",
            "System.Text.StringBuilder","long"}];
        loadphflshDll[rpUnits[P],rpUnits[H],z20,t,d,dl,dv,$xliq1,$xvap1,q,e,s,cv,cp,w,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadphflshDll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[thflsh::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"T [K]"->t,                                          (*       Return unitless results                *)
                    "D [mol/L]"->d,
                    "Dl [mol/L]"->dl,
                    "Dv [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U [J/mol]"->e,
                    "S [J/mol-K]"->s,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w}
            ]
        ,                                                                (*   ELSE                                       *)
            Return[{"T"->rpUnits[t,"T"],                                 (*       Return a quantity with units           *)
                    "D"->rpUnits[d,"D"],
                    "Dl"->rpUnits[dl,"D"],
                    "Dv"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U"->rpUnits[e,"E"],
                    "S"->rpUnits[s,"S"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"V"]
                    }
            ]
        ]
    ]

psflsh[P_,S_,z_List:{1.0}]:=
    Module[{loadPSFLSHdll,z20,x,xliq,xvap,t=0.,d=0.,dl=0.,dv=0.,q=0.,e=0.,h=0.,cv=0.,cp=0.,w=0.,ierr=0},
        If[mixedUnits[P,S],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[P];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                            (* Zero out the return mole fraction array      *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call PSFLSHdll function *)
        loadPSFLSHdll=DefineDLLFunction["PSFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*",
            "double*","double*","double[]","double[]","double*","double*","double*","double*","double*","double*","long*",
            "System.Text.StringBuilder","long"}];
        loadPSFLSHdll[rpUnits[P],rpUnits[S],z20,t,d,dl,dv,$xliq1,$xvap1,q,e,h,cv,cp,w,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadPSFLSHdll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[thflsh::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            x=NETObjectToExpression[$xliq1];                             (*    Extract the liquid mole frxn array        *)
            xliq=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];       (*    Remove any trailing zeroes                *)
            x=NETObjectToExpression[$xvap1];                             (*    Extract the vapor mole frxn array         *)
            xvap=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]]        (*    Remove any trailing zeroes                *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"T [K]"->t,                                        (*       Return unitless results                *)
                    "D [mol/L]"->d,
                    "Dl [mol/L]"->dl,
                    "Dv [mol/L]"->dv,
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U [J/mol]"->e,
                    "H [J/mol-K]"->h,
                    "Cv [J/mol-K]"->cv,
                    "Cp [J/mol-K]"->cp,
                    "W [m/s]"->w}
            ]
        ,                                                                (*   ELSE                                       *)
            Return[{"T"->rpUnits[t,"T"],                                 (*       Return a quantity with units           *)
                    "D"->rpUnits[d,"D"],
                    "Dl"->rpUnits[dl,"D"],
                    "Dv"->rpUnits[dv,"D"],
                    "xliq"->xliq,
                    "xvap"->xvap,
                    "Q"->q,
                    "U"->rpUnits[e,"E"],
                    "H"->rpUnits[h,"H"],
                    "Cv"->rpUnits[cv,"S"],
                    "Cp"->rpUnits[cp,"S"],
                    "W"->rpUnits[w,"V"]
                    }
            ]
        ]
    ]


(* ::SubSection:: *)
(* Transport Props *)

(****************************************************************************)
(* Transport Properties                                                     *)
(****************************************************************************)

(*Calculate viscosity (eta) and thermal conductivity (tcx)                                                               *)
trnprp[t_,d_,z_List:{1.0}]:=
    Module[{loadTRNPRPdll,z20,suFlag,mu=0.,k=0.,ierr=0},
        If[mixedUnits[t,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadTRNPRPdll=DefineDLLFunction["TRNPRPdll",$dllPath,"void",{"double*","double*","double[]","double*","double*",
            "long*","System.Text.StringBuilder","long"}];
        loadTRNPRPdll[rpUnits[t],rpUnits[d],z20,mu,k,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadTRNPRPdll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[trnprp::refprop,$herr1@ToString[]];                  (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"\[Mu] [\[Mu]Pa-s]"-> mu,"k [W/m-K]"-> k}]           (* Return unitless results     *)
        ,                                                                (*   ELSE                                       *)
            Return[{"\[Mu]"->rpUnits[mu,"MU"],"k"->rpUnits[k,"K"]}]      (*       Return a quantity with units           *)               
        ]
    ]

(* Calculate mixture surface tension at specified saturation temperature    *)
(* when saturation densities and compositions are already known. This is    *)
(* much faster than calculating them again usng surft[].                    *)
stn[t_,Dl_,Dv_,x_List:{1.0},y_List:{1.0}]:=
    Module[{loadSTNdll,x20,y20,suFlag,sigma=0.,ierr=0},
        If[mixedUnits[t,Dl],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.       *)
        If[mixedUnits[t,Dv],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.       *)
        suFlag=!QuantityQ[t];                             (* Set suppress units flag if inputs are non-quantities.       *)
        x20=PadRight[Take[x, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        y20=PadRight[Take[y, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call STNdll function *)
        loadSTNdll=DefineDLLFunction["STNdll",$dllPath,"void",{"double*","double*","double*","double[]","double[]","double*",
            "long*","System.Text.StringBuilder","long"}];
        loadSTNdll[rpUnits[t],rpUnits[Dl],rpUnits[Dv],x20,y20,sigma,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSTNdll];                                    (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[stn::refprop,$herr1@ToString[]];                     (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"\[Sigma] [N/m]"-> sigma}]                           (*       Return unitless results                *)
        ,                                                                (*   ELSE                                       *)
            Return[{"\[Sigma]"->rpUnits[sigma,"ST"]}]                    (*    Return a quantity with units        *)     
        ]
   ]

(* Calculate surface tension at specified saturation temperature *)
(* Calls SATT to get saturation densities.  If these are already *)
(* known, use stn[] instead and pass them in.                    *)

surft[t_,z_List:{1.0}]:=
    Module[{laodSURFTdll,z20,suFlag,dl=0.,sigma=0.,ierr=0},
        suFlag=!QuantityQ[t];                            (* Set suppress units flag if inputs are non-quantities.        *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        (* Define and call SURFTdll function *)
        laodSURFTdll=DefineDLLFunction["SURFTdll",$dllPath,"void",{"double*","double*","double[]","double*","long*","System.Text.StringBuilder","long"}];
        laodSURFTdll[rpUnits[t],dl,z20,sigma,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[laodSURFTdll];                                  (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[surft::refprop,$herr1@ToString[]];                   (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"\[Sigma] [N/m]"-> sigma}]                           (*       Return unitless results                *)
        ,                                                                (*   ELSE                                       *)
            Return[{"\[Sigma]"->rpUnits[sigma,"ST"]}]                    (*    Return a quantity with units        *)     
        ]
   ]

(* Calculate dielectric constant *)
dielec[t_,d_,z_List:{1.0}]:=
    Module[{dielecdlload,z20,dc=0.},
        If[mixedUnits[t,d],Return["Error"]];             (* Check for mixed Quantity/non-Quantity inputs - error.        *)
        (* suFlag=!QuantityQ[t]; *)                      (* dielectric constant is dimensionless; don't need suFlag.     *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        dielecdlload=DefineDLLFunction["DIELECdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
        dielecdlload[rpUnits[t],rpUnits[d],z20,dc];
        ReleaseNETObject[dielecdlload];                                  (* Release the object for garbage collection    *)
        {"\[Kappa]"-> dc}                                                (* Return unitless results                      *)
    ]

fugacity[t_,d_,z_List:{1.0}]:=
    Module[{loadFGCTY2dll,z20,suFlag,fl,fg,ierr=0},
        If[mixedUnits[t,d],Return["Error"]];                    (* Check for mixed Quantity/non-Quantity inputs - error. *)
        suFlag=!QuantityQ[t];                                   (* Set suppress units flag if inputs are non-quantities. *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        Do[$fug1@Set[i,0.0],{i,0,$ncmax-1}];                             (* Zero out the return fugacity array           *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadFGCTY2dll=DefineDLLFunction["FGCTY2dll",$dllPath,"void",{"double*","double*","double[]","double[]","long*","System.Text.StringBuilder","long"}];
        loadFGCTY2dll[rpUnits[t],rpUnits[d],z20,$fug1,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadFGCTY2dll];                                 (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[fugacity::refprop,$herr1@ToString[]];                (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}],                                      (*    Return the error code                     *)
        (* ELSE *)                                                       (* ELSE                                         *)
            fl=NETObjectToExpression[$fug1];                             (*    Extract the fugacity array                *)
            fg=Take[fl, Length[fl]-LengthWhile[Reverse[fl], #==0&]];     (*    Remove any trailing zeroes                *)
            If[Length[fg]==1, fg = fg[[1]]]                              (*    Don't return a list if there's only 1     *)
        ];                                                               (*    Fall through to Return results list...    *)
        If[suFlag,                                                       (* IF SupressUnits option is true               *)
            Return[{"\!\(\*SubscriptBox[\(\[ScriptF]\),\(\[ScriptI]\)]\) [kPa]"->fg}]  (* Return unitless results        *)
        ,                                                                (* ELSE                                         *)
            Return[{"\!\(\*SubscriptBox[\(\[ScriptF]\),\(\[ScriptI]\)]\)"->kPa*fg}]    (* Return a quantity with units   *)     
        ]
    ]

(* ::SubSection:: *)
(* MELT/SUBLIMATION *)

(****************************************************************************)
(* MELTING and SUBLIMATION curves                                           *)
(****************************************************************************)

meltp[p_,z_List:{1.0}]:=                                                 (* Calculate melting temp. at a given pressure  *)
   Module[{loadMELTPdll,z20,suFlag,t=0.,ierr=0},
        suFlag=!QuantityQ[p];                                            (* Set suppress units flag based on input t     *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadMELTPdll=DefineDLLFunction["MELTPdll",$dllPath,"void",{"double*","double[]","double*","long*","System.Text.StringBuilder","long"}];
        loadMELTPdll[rpUnits[p],z20,t,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadMELTPdll];                                  (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[meltp::refprop,$herr1@ToString[]];                   (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"T [K]"->t}]                                         (*       Return unitless results                *)
        ,                                                                (*   ELSE                                       *)
            Return[{"T"->rpUnits[t,"T"]}]                                (*       Return a quantity with units           *)
        ]
   ]


meltt[t_,z_List:{1.0}]:=                                                 (* Calculate melting press. at a given temp.    *)
   Module[{loadMELTTdll,z20,suFlag,p=0.,ierr=0},
        suFlag=!QuantityQ[t];                                            (* Set suppress units flag based on input t     *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadMELTTdll=DefineDLLFunction["MELTTdll",$dllPath,"void",{"double*","double[]","double*","long*","System.Text.StringBuilder","long"}];
        loadMELTTdll[rpUnits[t],z20,p,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadMELTTdll];                                  (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[meltt::refprop,$herr1@ToString[]];                   (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"P [kPa]"->p}]                                       (*       Return unitless results                *)
        ,                                                                (*   ELSE                                       *)
            Return[{"P"->rpUnits[p,"P"]}]                                (*       Return a quantity with units           *)
        ]
   ]

sublp[p_,z_List:{1.0}]:=                                                 (* Calculate melting temp. at a given pressure  *)
   Module[{loadSUBLPdll,z20,suFlag,t=0.,ierr=0},
        suFlag=!QuantityQ[p];                                            (* Set suppress units flag based on input t     *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadSUBLPdll=DefineDLLFunction["SUBLPdll",$dllPath,"void",{"double*","double[]","double*","long*","System.Text.StringBuilder","long"}];
        loadSUBLPdll[rpUnits[p],z20,t,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSUBLPdll];                                  (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[sublp::refprop,$herr1@ToString[]];                   (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"T [K]"->t}]                                         (*       Return unitless results                *)
        ,                                                                (*   ELSE                                       *)
            Return[{"T"->rpUnits[t,"T"]}]                                (*       Return a quantity with units           *)
        ]
   ]


sublt[t_,z_List:{1.0}]:=                                                 (* Calculate melting press. at a given temp.    *)
   Module[{loadSUBLTdll,z20,suFlag,p=0.,ierr=0},
        suFlag=!QuantityQ[t];                                            (* Set suppress units flag based on input t     *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        loadSUBLTdll=DefineDLLFunction["SUBLTdll",$dllPath,"void",{"double*","double[]","double*","long*","System.Text.StringBuilder","long"}];
        loadSUBLTdll[rpUnits[t],z20,p,ierr,$herr1,$errormessagelength];
        ReleaseNETObject[loadSUBLTdll];                                  (* Release the object for garbage collection    *)
        If[ierr>0,                                                       (* IF there's an error                          *)
            Message[sublt::refprop,$herr1@ToString[]];                   (*    Print the RefProp error message           *)
            Return[{"ierr"->ierr}]                                       (*    Return the error code                     *)
        ];                                                               (* ELSE --> Return results list...              *)
        If[suFlag,                                                       (*   IF SupressUnits option is true             *)
            Return[{"P [kPa]"->p}]                                       (*       Return unitless results                *)
        ,                                                                (*   ELSE                                       *)
            Return[{"P"->rpUnits[p,"P"]}]                                (*       Return a quantity with units           *)
        ]
   ]

(* ::SubSection:: *)
(* Compressibility *)

(****************************************************************************)
(* Calculate the virial coefficients as a function of temperature and comp. *)
(* virbcd[] should be used for pure fluids, virb/virc[] for mixtures        *)
(****************************************************************************)

virial[t_,z_List:{1.0}]:=
    Module[{virbcddlload,virbdlload,vircdlload,z20,suFlag,b=0.,c=0.,d=0.,e=0.},
        suFlag=!QuantityQ[t];                                            (* Set suppress units flag based on input t     *)
        z20=PadRight[Take[z, UpTo[$ncmax]], $ncmax];                     (* Trim and pad the input mole fraction array   *)
        $herr1@Clear[];                                                  (* Clear the return error string                *)
        If[Length[z]==1,                                                 (* IF Pure Fluid...                             *)
            virbcddlload=DefineDLLFunction["VIRBCDdll",$dllPath,{"double*","double[]","double*","double*","double*","double*"}];
            virbcddlload[rpUnits[t],z20,b,c,d,e];
            ReleaseNETObject[virbcddlload],                              (* Release the object for garbage collection    *)
        (* ELSE *)                                                       (* IF Mixture...                                *)
            virbdlload=DefineDLLFunction["VIRBdll",$dllPath,{"double*","double[]","double*"}];
            virbdlload[rpUnits[t],z20,b];
            vircdlload=DefineDLLFunction["VIRCdll",$dllPath,{"double*","double[]","double*"}];
            vircdlload[rpUnits[t],z20,c];
            ReleaseNETObject[virbdlload];                                (* Release the object for garbage collection    *)
            ReleaseNETObject[vircdlload]                                 (* Release the object for garbage collection    *)
        ];
        If[suFlag,
            Return[{"a01 [L/mol]"->b,
                    "a02 [(L/mol)\.b2]"->c,
                    "a03/2 [(L/mol)\.b3]"->d,
                    "a04/6 [(L/mol\!\(\*SuperscriptBox[\()\), \(4\)]\)]"->e}
            ],
        (*ELSE*)
            Return[{"a01"->rpUnits[b,"BVIR"],
                    "a02"->rpUnits[c,"CVIR"],
                    "a03/2"->rpUnits[d,"DVIR"],
                    "a04/6"->rpUnits[e,"EVIR"]}
                    ]
        ]
    ]
    

(* ::Section:: *)
(* REFPROP10 API *)

(* Get REFPROP Version Information                                                                               *)
(*                                                                                                               *)
(* Calling getversion[] returns the version string and stores it in $RPVersion                                   *)
(*    as well as storing the Major version numerically in $vMajor for comparisons.                               *)

If[$DLL,
    vstring = getversion[];
    SetPath[$NISTPath]//Quiet;
(*    If[$vMajor<10,SetPath[$NISTPath]];                            Call SetPath no matter what version is loaded. *)
    Print[Framed["Found NIST REFPROP Version: " <> vstring, FrameStyle->None, Background->LightGreen]]
];

General::notimp = "Not implmemented in version " <> $RPVersion <> "of REFPROP";       (* Not Implemented Message *)

(*****************************************************************************************************************)
(*                                       REFPROP 10 Function Calls                                               *)
(*****************************************************************************************************************)

GetEnum[iFlag_Integer:0,hEnum_String]:=                           (* Needs 2 Inputs                              *)
    Module[{loadGETENUMdll,iEnum=0,ierr=0},
        If[$vMajor<10,                                            (* IF REFPROP version < 10.x                   *)
            Message[GetEnum::notimp];                             (*    Issue Implementation Message             *)
            Return["Not Implemented"]                             (*    Return Error                             *)
        ];                                                        (* END IF                                      *)
        $herr1@Clear[];                                           (* Clear the return error string               *)
        loadGETENUMdll=DefineDLLFunction["GETENUMdll",$dllPath,"void",{"long*","char*","long*","long*","System.Text.StringBuilder","long","long"}];
        loadGETENUMdll[iFlag,
                       hEnum,
                       iEnum,
                       ierr,
                       $herr1,
                       $refpropcharlength,
                       $errormessagelength];                      (* Call the DLL with appropriate parameters    *)
        ReleaseNETObject[loadGETENUMdll];                         (* Release the .NET object for garbage coll.   *)
        If[ierr > 0,                                              (* If there is an error, ierr > 0              *)
            Message[GetEnum::refprop,$herr1@ToString[]];          (*     Print the RefProp error Message         *)
            Return[{"ierr"->ierr}]                                (*     Return the error code                   *)
        ];
        Return[iEnum]                                             (* Return the enumeration value                *)
    ];                                                            (* END Module                                  *)


If[$vMajor>=10,                                                   (* IF Refprop 10 or higher                     *)
    $iUnits=GetEnum[1,"DEFAULT"]
];

(*  Function call to ERRMSGdll ***********************************************************************************)
(*  Not advertised as we can return the ErrMsg in a Message[] string as in the Legacy API calls.                 *)
ErrMsg[ierr_]:=                                                   (* Only needs 1 Inputs                         *)
    Module[{loadERRMSGdll},
        If[$vMajor<10,                                            (* IF REFPROP version < 10.x                   *)
            Message[ErrMsg::notimp];                              (*    Issue Implementation Message             *)
            Return["Not Implemented"]                             (*    Return Error                             *)
        ];                                                        (* END IF                                      *)
        $herr1@Clear[];                                           (* Clear the return error string               *)
        loadERRMSGdll=DefineDLLFunction["ERRMSGdll",$dllPath,"void",{"long*","System.Text.StringBuilder","long"}];
        loadERRMSGdll[ierr,
                      $herr1,
                      $errormessagelength];                       (* Call the DLL with appropriate parameters    *)
        ReleaseNETObject[loadERRMSGdll];                          (* Release the .NET object for garbage coll.   *)
        StringTrim[$herr1@ToString[]]                             (* Return error message in trimmed string      *)
    ];                                                            (* END Module                                  *)


(*  Function call to SETFLUIDSdll ********************************************************************************)
SetFluids[hFld_String]:=                                          (* Only needs 1 Inputs                         *)
    Module[{loadSETFLUIDSdll,ierr=0},
        If[$vMajor<10,                                            (* IF REFPROP version < 10.x                   *)
            Message[SetFluids::notimp];                           (*    Issue Implementation Message             *)
            Return["Not Implemented"]                             (*    Return Error                             *)
        ];                                                        (* END IF                                      *)
        loadSETFLUIDSdll=DefineDLLFunction["SETFLUIDSdll",$dllPath,"void",{"char*","long*","long"}];
        loadSETFLUIDSdll[hFld,
                         ierr,
                         $componentstringlength];                 (* Call the DLL with appropriate parameters    *)
        ReleaseNETObject[loadSETFLUIDSdll];                       (* Release the .NET object for garbage coll.   *)
        If[ierr > 0,                                              (* If there is an error, ierr > 0              *)
            Message[SetFluids::refprop,ErrMsg[ierr]];             (*     Print the RefProp error Message         *)
        ];
        Return[ierr]                                              (* In all cases, return ierr                   *)
    ];                                                            (* END Module                                  *)

(*  Function call to SETMIXTUREdll **************************************************************************************)
SetMixture[hMixNme_String]:=                                             (* Only needs 1 Inputs and 1 output            *)
    Module[{loadSETMIXTUREdll,x,z,z1,ierr=0},
        If[$vMajor<10,                                                   (* IF REFPROP version < 10.x                   *)
            Message[SetMixture::notimp];                                 (*    Issue Implementation Message             *)
            Return["Not Implemented"]                                    (*    Return Error                             *)
        ];                                                               (* END IF                                      *)
        z1=MakeNETObject[PadRight[Take[{0.0}, UpTo[$ncmax]], $ncmax]];   (* Initialize z1 buffer for mole fractions     *)
        loadSETMIXTUREdll=DefineDLLFunction["SETMIXTUREdll",$dllPath,"void",{"char*","double[]","long*"}];
        loadSETMIXTUREdll[hMixNme,
                            z1,                                          (* Pass back the molar composition in z1       *)
                          ierr];                                         (* Call the DLL with appropriate parameters    *)
        x=NETObjectToExpression[z1];                                     (*    Extract the liquid mole frxn array       *)
        ReleaseNETObject[loadSETMIXTUREdll];                             (* Release the .NET object for garbage coll.   *)
        ReleaseNETObject[z1];                                            (* Release the .NET object for garbage coll.   *)
        If[ierr > 0,                                                     (* If there is an error, ierr > 0              *)
            Message[SetFluids::refprop,ErrMsg[ierr]];                    (*     Print the RefProp error Message         *)
            Return[ierr],
        (* ELSE *)                                                       (* ELSE                                        *)
            z=Take[x, Length[x]-LengthWhile[Reverse[x],#==0&]];          (*    Remove any trailing zeroes               *)
            $nc = Length[z]                                              (*    Set number of components globally        *)
        ];
        Return[z]                                                        (* Return z                                    *)
    ];                                                                   (* END Module                                  *)


(*  Function call to FLAGSdll *************************************************************************************)
Flags[hFlag_String, jFlag_]:=                                      (* Only needs 2 Inputs                         *)
    Module[{loadFLAGSdll,kFlag=0.,ierr=0},
        If[$vMajor<10,                                             (* IF REFPROP version < 10.x                   *)
            Message[Flags::notimp];                                (*    Issue Implementation Message             *)
            Return["Not Implemented"]                              (*    Return Error                             *)
        ];                                                         (* END IF                                      *)
        kFlag1=MakeNETObject[0,"System.Int32"];                    (* Initialize returned kFlag Value             *)
        $herr1@Clear[];                                            (* Clear the return error string               *)
        loadFLAGSdll=DefineDLLFunction["FLAGSdll",$dllPath,"void",{"char*","long*","long*","long*","System.Text.StringBuilder", "long","long"}];
        loadFLAGSdll[hFlag,
                     jFlag,
                     kFlag,
                     ierr,
                     $herr1,
                     $refpropcharlength,
                     $errormessagelength];                         (* Call the DLL with params                    *)
        ReleaseNETObject[loadFLAGSdll];                            (* Release the .NET object for garbage coll.   *)
        If[ierr > 0,                                               (* If there is an error, ierr1 > 0             *)
            Message[Flags::refprop,StringTrim[$herr1@ToString[]]]; (*     Print the RefProp error Message         *)
            Return[{"ierr"->ierr}]                                 (*     Return ierr                             *)
        ];
        Return[kFlag]                                              (* In all cases, return kFlag                  *)
    ]                                                              (* END Module                                  *)

(*  Function call to REFPROPxdll ************************************************************************************************)

Options[RefProp] = {iUnits->$iUnits, iMass->0, iFlag->0, SatComp->False, SuppressUnits->False};   (* Defaults: $iUnits(DEFAULT), molar basis, do not call SATSPLN   *)
RefProp::splnok = "INFO: Saturation splines created sucessfully.";
RefProp::fijmix = "INFO: Mixing rule: `1`";
RefProp::refreset = "INFO: Reference State reset successful.";
$hInSpec = {"*MELT", "*SUBL", "*SAT*", "*Q*"};
$hOutSpec= {"*MAX*", "*MIN*","M*","*TRP","*CRIT","*C","*CEST","*CTRUE","*REF","*NBP","DIP","REOS"};
$hFlags  = {"CRIT", "TRIP", "NBP", "FLAGS", "EOS*", "SET*", "PATH", "SATSPLN"};
$hNoOut  = {"SET*","PATH","SATSPLN"};
$hOutStr = {"ALTID*","CAS*","CHEMFORM*","SYNONYM*","FAMILY*","FLDNAME*","HASH*","INCHI**",
            "SAFETY*","*NAME*","UNNUMBER*","DOI_*","WEB_*","REF*","GWP*","ODP*","FDIR*","UNITSTRING","DLL#",
            "PHASE","FULLCHEMFORM*","LIQUID*","VAPOR*"};
$hArrays = {"K","F","FC","CPOT","DADN","DNADN","XMOLE","XMASS","XLIQ","XVAP",
            "XMOLELIQ","XMOLEVAP","XMASSLIQ","XMASSVAP","FIJMIX"}

RefProp[ hFld_String:" ", hIn_String, hOut_String, a_, b_, z_List:{1.0}, OptionsPattern[] ]:=
       (* Valid hIn codes are T,P,D,E,H,S, and Q - only Q is unitless so no mixed unit check if using Q                         *)
    Module[{loadREFPROPdll,iUnits2,z20,a2,b2,zL,zOut,xL,x,yL,y,outL,output,suFlag,iUCode=0,q=0.,ierr=0,nOut,hList,f,resList,
            ixFlag,inc},
        If[$vMajor<10,                                                           (* IF REFPROP version < 10.x                   *)
            Message[RefProp::notimp];                                            (*    Issue Implementation Message             *)
            Return["Not Implemented"]                                            (*    Return Error                             *)
        ];                                                                       (* END IF                                      *)
        (* PROCESS hIn string to set Units Flag *********************************************************************************)
        If[!StringMatchQ[hIn,  $hFlags,   IgnoreCase -> True] &&                 (* IF NOT a FLAG string in hIn - AND -         *)
           !StringMatchQ[hOut, $hOutSpec, IgnoreCase -> True],                   (*    NOT a special output case...             *)
            If[!StringMatchQ[hIn,$hInSpec, IgnoreCase -> True],                  (*    IF NOT Special Input type (2 vars)       *)
               If[mixedUnits[a,b],ierr=1]];                                      (*       Check for mixed input types           *)
            suFlag =! QuantityQ[a]                                               (*    Set suFlag based on first input, a       *)
        ,                                                                        (* OTHERWISE                                   *)
            suFlag = OptionValue[SuppressUnits]                                  (*    Set suFlag based on Option, if set       *)
        ];
        If[ierr>0, Message[RefProp::mixed]; Return["Input Error"]];              (* Return if there was a mixed input type      *)
        If[suFlag, iUnits2 = OptionValue[iUnits], iUnits2 = $iUnits];            (* Allow non-def iUnits only if SuppressUnits  *)
        (* INITIALIZE in/out variables ******************************************************************************************)
        z20=MakeNETObject[PadRight[Take[z, UpTo[$ncmax]], $ncmax]];              (* Initialize z20 buffer for mole fractions    *)
        Do[$xliq1@Set[i,0.0],{i,0,$ncmax-1}];                                    (* Zero out the return mole fraction array     *)
        Do[$xvap1@Set[i,0.0],{i,0,$ncmax-1}];                                    (* Zero out the return mole fraction array     *)
        Do[$Output1@Set[i,0.0],{i,0,199}];                                       (* Zero out the Output[200] array              *)
        $herr1@Clear[];                                                          (* Clear the return error string               *)
        $hUnits1@Clear[];                                                        (* Clear the return units string               *)
        ixFlag = Min[Length[z],2];                                               (* ixFlag = 1 (pure fluid) or 2 (mixture)      *)
        a2=rpUnits[a]; b2=rpUnits[b];                                            (* Strip units from a and b if necessary       *)
        If[(hIn == "SETREF") && ((a2 != 1) || (a2 != 2)), 
            a2 = ixFlag];                                                        (* For SETREF, use ixFlag or override with a   *)
        (* Define REFPROPdll Function *******************************************************************************************)
        loadREFPROPdll =DefineDLLFunction["REFPROPdll",$dllPath,"void",
                                         {"char*","char*","char*",               (*      hFld,hIn,hOut                          *)
                                          "long*","long*","long*",               (* iUnits,iMass,iFlag                          *)
                                          "double*","double*","double[]",        (*          a,b,z[20]                          *)
                                          "double[]",                            (*        Output[200]                          *)
                                          "System.Text.StringBuilder","long*",   (*      hUnits,iUCode                          *)
                                          "double[]","double[]",                 (*       x[20], y[20]                          *)
                                          "double[]","double*",                  (*          x3[20], q                          *)
                                          "long*","System.Text.StringBuilder",   (*          ierr,herr                          *)
                                          "long","long","long","long","long"}];
        (* Call REFPROPdll Function *********************************************************************************************)
        loadREFPROPdll[ hFld, hIn, hOut,                                         (* String inputs                (in)           *)
                        iUnits2,                                                 (* iUnits -> defaults to enum for "DEFAULT"    *)
                        OptionValue[iMass],OptionValue[iFlag],                   (* Options                      (in)           *)
                          a2,b2,z20,                                             (* input values                 (in)           *)
                          $Output1,$hUnits1,iUCode,                              (* output values                (out)          *)
                          $xliq1,$xvap1,$x3,q,
                        ierr,$herr1,                                             (* error code and error message (in)           *)
                        $componentstringlength,                                  (* hFld length                  (in)           *)
                        $refpropcharlength,$refpropcharlength,                   (* hIn and hOut lengths         (in)           *)
                        $refpropcharlength,$errormessagelength];                 (* hUnits and herr lengths      (in)           *)
        (* EXTRACT any temporary .NET Objects and Release them ******************************************************************)
        zL=NETObjectToExpression[z20];                                           (* Extract the bulk mole frxn array            *)
        outL=NETObjectToExpression[$Output1];                                    (* Extract the output array                    *)
        xL=NETObjectToExpression[$xliq1];                                        (* Extract the liquid mole frxn array          *)
        yL=NETObjectToExpression[$xvap1];                                        (* Extract the vapor mole frxn array           *)
        ReleaseNETObject[z20];                                                   (* Release the .NET object for garbage coll.   *)
        ReleaseNETObject[loadREFPROPdll];                                        (* Release the .NET object for garbage coll.   *)
        (* Check for Errors *****************************************************************************************************)                                          
        If[ierr > 0,                                                             (* If there is an error, ierr1 > 0             *)
            Message[RefProp::refprop,StringTrim[$herr1@ToString[]]];             (*     Print the RefProp error Message         *)
            Return[{"ierr"->ierr}]                                               (*     Return ierr                             *)
        ];
        (* Return nothing if hIn is in $hNoOut (SETREF, SETREFOFF, PATH, SATSPLN) ***********************************************)
        If[StringMatchQ[hIn, {"SATSPLN"},IgnoreCase -> True] ||                  (* If setting SATSPLN in hIn OR                *)
            (OptionValue[iFlag] == 1),                                           (*    by setting iFlag -> 1 THEN               *)
            Message[RefProp::splnok];                                            (*       Issue SATSPLN Message                 *)
        ];
        If[StringMatchQ[hIn, $hNoOut, IgnoreCase -> True]                        (* IF this call has no output when ierr = 0    *)
            && ((hFld==" ") || ($nc==1)),                                        (*    and no mixture was set THEN              *)
            Switch[ToUpperCase[hIn],                                             (*    Set message based on hIN code            *)
                "SATSPLN",   If[hFld==" ",Return[]],                             (*       Splines Msg issued above, return      *)
                "SETREF",    If[a2 == 1,                                         (*       Reference state set for...            *)
                                Message[RefProp::PurSet,hOut],                   (*          Pure Fluids                        *)
                                Message[RefProp::MixSet,hOut]                    (*          Mixtures                           *)
                             ]; Return[],                                        (*            and return                       *)
                "SETREFOFF", Message[RefProp::refreset];Return[],                (*       Reference state reset. and return     *)
                _, Return[];                                                     (*       Null (just return)                    *)
            ];
        ];
        (* Return string in hUnits (if string result requested in hOut) *********************************************************)
        If[StringMatchQ[hOut, $hOutStr, IgnoreCase -> True],
            If[ToUpperCase@hOut=="FDIR",                                         (* IF Fluid path (FDIR) requested,             *)
                (* Return[StringTrim[$herr1@ToString[]]] *)                      (*   Return $herr1 - it's longer (255 chars)   *)
                Return[StringTrim[$hUnits1@ToString[]]]                          (*   $herr1 Does not work as adv. in manual.   *)
            ,                                                                    (* ELSE                                        *)
                If[ToUpperCase@hOut=="NCOMP",                                    (*   IF NCOMP                                  *)
                    Return[{"NCOMP"->iUCode}]                                    (*     Return number of components in iUCode   *)
                ,                                                                (*   ELSE                                      *)
                    Return[StringTrim[$hUnits1@ToString[]]]                      (*     Return all other strings from $hUnits1  *)
                ]
            ]
        ];
        If[ToUpperCase@hOut == "FIJMIX",                                         (* If requesting the mixing parameters,        *)
        	Message[RefProp::fijmix,StringTrim[$hUnits1@ToString[]]]];           (*   Message mixing rule returned in hUnits    *)
        (* OTHERWISE - prepare the return arrays ********************************************************************************)
        zOut=Take[zL, Length[zL]-LengthWhile[Reverse[zL],#==0&]];                (*    Remove any trailing zeroes from z        *)
        output=Take[outL, Length[outL]-LengthWhile[Reverse[outL],#==0&]];        (*    Remove any trailing zeroes froum Output  *)
        $nc = Length[zOut];                                                      (*    Set number of components globally        *)
        If[OptionValue[SatComp],
            x=Take[xL, Length[xL]-LengthWhile[Reverse[xL],#==0&]];               (*    Remove any trailing zeroes from x        *)
            y=Take[yL, Length[yL]-LengthWhile[Reverse[yL],#==0&]];               (*    Remove any trailing zeroes from y        *)
        ];
        (* Parse out the output string to an UpperCase list and count the elements to be returned *******************************)
        hList = StringSplit[hOut, {" ", ",", ";", "|"}];                         (* Parse hOut into a List                      *)
        nOut = Length[hList];                                                    (* Get the total number of outputs requested   *)
        (* Collapse any Arrays in output to single sub-Array elements ***********************************************************)
        If[StringContainsQ[ToUpperCase@hOut, $hArrays],                          (* IF any requested return value is an Array,  *)
            For[i = 1, i <= nOut, i++,                                           (*   FOR each element in the output list       *)
                If[Length[Position[$hArrays, ToUpperCase@hList[[i]]]] > 0,       (*     IF this return value is an Array,       *)
                    If[ToUpperCase@hList[[i]] == "FIJMIX",                       (*       IF "FIJMIX", collapse 6 els, else $nc *)
                    	inc = 6,                                                 (*         collapse 6 elements, else           *)
                    	inc = $nc];                                              (*         collapse $nc elements               *)
                    If[inc > 1,                                                  (*       IF # elements > 1,                    *)
                        output = ReplacePart[output,                             (*          Replace current element with       *)
                                        i -> Take[output, {i, i + inc - 1}]];    (*          an array by collapsing inc elems   *)
                        output = Drop[output, {i + 1, i + inc - 1}]              (*          and Drop the next inc - 1 elems    *)
                    ];                                                           (*       END IF                                *)
                ];                                                               (*     END IF                                  *)
            ];                                                                   (*   END FOR                                   *)
        ];                                                                       (* END IF                                      *)
        (* Set up function to build ruled list **********************************************************************************)
        If[suFlag,                                                               (* Set up parsing function for Array           *)
            f[i_] := hList[[i]] -> output[[i]],                                  (*   IF suFlag -> Return value as is           *)
            f[i_] := hList[[i]] -> rpUnits[ output[[i]], hList[[i]] ]            (*   ELSE Return value with default units      *)
        ];
        (* Return numerical OUTPUTS in ruled List with units (if not suppressed) ************************************************)
        resList = Array[f, nOut];                                                (* Build Return Array                          *)
        If[ (hFld!=" ") && ($nc>1) , resList = Append[resList,"z"->zOut]];       (* IF setting new mixture, return z (comp.)    *)
        If[ OptionValue[SatComp],                                                (* IF SatComp flagged                          *)
            resList = Join[resList,{"x"->x,"y"->y}]                              (*       THEN return sat. comps., x & y        *)
        ];
        Return[resList]                                                          (* Return ruled list of results                *)
    ];                                                                           (* END Module                                  *)


(*****************************************************************************************************************)
(*                                   End REFPROP 10 API Function Definitions                                     *)
(*****************************************************************************************************************)

(* ::Section:: *)
(* Package Postscript *)

End[]           (* END Private Definitions *)

Protect @@ Names["RefpropLink`*"];

EndPackage[]    (* END Package *)
