(* ::Package:: *)

BeginPackage["RefpropLink`",{"NETLink`"}]
RefpropLink::usage = "RefpropLink calls functions from REFPROP64.dll included in the REFPROP application"
(*initializeFilePaths::usage = "Initializes dll and fluids file path"*)
setup::usage = "setupdll will initialize the program"
info::usage = "provides fluid constants for specified component"
satp::usage = "Calculate saturation properties at a given p"
tprho::usage = "iterate for density as a function of temperature, pressure, and composition for a specified phase"
tpflsh::usage = "General flash subroutine "
therm::usage = "compute thermal quantities as a function of temperature, density, and compositions using core functions (Helmholtz free energy, ideal gas heat capacity and various derivatives and integrals)"
press::usage = "compute pressure as a function of temperature, density, and composition using core functions "
fgcty::usage = "compute fugacity for each of the nc components of a mixture by numerical differentiation (using central differences) of the dimensionless residual Helmholtz energy"
virb::usage = "compute second acoustic virial coefficient as a function of temperature and composition"
virc::usage = "compute third acoustic virial coefficient as a function of temperature and composition"
dpdd::usage = "compute partial derivative of pressure w.r.t. density at constant temperature as a function of temperature, density, and composition"
dpdd2::usage = "compute second partial derivative of pressure w.r.t. density at const temperature as a function of temperature, density, and composition"
dpdt::usage = "compute partial derivative of pressure w.r.t. temperature at constant density as a function of temperature, density, and composition"
dhdl::usage = "Compute partial derivatives of enthalpy w.r.t. t, p, or rho at constant t, p, or rho as a function of temperature, density, and composition"
surft::usage = "Compute surface tension"
trnprp::usage = "Calculate viscosity (eta) and thermal conductivity (tcx)"
tdflsh::usage = "General property calculation with inputs of t,d,x"
pdflsh::usage = "General property calculation with inputs of p,h,x"
meltt::usage = "Calculate melting pressure"
dielec::usage = "Calculate dielectric constant"
RFPI::usage = "Brings up a an interface to run set up"
checkErrorCodes::usage= "Checks error codes during set up"
Begin["`Private`"]

InstallNET[];


ncmax=20; (*maximum number of components*)
refpropcharlength=255; (*maximum number of characters reserved for each character*)
lengthtoreference=3;
errormessagelength=255;
(*ierr=MakeNETObject[0]; (* An integer flag defining an error*)*)
(*herr=MakeNETObject[StringJoin[Table[" ",{256}]]]; (* a character array for storing a string - Error message*)*)
hfmix=$mixturesCoeffPath;


Clear[$dllPath,$fluidsPath,$mixturesCoeffPath,$numberOfComponents,$fluidmixComponents];

