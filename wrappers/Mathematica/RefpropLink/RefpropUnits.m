(* ::Package:: *)

(* Wolfram Language Package *)

BeginPackage["RefpropLink`RefpropUnits`"]
RefpropUnits::usage = "Unit used to simplify unit conversions in RefpropLink" 
(* Exported symbols added here with SymbolName::usage *)  
rpUnits::usage="rpUnits[X] examines the unit types on Quantity X, converts the units to default RefProp units, and returns the magnitude of that Quantity. " <>
               "\nrpUnits[X, uOut] takes a non-Quantity value in X and returns a Quantity with units applied to X according to the " <>
               "unit type string provided in uOut."
mixedUnits::usage = "mixedUnits[X,Y] returns True if one or the other of the input values (but not both) is a unit Quantity." <> 
                    "  If both inputs are Quantity values or non-Quantity values, the returned value is False"
(* Unit Symbol Exports *)
(*** Length ***)
km::usage="kilometers"
m::usage="meters"
mm::usage="millimeters"
cm::usage="centimeters"
\[Mu]m::usage="micrometers"
nm::usage="nanometers"
in::usage="inch"
ft::usage="feet"
mi::usage="miles"
mil::usage="mil"
micron::usage="micron"
(*** Volume ***)
L::usage = "liters"
cc::usage = "cubic centimeters"
gal::usage = "gallons"
(*** Time ***)
s::usage="seconds"
min::usage="minutes"
hr::usage="hours"
ms::usage="milliseconds"
\[Mu]s::usage = "microseconds"
ns::usage = "nanoseconds"
(*** Velocity ***)
mph::usage = "miles per hour"
kph::usage = "kilometers per hour"
(*** Mass ***)
gm::usage = "grams"
kg::usage = "kilograms"
lb::usage = "pounds"
(*** Force Units ***)
Newton::usage = "newtons"
mN::usage = "milli-Newton"
lbf::usage = "pounds force"
(*** Pressure  ***)
psi::usage = "pounds per square inch"
ksi::usage = "1,000 pounds per square inch"
psf::usage = "pounds per square foot"
atm::usage = "atmospheres"
Pa::usage = "pascals"
\[Mu]Pa::usage = "micropascals"
kPa::usage = "kilopascals"
MPa::usage = "megapascals"
bar::usage = "Bars"
torr::usage = "Torr"
mmHg::usage = "millimeters of mercury"
inHg::usage = "inches of mercury"
inH2O::usage = "inches of water"
(*** Temperature ***)
\[Degree]F::usage = "degrees Fahrenheit - Obsolete as of Mathematica 13.2"
\[Degree]R::usage = "degrees Rankin"
\[Degree]K::usage = "degree Kelvin"
\[Degree]C::usage = "degrees Celsius - Obsolete as of Mathematica 13.2"
(*** Molar Quantities ***)
mol::usage = "moles"
kmol::usage = "kilomoles"
(*** Energy ***)
J::usage = "joules"
kJ::usage = "kilojoules"
MJ::usage = "megajoules"
BTU::usage = "British Thermal Units"
kBTU::usage = "Thousand BTUs"
MBTU::usage = "Million BTUs"
ergs::usage = "Ergs"
(*** Power ***)
W::usage = "watts"
mW::usage = "milliwatts"
kW::usage = "kilowatts"
MW::usage = "megawatts"
(*** Viscosity ***)
poise::usage = "Poise"
cP::usage = "Centipoise"
\[Mu]cP::usage = "Micropoise"
(*** Dipole Moment ***)
debye::usage = "debyes"


Begin["`Private`"] (* Begin Private Context *) 

(***********************************************************************************************)
(*                    Thermodynamics / Physical Unit Definitions                               *)
(***********************************************************************************************)

(*** Length Units ***)
km = Quantity[1.,"Kilometers"];
m = Quantity[1., "Meters"];
mm = Quantity[1., "Millimeters"];
cm = Quantity[1., "Centimeters"];
\[Mu]m = Quantity[1., "Micrometers"];
nm = Quantity[1., "Nanometers"];
in = Quantity[1., "Inches"];
ft = Quantity[1., "Feet"];
mil = Quantity[1.0e-6, "Inches"];
micron = Quantity[1., "Microinches"];

