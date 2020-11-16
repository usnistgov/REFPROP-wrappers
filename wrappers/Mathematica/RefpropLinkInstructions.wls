#!/usr/bin/env wolframscript
(* ::Package:: *)

(* ::Section:: *)
(*Steps to install RefpropLink package*)


(* ::ItemNumbered:: *)
(*Unzip the RefpropLink folder.*)


(* ::ItemNumbered:: *)
(*Evaluate (use Shift+Enter) the following:*)


(* ::Input:: *)
(*SystemOpen[FileNameJoin[{$UserBaseDirectory,"Applications"}]]*)


(* ::Text:: *)
(*The above command will open up the Windows explorer showing the Applications folder.*)


(* ::ItemNumbered:: *)
(*Copy unzipped folder RefpropLink into Applications folder.*)


(* ::Text:: *)
(*Now start evaluating the sections below.*)


(* ::Section:: *)
(*Load RefpropLink*)


(* ::Text:: *)
(*Start a new kernel session*)


(* ::Input:: *)
(*Quit*)


(* ::Text:: *)
(*Load RefpropLink package*)


(* ::Input:: *)
(*Needs["RefpropLink`"]*)


(* ::Section:: *)
(*Setup*)


(* ::Text:: *)
(*Setup can be run in two ways *)
(*1. Using a user interface*)
(*2. Programmatically*)
(*Both ways are shown below*)


(* ::Text:: *)
(*Note : To evaluate any function or expression in Mathematica place the cursor anywhere on the expression and use Shift + Enter*)


(* ::Subsection:: *)
(*1. Running setup through user interface*)


(* ::Text:: *)
(*For single fluid set field Input single fluid or mixture as {"nameoflfuid.fld"} and Input number of components will be 1*)
(*For mixtures, give the  Input single fluid or mixture as a list of strings. Example: {"fluid1.fld","fluid2.fld","fluid3.fld"},Input number of components is 3*)


(* ::Text:: *)
(*Evaluate RFPI[] function to bring up the interface. *)


(* ::Input:: *)
(*RFPI[]*)


(* ::Subsection:: *)
(*2. Programmatically - define models and initialize arrays*)


(* ::Input:: *)
(*(*For single fluid set fluidComponents as {"nameoflfuid.fld"}*)*)
(*(*For mixtures, give the components of the mixture as a list of string. Example: {"fluid1.fld","fluid2.fld","fluid3.fld"}*)*)


(* ::Input:: *)
(*dllpath="C:\\Program Files (x86)\\REFPROP\\REFPRP64.DLL";*)
(*fluidpath="C:\\Program Files (x86)\\REFPROP\\fluids";*)
(*fluidComponents={"nitrogen.fld"};*)
(*numberofComp=1;*)
(*refstate="DEF";*)
(*ierr=0;*)
(*herr=StringJoin[Table[" ",{256}]];*)
(**)


(* ::Input:: *)
(*(*A return value of 0 indicates successful setup*)*)


(* ::Input:: *)
(*setup[dllpath,fluidpath,fluidComponents,numberofComp,refstate,ierr,herr]*)


(* ::Section:: *)
(*Constants*)


(* ::Subsection:: *)
(*info - provides fluid constants for specified component*)


(* ::Input:: *)
(*wm=0.0;*)
(*ttp=0.0;*)
(*tnbp=0.0;*)
(*tc=0.0;*)
(*pc=0.0;*)
(*dc=0.0;*)
(*zc=0.0;*)
(*acf=0.0;*)
(*dip=0.0;*)
(*rgas=0.0;*)


(* ::Text:: *)
(*The output is a set of rules of pattern name->value*)


(* ::Input:: *)
(*const=info[wm,ttp,tnbp,tc,pc,dc,zc,acf,dip,rgas]*)


(* ::Text:: *)
(*info function can also be called by directly inputing numeric values as shown below:*)


(* ::Input:: *)
(*info[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]*)


(* ::Text:: *)
(*Use assignment below to save values in separate variable list*)


(* ::Input:: *)
(*{wm,ttp,tnbp,tc,pc,dc,zc,acf,dip,rgas}=const[[All,2]]*)


(* ::Input:: *)
(*wm*)


(* ::Input:: *)
(*const[[1]]*)


(* ::Section:: *)
(*Saturation state subroutine*)


(* ::Subsection:: *)
(*satp - iterate for saturated liquid and vapor states given pressure and the composition of one phase*)


(* ::Input:: *)
(*p=0.0;*)
(*x={0.7812,0.0092,0.2096};*)
(*i=2;*)
(*t=100.0;*)
(*dl=0.0;*)
(*dv=0.0;*)
(*xliq=Table[0.0,{20}];*)
(*xvap=Table[0.0,{20}];*)
(*ierr=0;*)
(*herr=StringJoin[Table[" ",{256}]];*)


(* ::Input:: *)
(*satp[p,x,i,t,dl,dv,xliq,xvap,ierr,herr]*)


(* ::Subsection:: *)
(*surft -  compute surface tension*)


(* ::Input:: *)
(*Clear[ierr,herr]*)


(* ::Input:: *)
(*t=100.0;*)
(*dl=0.0;*)
(*x={0.7812,0.0092,0.2096};*)
(*sigma=0.0;*)
(*ierr=0;*)
(*herr=StringJoin[Table[" ",{256}]];*)


(* ::Input:: *)
(*surft[t,dl,x,sigma,ierr,herr]*)


(* ::Section:: *)
(*Single phase flash subroutine*)


(* ::Subsection:: *)
(*tprho -iterate for density as a function of temperature, pressure, and composition for a specified phase*)