setup[dllPath_String, fluidsPath_String,fluidComponents_List, numberOfComponents_,refState_String,ierr_,herr_]:=Module[{loadsetupDll,loadinfoDll,ierr1,herr1},
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];
$dllPath=dllPath;
$fluidsPath=fluidsPath;
$mixturesCoeffPath=FileNameJoin[{$fluidsPath,"HMX.BNC"}];
$numberOfComponents=numberOfComponents;
If[numberOfComponents>1,$fluidmixComponents=StringJoin[Map[$fluidsPath<>"\\"<>#<>"|"&,fluidComponents]],$fluidmixComponents=FileNameJoin[{$fluidsPath,fluidComponents[[1]]}]];
loadsetupDll=DefineDLLFunction["SETUPdll",$dllPath,"void",{"long*","char*","char*","char*","long*","char*", "long","long","long","long"}];
loadsetupDll[$numberOfComponents,$fluidmixComponents,hfmix,refState,ierr,herr,refpropcharlength*ncmax,refpropcharlength,lengthtoreference,errormessagelength];
NETObjectToExpression[ierr1]
]


info[wm_Real,ttp_Real,tnbp_Real,tc_Real,pc_Real,dc_Real,zc_Real,acf_Real,dip_Real,rgas_Real]:=Module[{loadinfoDll},
loadinfoDll=DefineDLLFunction["INFOdll",$dllPath,"void",{"long*","double*","double*","double*","double*","double*", "double*","double*","double*","double*","double*"}];
wm1=MakeNETObject[wm];
ttp1=MakeNETObject[ttp];
tnbp1=MakeNETObject[tnbp];
tc1=MakeNETObject[tc];
pc1=MakeNETObject[pc];
dc1=MakeNETObject[dc];
zc1=MakeNETObject[zc];
acf1=MakeNETObject[acf];
dip1=MakeNETObject[dip];
rgas1=MakeNETObject[rgas];
loadinfoDll[1,wm1,ttp1,tnbp1,tc1,pc1,dc1,zc1,acf1,dip1,rgas1];
NETObjectToExpression/@{"wm"-> wm1,"ttp"-> ttp1,"tnbp"-> tnbp1,"tc"-> tc1,"pc"-> pc1,"dc"-> dc1,"zc"-> zc1,"acf"-> acf1,"dip"-> dip1,"rgas"-> rgas1}
]


satp[p_,x_,i_,t_,dl_,dv_,xliq_,xvap_,ierr_,herr_]:=Module[{loadsattDll},
p1=MakeNETObject[p];
x1=MakeNETObject[x];
i1=MakeNETObject[i];
t1=MakeNETObject[t];
dl1=MakeNETObject[dl];
dv1=MakeNETObject[dv];
xliq1=MakeNETObject[xliq];
xvap1=MakeNETObject[xvap];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];
loadsattDll=DefineDLLFunction["SATPdll",$dllPath,"void",{"double*","double[]","long*","double*","double*","double*","double[]","double[]","long*","char*","long"}];
loadsattDll[p1,x1,i1,t1,dl1,dv1,xliq1,xvap1,ierr1,herr1,errormessagelength];
{NETObjectToExpression/@{"t"-> t1,"dl"->  dl1,"dv"->  dv1},"xliq[0]"-> xliq1@GetValue[0],"xvap[0]"-> xvap1@GetValue[0]}
]


tprho[t_,p_,x_,j_,tmpint_,d_,ierr_,herr_]:=Module[{loadtprhodll,t1,p1,x1,j1,tmpint1,d1,ierr1,herr1},
t1=MakeNETObject[t];
p1=MakeNETObject[p];
x1=MakeNETObject[x];
j1=MakeNETObject[j];
tmpint1=tmpint;
d1=MakeNETObject[d];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];
loadtprhodll=DefineDLLFunction["TPRHOdll",$dllPath,"void",{"double*","double*","double[]","long*","long*","double*","long*","char*","long"}];
loadtprhodll[t1,p1,x1,j1,tmpint1,d1,ierr1,herr1,errormessagelength];
NETObjectToExpression/@{"t"-> t1,"p"-> p1,"d"-> d1}
]


tpflsh[t_,p_,x_,d_,dl_,dv_,xliq_,xvap_,q_,e_,h_,s_,cv_,cp_,w_,ierr_,herr_]:=Module[{tpflshdllload,t1,p1,x1,d1,dl1,dv1,xliq1,xvap1,q1,e1,h1,s1,cv1,cp1,w1,ierr1,herr1},
t1=MakeNETObject[t];
p1=MakeNETObject[p];
x1=MakeNETObject[x];
d1=MakeNETObject[d];
dl1=MakeNETObject[dl];
dv1=MakeNETObject[dv];
xliq1=MakeNETObject[xliq];
xvap1=MakeNETObject[xvap];
q1=MakeNETObject[q];
e1=MakeNETObject[e];
h1=MakeNETObject[h];
s1=MakeNETObject[s];
cv1=MakeNETObject[cv];
cp1=MakeNETObject[cp];
w1=MakeNETObject[w];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];

tpflshdllload=DefineDLLFunction["TPFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*","double*","double[]","double[]","double*","double*","double*","double*","double*","double*","double*","long*","char*","long"}];
tpflshdllload[t1,p1,x1,d1,dl1,dv1,xliq1,xvap1,q1,e1,h1,s1,cv1,cp1,w1,ierr1,herr1,errormessagelength];
NETObjectToExpression/@{"t"-> t1,"p"-> p1,"d"-> d1,"h"-> h1,"cp"-> cp1}
]


(*Calculate surface tension*)
surft[t_,dl_,x_,sigma_,ierr_,herr_]:=Module[{surftdlload,t1,dl1,x1,sigma1,ierr1,herr1},
t1=MakeNETObject[t];
dl1=MakeNETObject[dl];
x1=MakeNETObject[x];
sigma1=MakeNETObject[sigma];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];