(*** Volume Units ***)
L = Quantity[1., "Liters"];
cc = Quantity[1., "Centimeters"^3];
gal = Quantity[1., "Gallons"];

(*** Time Units ***)
s = Quantity[1., "Seconds"];
min = Quantity[1., "Minutes"];
hr = Quantity[1., "Hours"];
ms = Quantity[1., "Milliseconds"];
\[Mu]s = Quantity[1., "Microseconds"];
ns = Quantity[1., "Nanoseconds"];

(*** Velocity Units ***)
mph = Quantity[1.,"Miles"/"Hours"];
kph = Quantity[1.,"Kilometers"/"Hours"];

(*** Mass Units ***)
gm = Quantity[1., "Grams"];
kg = Quantity[1., "Kilograms"];
lb = Quantity[1., "Pounds"];

(*** Force Units ***)
Newton = Quantity[1., "Newtons"];
mN = Quantity[0.001, "Millinewtons"];   (* Fixes functionality in MM v13.2 *)
lbf = Quantity[1., "PoundsForce"];

(*** Pressure  ***)
psi = Quantity[1., "PoundsForce"/"Inches"^2];
ksi = Quantity[1000.,"PoundsForce"/"Inches"^2];
psf = Quantity[1., "PoundsForce"/"Feet"^2];
atm = Quantity[1., "Atmospheres"];
Pa = Quantity[1., "Pascals"];
\[Mu]Pa = Quantity[1., "Micropascals"];
kPa = Quantity[1., "Kilopascals"];
MPa = Quantity[1., "Megapascals"];
bar = Quantity[1., "Bars"];
torr = Quantity[1., "Torr"];
mmHg = Quantity[1., "MillimetersOfMercury"];
inHg = Quantity[1., "InchesOfMercury"];
inH2O = Quantity[1., "InchesOfWaterColumn"];

(*** Temperature ***)
(* \[Degree]F and \[Degree]C removed due to fundamental change in unit handling as of Mathematica *)
(* version 13.2; Affine temperatures must be set by user using the Quantity[] function. *)                    
If[ $VersionNumber < 13.2, \[Degree]F = Quantity[1., "DegreesFahrenheit"]];
R = Quantity[1., "DegreesRankine"];
K = Quantity[1., "Kelvins"];
If[ $VersionNumber < 13.2, \[Degree]C = Quantity[1., "DegreesCelsius"]];

(*** Molar Quantities ***)
mol = Quantity[1., "Moles"];
kmol = Quantity[1., "Kilomoles"];

(*** Energy ***)
J = Quantity[1., "Joules"];
kJ = Quantity[1., "Kilojoules"];
MJ = Quantity[1., "Megajoules"];
ergs = Quantity[1., "Ergs"];
BTU = Quantity[1., "BritishThermalUnitsIT"];
kBTU = Quantity[1000., "BritishThermalUnitsIT"];
MBTU = Quantity[1.e6, "BritishThermalUnitsIT"];

(*** Power ***)
W = Quantity[1., "Watts"];
mW = Quantity[1., "Milliwatts"];
kW = Quantity[1., "Kilowatts"];
MW = Quantity[1., "Megawatts"];

(** Viscosity **)
poise = Quantity[1.,"Poise"];
cP = Quantity[1., "Centipoise"];
\[Mu]cP = Quantity[1., "Micropoise"];

(*** Dipole Moment ***)
debye = Quantity[1., "Debyes"];

(***********************************************************************************************)
(* END Unit Definitions                                                                        *)
(***********************************************************************************************)