(* ::Input:: *)
(*t=300.0;*)
(*p=20000.0;*)
(*j=1;*)
(*x={0.7812,0.0092,0.2096};*)
(*tmpint=0;*)
(*d=0.0;*)
(*ierr=0;*)
(*herr=StringJoin[Table[" ",{256}]];*)


(* ::Input:: *)
(*tprho[t,p,x,j,tmpint,d,ierr,herr]*)


(* ::Section:: *)
(*General flash subroutines*)


(* ::Subsection:: *)
(*tpflsh,tdflsh,pdflsh*)


(* ::Input:: *)
(*t=100.0;*)
(*p=20000.0;*)
(*x={0.7812,0.0092,0.2096};*)
(*d=0.0;*)
(*dl=0.0;*)
(*dv=0.0;*)
(*xliq=Table[0.0,{20}];*)
(*xvap=Table[0.0,{20}];*)
(*q=0.0;*)
(*e=0.0;*)
(*h=0.0;*)
(*s=0.0;*)
(*cv=0.0;*)
(*cp=0.0;*)
(*w=0.0;*)
(*ierr=0;*)
(*herr=StringJoin[Table[" ",{256}]];*)
(*pp=0.0;*)
(*tt=0.0;*)
(**)


(* ::Input:: *)
(*tpflsh[t,p,x,d,dl,dv,xliq,xvap,q,e,h,s,cv,cp,w,ierr,herr]*)


(* ::Input:: *)
(*tdflsh[t,d,x,pp,dl,dv,xliq,xvap,q,e,h,s,cv,cp,w,ierr,herr]*)


(* ::Input:: *)
(*pdflsh[p,d,x,tt,dl,dv,xliq,xvap,q,e,h,s,cv,cp,w,ierr,herr]*)


(* ::Section:: *)
(*Transport property subroutine*)


(* ::Subsection:: *)
(*trnprp - compute the transport properties of thermal conductivity and viscosity as functions of temperature, density, and composition*)


(* ::Input:: *)
(*t=100.0;*)
(*d=0.0;*)
(*x={0.7812,0.0092,0.2096};*)
(*eta=0.0;*)
(*tcx=0.0;*)
(*ierr=0;*)
(*herr=StringJoin[Table[" ",{256}]];*)
(**)


(* ::Input:: *)
(*trnprp[t,d,x,eta,tcx,ierr,herr]*)


(* ::Section:: *)
(*Miscellaneous properties*)


(* ::Subsection:: *)
(*meltt - compute the melting line pressure as a function of temperature and composition.*)


(* ::Input:: *)
(*t=100.0;*)
(*d=0.0;*)
(*de=0.0;*)
(*x={0.7812,0.0092,0.2096};*)
(*p=20000.0;*)
(*ierr=0;*)
(*herr=StringJoin[Table[" ",{256}]];*)
(**)


(* ::Input:: *)
(*meltt[t,x,p,ierr,herr]*)


(* ::Subsection:: *)
(*dielec - compute the dielectric constant as a function of temperature, density,*)
(*and composition.*)


(* ::Input:: *)
(*dielec[t,d,x,de]*)


(* ::Section:: *)
(*Thermodynamic property subroutines*)


(* ::Subsection:: *)
(*therm- compute thermal quantities as a function of temperature, density,and compositions*)
(*press- compute pressure as a function of temperature, density, and composition using core functions*)
(*fgcty - compute fugacity for each of the nc components of a mixture*)
(*virb - compute second virial coefficient as a function of temperature and composition.*)
(*virc -  compute the third virial coefficient as a function of temperature and composition*)


(* ::Input:: *)
(*t=100.0;*)
(*d=0.0;*)
(*x={0.7812,0.0092,0.2096};*)
(*p=20000.0;*)
(*e=0.0;*)
(*h=0.0;*)
(*s=0.0;*)
(*cv=0.0;*)
(*cp=0.0;*)
(*w=0.0;*)
(*hjt=0.0;*)
(*f=Table[0.0,{20}];*)
(*b=0.0;*)
(*c=0.0;*)


(* ::Input:: *)
(*therm[t,d,x,p,e,h,s,cv,cp,w,hjt]*)


(* ::Input:: *)
(*press[t,d,x,p]*)


(* ::Input:: *)
(*fgcty[t,d,x,f]*)


(* ::Input:: *)
(*virb[t,x,b]*)


(* ::Input:: *)
(*virc[t,x,c]*)


(* ::Section:: *)
(*Derivatives*)


(* ::Subsection:: *)
(*dpdd- compute partial derivative of pressure w.r.t. density at constant temperature as a function of temperature, density, and composition*)
(*dpdd2- compute second partial derivative of pressure w.r.t. density at const temperature as a function of temperature, density, and composition*)
(*dpdt- compute partial derivative of pressure w.r.t. temperature at constant density as a function of temperature, density, and composition*)
(*dhdl- compute partial derivatives of enthalpy w.r.t. t, p, or rho at constant t, p, or rho as a function of temperature, density, and composition*)


(* ::Input:: *)
(*t=100.0;*)
(*d=0.0;*)
(*x={0.7812,0.0092,0.2096};*)
(*dpdrho=0.0;*)
(*d2pdd2=0.0;*)
(*dpdtvar=0.0;*)
(*dhdtd=0.0;*)
(*dhdtp=0.0;*)
(*dhddt=0.0;*)
(*dhddp=0.0;*)
(*dhdpt=0.0;*)
(*dhdpd=0.0;*)


(* ::Input:: *)
(*dpdd[t,d,x,dpdrho]*)


(* ::Input:: *)
(*dpdd2[t,d,x,d2pdd2]*)


(* ::Input:: *)
(*dpdt[t,d,x,dpdtvar]*)


(* ::Input:: *)
(*dhdl[t,d,x,dhdtd,dhdtp,dhddt,dhddp,dhdpt,dhdpd]*)