surftdlload=DefineDLLFunction["SURFTdll",$dllPath,"void",{"double*","double*","double[]","double*","long*","char*","long"}];
surftdlload[t1,dl1,x1,sigma1,ierr1,herr1,errormessagelength];
NETObjectToExpression/@{"T"-> t1,"Surf. Tn"-> sigma1}
]


(*Calculate viscosity (eta) and thermal conductivity (tcx)*)
trnprp[t_,d_,x_,eta_,tcx_,ierr_,herr_]:=Module[{trnprpdlload,t1,d1,x1,eta1,tcx1,ierr1,herr1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
eta1=MakeNETObject[eta];
tcx1=MakeNETObject[tcx];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];
trnprpdlload=DefineDLLFunction["TRNPRPdll",$dllPath,"void",{"double*","double*","double[]","double*","double*","long*","char*","long"}];
trnprpdlload[t1,d1,x1,eta1,tcx1,ierr1,herr1,errormessagelength];
NETObjectToExpression/@{"VIS"-> eta1,"TH. CND"-> tcx1*1000.0}]


(*Calculate melting pressure*)
meltt[t_,x_,p_,ierr_,herr_]:=Module[{melttdlload,t1,x1,p1,ierr1,herr1},
t1=MakeNETObject[t];
x1=MakeNETObject[x];
p1=MakeNETObject[p];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];
melttdlload=DefineDLLFunction["MELTTdll",$dllPath,"void",{"double*","double[]","double*","long*","char*","long"}];
melttdlload[t1,x1,p1,ierr1,herr1,errormessagelength];
NETObjectToExpression/@{"Melting Pressure(MPa)"-> p1/1000.0,"T"-> t1}
]