(*** Error and Information Messages ***)
rpUnits::badCode  = "Invalid unit request code: `1`";        (* Bad unit request code                   *)
rpUnits::badUnit  = "Unknown unit type: `1`";                (* Bad unit type on X                      *)
rpUnits::inout    = "Input value already has units of: `1`"; (* Request to apply units to Quantity      *)
rpUnits::noUnits  = "Dimensionless value detected";          (* X is Dimensionless                      *)
rpUnits::outUnits = "Applying units of `1` to `2`";          (* Applying Units to X                     *)
rpUnits::strip    = "`1` units detected: converting to `2` and stripping units off." (* Stripping units *)
rpUnits::uOutDbg  = "uOut has value of `1` and length `2`"   (* Debug output unit request               *)
General::mixedInputs = "Error: Mixed quantity/non-quantity input state points." (* General error msg.   *)

SetAttributes[{rpUnits}, {Listable}];     (* Can be made Listable to apply units to entire arrays.      *)

rpUnits[X_, uOut_String:""] :=
 Module[{U, uOutMod, uString, uDebug},
  uDebug = False;
  If[uDebug, Print["Unit dimensions on input variable: " <> UnitDimensions[X]]];
  If[uDebug, Message[rpUnits::uOutDbg,uOut,StringLength[uOut]]];
  If[StringLength[uOut]>0,                                         (* IF there's an output unit requested    *)
    If[QuantityQ[X],                                               (*    IF X already has units...           *)
        Message[rpUnits::inout,QuantityUnit[X]];                   (*      Issue error message and           *)
        Return[X];                                                 (*      just return the input value       *)
    ];                                                             (*    ELSE                                *)
    uOutMod=ToUpperCase[uOut];
    If[StringMatchQ[uOutMod, {"*VAP","*LIQ","*MAX","*MIN"}],uOutMod=StringDrop[uOutMod,-3]];
    Switch[ToUpperCase[uOutMod],                                   (*    Set output unit based on uOut code  *)
        "P",  U = QuantityUnit[kPa],
        "T",  U = QuantityUnit[K],
        "D",  U = QuantityUnit[mol/L],
        "V",  U = QuantityUnit[L/mol],
        "H",  U = QuantityUnit[J/mol],
        "E",  U = QuantityUnit[J/mol],
        "S",  U = QuantityUnit[J/mol/K],
        "CV", U = QuantityUnit[J/mol/K],
        "CP", U = QuantityUnit[J/mol/K],
        "W",  U = QuantityUnit[m/s],
        "JT", U = QuantityUnit[K/kPa],
        "A",  U = QuantityUnit[J/mol],
        "G",  U = QuantityUnit[J/mol],
        "R",  U = QuantityUnit[J/mol/K],
        "M",  U = QuantityUnit[kg/kmol],
        "MM", U = QuantityUnit[kg/kmol],
        (* Not so regular properties *)
        "KAPPA",  U = QuantityUnit[1./kPa],
        "BETA",   U = QuantityUnit[1./K],
        "BETAS",  U = QuantityUnit[1./kPa],
        "BS",     U = QuantityUnit[kPa],
        "KKT",    U = QuantityUnit[kPa],
        "THROTT", U = QuantityUnit[L/mol],
        (* Derivatives                *)
        "DPDD",      U = QuantityUnit[kPa]/QuantityUnit[L/mol],
        "DPDT",      U = QuantityUnit[kPa/K],
        "DDDP",      U = QuantityUnit[mol/L/kPa],
        "DDDP",      U = QuantityUnit[mol/L/K],
        "DTDP",      U = QuantityUnit[K/kPa],
        "DTDD",      U = QuantityUnit[K*L/mol],
        "D2PDD2",    U = QuantityUnit[kPa]/Power[QuantityUnit[L/mol],2],
        "D2PDT2",    U = QuantityUnit[kPa/K/K],
        "D2PDTD",    U = QuantityUnit[kPa*mol/L/K],
        "D2DDP2",    U = QuantityUnit[mol/L/kPa/kPa],
        "D2DDT2",    U = QuantityUnit[mol/L/K/K],
        "D2DDPT",    U = QuantityUnit[mol/L/kPa/K],
        "D2TDP2",    U = QuantityUnit[K/kPa/kPa],
        "D2TDD2",    U = QuantityUnit[K]/Power[QuantityUnit[mol/L],2],
        "D2TDPD",    U = QuantityUnit[K*mol/kPa/L],
        (* Enthalpy Derivatives                *)
        "DHDT_D",      U = QuantityUnit[J/mol/K],
        "DHDT_P",      U = QuantityUnit[J/mol/K],
        "DHDD_P",      U = QuantityUnit[J/mol*L/mol],
        "DHDD_T",      U = QuantityUnit[J/mol*L/mol],
        "DHDP_T",      U = QuantityUnit[J/mol/kPa],
        "DHDP_D",      U = QuantityUnit[J/mol/kPa],
        (* Entropy Derivatives                *)
        "DSDT_D",      U = QuantityUnit[J/mol/K/K],
        "DSDT_P",      U = QuantityUnit[J/mol/K/K],
        "DSDD_P",      U = QuantityUnit[J/mol*L/mol/K],
        "DSDD_T",      U = QuantityUnit[J/mol*L/mol/K],
        "DSDP_T",      U = QuantityUnit[J/mol/kPa/K],
        "DSDP_D",      U = QuantityUnit[J/mol/kPa/K],
        (* Virial Coefficients       *)
        "BVIR",      U = QuantityUnit[L/mol],
        "CVIR",      U = Power[QuantityUnit[L/mol],2],
        "DVIR",      U = Power[QuantityUnit[L/mol],3],
        "EVIR",      U = Power[QuantityUnit[L/mol],4],
        "DBVIRDT",   U = QuantityUnit[L/mol/K],
        "D2BVIRDT2", U = QuantityUnit[L/mol/K/K],
        "DCVIRDT",   U = Power[QuantityUnit[L/mol],2]/QuantityUnit[K],
        "D2CVIRDT2", U = Power[QuantityUnit[L/mol],2]/Power[QuantityUnit[K],2],
        "DDVIRDT",   U = Power[QuantityUnit[L/mol],3]/QuantityUnit[K],
        "D2DVIRDT2", U = Power[QuantityUnit[L/mol],3]/Power[QuantityUnit[K],2],
        "BA",        U = QuantityUnit[L/mol],
        "CA",        U = Power[QuantityUnit[L/mol],2],
        (* EOS Testing Properties      *)
        "(Z-1)/D",   U = QuantityUnit[L/mol],
        "(Z-1)/P",   U = QuantityUnit[L/mol*kPa],
        "P*V",       U = QuantityUnit[kPa*L/mol],
        "S*D",       U = QuantityUnit[J/L/K],
        "N1/T",      U = QuantityUnit[1./K],
        "RD",        U = QuantityUnit[mol/L],
        (* Properties from ancillary equations *)
        "ANC-TP",    U = QuantityUnit[kPa],
        "ANC-TDL",   U = QuantityUnit[mol/L],
        "ANC-TDV",   U = QuantityUnit[mol/L],
        "ANC-PR",    U = QuantityUnit[K],
        "ANC-DT",    U = QuantityUnit[K],
        "MELT-TP",   U = QuantityUnit[kPa],
        "MELT-PT",   U = QuantityUnit[K],
        "SUBL-TP",   U = QuantityUnit[kPa],
        "SUBL-PT",   U = QuantityUnit[K],
        (* Less Common Saturation Properties    *)
        "CSAT",      U = QuantityUnit[J/mol/K],
        "CV2P",      U = QuantityUnit[J/mol/K],
        "DPDTSAT",   U = QuantityUnit[kPa/K],
        "DHDZSAT",   U = QuantityUnit[J/mol],
        "LIQSPNDL",  U = QuantityUnit[mol/L],
        "VAPSPNDL",  U = QuantityUnit[mol/L],
        (* Excess Properties                    *)
        "VE",      U = QuantityUnit[L/mol],
        "EE",      U = QuantityUnit[J/mol],
        "HE",      U = QuantityUnit[J/mol],
        "SE",      U = QuantityUnit[J/mol/K],
        "AE",      U = QuantityUnit[J/mol],
        "GE",      U = QuantityUnit[J/mol],
        "B12",     U = QuantityUnit[L/mol],
        (* Ideal Gas Properties                  *)
        "P0",      U = QuantityUnit[kPa],
        "E0",      U = QuantityUnit[J/mol],
        "H0",      U = QuantityUnit[J/mol],
        "S0",      U = QuantityUnit[J/mol/K],
        "CV0",     U = QuantityUnit[J/mol/K],
        "CP0",     U = QuantityUnit[J/mol/K],
        "W0",      U = QuantityUnit[m/s],
        "A0",      U = QuantityUnit[J/mol],
        "G0",      U = QuantityUnit[J/mol],
        "P-P0",    U = QuantityUnit[kPa],
        (* Residual Properties                  *)
        "PR",      U = QuantityUnit[kPa],
        "ER",      U = QuantityUnit[J/mol],
        "HR",      U = QuantityUnit[J/mol],
        "SR",      U = QuantityUnit[J/mol/K],
        "CVR",     U = QuantityUnit[J/mol/K],
        "CPR",     U = QuantityUnit[J/mol/K],
        "AR",      U = QuantityUnit[J/mol],
        "GR",      U = QuantityUnit[J/mol],
        (* Critical point and P,T maximums along isopleth *)
        "TC",      U = QuantityUnit[K],
        "PC",      U = QuantityUnit[kPa],
        "DC",      U = QuantityUnit[mol/L],
        "TCEST",   U = QuantityUnit[K],
        "PCEST",   U = QuantityUnit[kPa],
        "DCEST",   U = QuantityUnit[mol/L],
        "TMAXT",   U = QuantityUnit[K],
        "PMAXT",   U = QuantityUnit[kPa],
        "DMAXT",   U = QuantityUnit[mol/L],
        "TMAXP",   U = QuantityUnit[K],
        "PMAXP",   U = QuantityUnit[kPa],
        "DMAXP",   U = QuantityUnit[mol/L],
        (* Reducing Properties                  *)
        "TRED",    U = QuantityUnit[K],
        "DRED",    U = QuantityUnit[mol/L],
        (* Limits                               *)
        "TMIN",    U = QuantityUnit[K],
        "TMAX",    U = QuantityUnit[K],
        "PMAX",      U = QuantityUnit[kPa],
        "DMAX",      U = QuantityUnit[mol/L],
        (* Transport Properties                   *)
        "MU",      U = QuantityUnit[\[Mu]Pa*s],
        "VIS",     U = QuantityUnit[\[Mu]Pa*s],
        "TCX",     U = QuantityUnit[W/m/K],
        "TD",      U = QuantityUnit[cm*cm/s],
        "KV",      U = QuantityUnit[cm*cm/s],
        "ST",      U = QuantityUnit[Newton/m],
        "STN",     U = QuantityUnit[Newton/m],
        "DM",      U = QuantityUnit[debye],
        "DIPOLE",  U = QuantityUnit[debye],
        (* Heating Values           *)
        "SPHT",        U = QuantityUnit[J/mol],
        "HFRM",        U = QuantityUnit[J/mol],
        "HG",          U = QuantityUnit[J/mol],
        "HN",          U = QuantityUnit[J/mol],
        "HGLQ",        U = QuantityUnit[J/mol],
        "HNLQ",        U = QuantityUnit[J/mol],
        "HGVOL",       U = QuantityUnit[MJ/m/m/m],
        "HNVOL",       U = QuantityUnit[MJ/m/m/m],
        "HEATVAPZ",    U = QuantityUnit[J/mol],
        "HEATVAPZ_T",  U = QuantityUnit[J/mol],
        "HEATVAPZ_P",  U = QuantityUnit[J/mol],
        "HEATVALUE",   U = QuantityUnit[J/mol],
        (* Other Properties           *)
        "PINT",        U = QuantityUnit[kPa],
        "PREP",        U = QuantityUnit[kPa],
        "PATT",        U = QuantityUnit[kPa],
        "EXERGY",      U = QuantityUnit[J/mol],
        "CEXERGY",     U = QuantityUnit[J/mol],
        "TMF",         U = QuantityUnit[kg/m/m/s],
        (* Fluid fixed points for mixtures         *)
        "TCRIT",       U = QuantityUnit[K],
        "PCRIT",       U = QuantityUnit[kPa],
        "DCRIT",       U = QuantityUnit[mol/L],
        "TCTRUE",      U = QuantityUnit[K],
        "PCTRUE",      U = QuantityUnit[kPa],
        "TTRP",        U = QuantityUnit[K],
        "PTRP",        U = QuantityUnit[kPa],
        "DTRP",        U = QuantityUnit[mol/L],
        "TNBP",        U = QuantityUnit[K],
        "REOS",        U = QuantityUnit[J/mol/K],
        "TREF",        U = QuantityUnit[K],
        "PREF",        U = QuantityUnit[mol/L],
        "HREF",        U = QuantityUnit[J/mol],
        "SREF",        U = QuantityUnit[J/mol/K],
        (* Input Transport Property Units as function of component Number *)
        "ETA0",        U = QuantityUnit[\[Mu]Pa*s],
        "ETAB2",       U = QuantityUnit[\[Mu]Pa*s],
        "ETAR",        U = QuantityUnit[\[Mu]Pa*s],
        "ETAC",        U = QuantityUnit[\[Mu]Pa*s],
        "TCX0",        U = QuantityUnit[W/m/K],
        "TCXR",        U = QuantityUnit[W/m/K],
        "TCXC",        U = QuantityUnit[W/m/K],
        (* Input Mixture Property Units as a function of component number *)
        "F",           U = QuantityUnit[kPa],
        "CPOT",        U = QuantityUnit[J/mol],
        (* Catch-All for dimensionless values*)
        _,    (* Message[rpUnits::badCode,uOut]; *) Return[X]       (* No error; catch-all for dimensionless    *)
    ];
    Return[Quantity[X,U]]                                           (*    Return the X value with units applied *)
  ,                                                                 (* ELSE                                     *)
    Switch[UnitDimensions[X],                                       (*    Detect the units on X                 *)
       UnitDimensions[kPa],                  uString = "P";         (*       Set Unit code for debug msg        *)
          U = QuantityUnit[kPa],                                    (*       Set Unit String                    *)
       UnitDimensions[K],                    uString = "T";
          U = QuantityUnit[K],
       UnitDimensions[mol/L],                uString = "D";
          U = QuantityUnit[mol/L],
       UnitDimensions[J/mol],                uString = "E";
          U = QuantityUnit[J/mol],
       UnitDimensions[J/mol/K],              uString = "S";
          U = QuantityUnit[J/mol/K],
       UnitDimensions[m/s],                  uString = "W";
          U = QuantityUnit[m/s],
       UnitDimensions[K/kPa],                uString = "JT";
          U = QuantityUnit[K/kPa],
       UnitDimensions[kg/kmol],              uString = "MM";
          U = QuantityUnit[kg/kmol],
       UnitDimensions[\[Mu]Pa*s],            uString = "MU";
          U = QuantityUnit[\[Mu]Pa*s],
       UnitDimensions[W/m/K],                uString = "TCX";
          U = QuantityUnit[W/m/K],
       UnitDimensions[debye],                uString = "DIPOLE";
          U = QuantityUnit[debye],
       {},
          If[uDebug, Message[rpUnits::noUnits]];                     (*     If X has no units                       *)
          Return[X],                                                 (*        just return the unitless value back  *)
       _,
          Message[rpUnits::badUnit,UnitDimensions[X]];               (*     If unexpected units on X                *)
          Return[X]                                                  (*        issue error and return X             *)
       ];
    If[uDebug, Message[rpUnits::strip,uString]];                     (*    Print debug message for stripping        *)
    N[QuantityMagnitude[UnitConvert[X, U]]]                          (*    Convert to RP units and return magnitude *)
  ]
 ]

mixedUnits[X_, Y_] :=
 Module[{},
    If[Xor[QuantityQ[X],QuantityQ[Y]],
        Message[mixedUnits::mixedInputs];True
    ,
        False
    ]
 ]


End[] (* End Private Context *)

EndPackage[]