(*Calculate dielectric constant*)
dielec[t_,d_,x_,de_]:=Module[{melttdlload,t1,d1,x1,de1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
de1=MakeNETObject[de];
melttdlload=DefineDLLFunction["DIELECdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
melttdlload[t1,d1,x1,de1];
NETObjectToExpression/@{"Dielectric const"-> de1}
]


(*Calculate pressure (p), internal energy (e), enthalpy (h), entropy (s), isochoric (cv) and isobaric (cp) heat capacities, speed of sound (w),
and Joule-Thomson coefficient (hjt) from t,d,x
(subroutines THERM2 and THERM3 contain more properties, see PROP_SUB.FOR)*)
therm[t_,d_,x_,p_,e_,h_,s_,cv_,cp_,w_,hjt_]:=Module[{thermdllload,t1,d1,x1,p1,e1,h1,s1,cv1,cp1,w1,hjt1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
p1=MakeNETObject[p];
e1=MakeNETObject[e];
h1=MakeNETObject[h];
s1=MakeNETObject[s];
cv1=MakeNETObject[cv];
cp1=MakeNETObject[cp];
w1=MakeNETObject[w];
hjt1=MakeNETObject[hjt];
thermdllload=DefineDLLFunction["THERMdll",$dllPath,"void",{"double*","double*","double[]","double*","double*","double*","double*","double*","double*","double*","double*"}];
thermdllload[t1,d1,x1,p1,e1,h1,s1,cv1,cp1,w1,hjt1];]

press[t_,d_,x_,p_]:=Module[{pressdllload,t1,d1,x1,p1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
p1=MakeNETObject[p];
pressdllload=DefineDLLFunction["PRESSdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
pressdllload[t1,d1,x1,p1];]

fgcty[t_,d_,x_,f_]:=Module[{fgctydllload,t1,d1,x1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
f1=MakeNETObject[f];
fgctydllload=DefineDLLFunction["FGCTYdll",$dllPath,"void",{"double*","double*","double[]","double[]"}];
fgctydllload[t1,d1,x1,f1];]

virb[t_,x_,b_]:=Module[{virbdlload,t1,x1},
t1=MakeNETObject[t];
x1=MakeNETObject[x];
b1=MakeNETObject[b];
virbdlload=DefineDLLFunction["VIRBdll",$dllPath,{"double*","double[]","double*"}];
virbdlload[t1,x1,b1];]

virc[t_,x_,c_]:=Module[{vircdlload,t1,x1},
t1=MakeNETObject[t];
x1=MakeNETObject[x];
c1=MakeNETObject[c];
vircdlload=DefineDLLFunction["VIRCdll",$dllPath,{"double*","double[]","double*"}];
vircdlload[t1,x1,c1];
{NETObjectToExpression/@{"b"-> b1,"c"-> c1},"f"-> f1@GetValue[0]}]


(*Calculate the derivatives : dP/dD, d^2 P/dD^2, dP/dT (D indicates density)
(dD/dP, dD/dT, and dB/dT are also available, see PROP_SUB.FOR)*)
dpdd[t_,d_,x_,dpdrho_]:=Module[{dpdddlload,t1,d1,x1,dpdrho1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
dpdrho1=MakeNETObject[dpdrho];
dpdddlload=DefineDLLFunction["DPDDdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
dpdddlload[t1,d1,x1,dpdrho1];]

dpdd2[t_,d_,x_,d2pdd2_]:=Module[{dpdd2dlload,t1,d1,x1,d2pdd21},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
d2pdd21=MakeNETObject[d2pdd2];
dpdd2dlload=DefineDLLFunction["DPDD2dll",$dllPath,"void",{"double*","double*","double[]","double*"}];
dpdd2dlload[t1,d1,x1,d2pdd21];]

dpdt[t_,d_,x_,dpdtvar_]:=Module[{dpdtdlload,t1,d1,x1,dpdtvar1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
dpdtvar1=MakeNETObject[dpdtvar];
dpdtdlload=DefineDLLFunction["DPDTdll",$dllPath,"void",{"double*","double*","double[]","double*"}];
dpdtdlload[t1,d1,x1,dpdtvar1];]

(*Calculate derivatives of enthalpy with respect to T, P and D*)
dhdl[t_,d_,x_,dhdtd_,dhdtp_,dhddt_,dhddp_,dhdpt_,dhdpd_]:=Module[{dhd1load,t1,d1,x1,dhdtd1,dhdtp1,dhddt1,dhddp1,dhdpt1,dhdpd1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
dhdtd1=MakeNETObject[dhdtd];
dhdtp1=MakeNETObject[dhdtp];
dhddt1=MakeNETObject[dhddt];
dhddp1=MakeNETObject[dhddp];
dhdpt1=MakeNETObject[dhdpt];
dhdpd1=MakeNETObject[dhdpd];
dhd1load=DefineDLLFunction["DHD1dll",$dllPath,"void",{"double*","double*","double[]","double*","double*","double*","double*","double*","double*"}];
dhd1load[t1,d1,x1,dhdtd1,dhdtp1,dhddt1,dhddp1,dhdpt1,dhdpd1];
NETObjectToExpression/@{"dhdt_d"-> dhdtd1,"dhdt_p"-> dhdtp1,"dhdd_t"-> dhddt1,"dhdd_p"-> dhddp1,"dhdp_t"-> dhdpt1,"dhdp_d"-> dhdpd1}
]


(*General property calculation with inputs of t,d,x*)
tdflsh[t_,d_,x_,pp_,dl_,dv_,xliq_,xvap_,q_,e_,h1_,s_,cv_,cp_,w_,ierr_,herr_]:=Module[{tdflshdlload,t1,d1,x1,pp1,dl1,dv1,xliq1,xvap1,q1,e1,h11,s1,cv1,cp1,w1,ierr1,herr1},
t1=MakeNETObject[t];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
pp1=MakeNETObject[pp];
dl1=MakeNETObject[dl];
dv1=MakeNETObject[dv];
xliq1=MakeNETObject[xliq];
xvap1=MakeNETObject[xvap];
q1=MakeNETObject[q];
e1=MakeNETObject[e];
h11=MakeNETObject[h1];
s1=MakeNETObject[s];
cv1=MakeNETObject[cv];
cp1=MakeNETObject[cp];
w1=MakeNETObject[w];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];
tdflshdlload=DefineDLLFunction["TDFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*","double*","double[]","double[]","double*","double*","double*","double*","double*","double*","double*","long*","char*","long"}];
tdflshdlload[t1,d1,x1,pp1,dl1,dv1,xliq1,xvap1,q1,e1,h11,s1,cv1,cp1,w1,ierr1,herr1,errormessagelength];
NETObjectToExpression/@{"T"-> t1,"D"->d1,"P from TDFLSH"-> pp1/1000.0}]


(*General property calculation with inputs of p,d,x*)
pdflsh[p_,d_,x_,tt_,dl_,dv_,xliq_,xvap_,q_,e_,h1_,s_,cv_,cp_,w_,ierr_,herr_]:=Module[{pdflshdlload,p1,d1,x1,tt1,dl1,dv1,xliq1,xvap1,q1,e1,h11,s1,cv1,cp1,w1,ierr1,herr1},
p1=MakeNETObject[p];
d1=MakeNETObject[d];
x1=MakeNETObject[x];
tt1=MakeNETObject[tt];
dl1=MakeNETObject[dl];
dv1=MakeNETObject[dv];
xliq1=MakeNETObject[xliq];
xvap1=MakeNETObject[xvap];
q1=MakeNETObject[q];
e1=MakeNETObject[e];
h11=MakeNETObject[h1];
s1=MakeNETObject[s];
cv1=MakeNETObject[cv];
cp1=MakeNETObject[cp];
w1=MakeNETObject[w];
ierr1=MakeNETObject[ierr];
herr1=MakeNETObject[herr];
pdflshdlload=DefineDLLFunction["PDFLSHdll",$dllPath,"void",{"double*","double*","double[]","double*","double*","double*","double[]","double[]","double*","double*","double*","double*","double*","double*","double*","long*","char*","long"}];
pdflshdlload[p1,d1,x1,tt1,dl1,dv1,xliq1,xvap1,q1,e1,h11,s1,cv1,cp1,w1,ierr1,herr1,errormessagelength];
NETObjectToExpression/@{"T"-> tt1,"D"-> d1,"P from PDFLSH"-> p1/1000.0}]


(*Check error codes returned by setup*)
checkErrorCodes[errcode_]:=Module[{},Which[errcode==0,Print["Initialization successful"],
errcode==101,Print["Error in opening file"],
errcode==102,Print["Error in file or premature end of file"],
errcode==-103,Print["Unknown model encountered in file"],
errcode==104,Print["Error in setup of model"],
errcode==105,Print["Specified model not found"],
errcode==111,Print["rror in opening mixture file"],
errcode==112,Print["Mixture file of wrong type"],
errcode==114,Print["nc<>nc from setmod"],
errcode==-117,Print["binary pair not found, all parameters will be estimated"],
errcode==117,Print["No mixture data are available, mixture is outside the range of the model and calculations will not be made"]
]]


$filepath=StringSplit[Import[FileNameJoin[{$UserBaseDirectory,"Applications","RefpropLink","Filepaths.txt"}]],","];


RFPI[]:=DynamicModule[{dllPath=$filepath[[1]],fluidPath=$filepath[[2]],nComp=0,fluidMix={ },val,selectFunc,data,refState="DEF"},
Deploy[
   Panel[
      Column[{Grid[{{Style["Initialize program",Bold, Black]},
{"Input path to REFPROP.DLL",InputField[Dynamic[dllPath],String],FileNameSetter[Dynamic[dllPath]]},
{"Input path to fluids",InputField[Dynamic[fluidPath],String],FileNameSetter[Dynamic[fluidPath],"Directory"]},
{"Input number of components",InputField[Dynamic[nComp],Number]},
{"Input single fluid or mixture",InputField[Dynamic[fluidMix]]},
{"Input reference state",PopupMenu[Dynamic[refState],{"DEF"-> "DEF- Default reference state","NBP"-> "NBP-h,s = 0 at pure component normal boiling point(s)","ASH"-> "ASH-h,s = 0 for sat liquid at -40 C (ASHRAE convention)","IIR"-> "IIR-h = 200, s = 1.0 for sat liq at 0 C (IIR convention)"}]}},Alignment->Left],

Button["Initialize",If[Dynamic[nComp][[1]]==0||Length[Dynamic[fluidMix][[1]]]==0,Print["Enter number of components and fluid/mixture"];,val=Quiet@setup[Dynamic[dllPath][[1]],Dynamic[fluidPath][[1]],Dynamic[fluidMix][[1]],Dynamic[nComp][[1]],Dynamic[refState][[1]],0,StringJoin[Table[" ",{256}]]];checkErrorCodes[val]];
If[val==0,Export[FileNameJoin[{$UserBaseDirectory,"Applications","RefpropLink","Filepaths.txt"}],{Dynamic[dllPath][[1]]<>","<>Dynamic[fluidPath][[1]]}];
,Print["Set up did not initialize successfully"]],ImageSize->Large,Method->"Queued"]}]]]]


End[ ]
EndPackage[ ]
