c     Example program that uses the routines in REFPROP.
c
c     Refprop uses a default set of units, these being:
c       K, kPa, mol/dm^3, mole fraction, J/mol, J/mol-K, m/s, uPa-s, W/m-K, and N/m
c
c     Refprop 10 comes with two new subroutines, labeled as ALLPROPS and REFPROP,
c       that allow the user to access nearly all properties in the Refprop program.
c       One additional routine allows the setting of options and flags to change
c       how the program works.  The details of these routines are given at the top
c       of the file REFPROP.FOR for the first two, and at the bottom for the last one.
c       Examples are given below.
c
c     The examples here are split into two pieces.  The first shows the new method
c       to obtain properties.  The second shows the old method.  The method to use
c       depends on your needs.  For simple calculations, then the use of the
c       techniques in the first method works very well.  For applications
c       that require heavy computational needs, the second method will be faster
c       by calling directly the routines needed due to the higher overhead required
c       in the ALLPROPS and REFPROP routines to search for the correct routines
c       to call for each calculation.

      program EXAMPLE
      implicit double precision (a-h,o-z)
      implicit integer (i-k,m,n)
      parameter (ncmax=150)     !Maximum number of components in the mixture (do not change unless the Refprop Fortran code is also changed with the same number in the CONSTS.INC file)
      parameter (ipropmax=200) !Number of output properties available in ALLPROPS (also do not change).
      character*255 hOut,hUnits,herr,hUArray(ipropmax)*50
      dimension Output(ipropmax),iUArray(ipropmax)
      dimension z(ncmax),x(ncmax),y(ncmax),x3(ncmax)
      dimension ymass(ncmax),ymole(ncmax)
      o=1000.d0


c***New methods for obtaining properties from the REFPROP and ALLPROPS subroutines***  (see notes at top of file)

c...Get the DLL number of the REFPROP DLL.
      call REFPROP (' ',' ','DLL#',0,0,0,0d0,0d0,z,
     &              Output,hUnits,i,x,y,x3,q,ierr,herr)

c...When Refprop was installed, an environmental variable called RPPrefix was setup so that Refprop can find
c   the fluid files.  To manually set the path to these files, use the following lines.
c     call REFPROP ('c:\Program Files (x86)\Refprop','Path',' ',
c    &       0,0,0,0d0,0d0,z,Output,hUnits,iUnit,x,y,x3,q,ierr,herr)
c     if (ierr.gt.0) write (*,*) herr

c...Load nitrogen as a pure fluid (this can also be done in the call to REFPROP).
      call SETFLUIDS ('NITROGEN',ierr)
      call ERRMSG (ierr,herr)  !If the error message is desired, call ERRMSG with the value of ierr returned from the line above.
      if (ierr.gt.0) write (*,*) herr

c...Calculate the density, isobaric heat capacity, viscosity, and thermal conductivity at a given
c...temperature and pressure with the REFPROP subroutine.
      T=300   !K
      P=1     !MPa
      iU=2    !Units in mass SI (see the REFPROP.FOR file for a complete list of the unit systems available).
      call REFPROP (' ','TP','D,Cp,Vis,Tcx',iU,iMass,0,T,P,z,
     &              Output,hUnits,iUnit,x,y,x3,q,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,*)    '                  ierr Units'
      write (*,1000) 'T,P,D,Cp,Vis,Tcx    ',ierr,iU,T,P,Output(1:4)
      D  =Output(1)   !Density - kg/m^3
      Cp =Output(2)   !Cp - kJ/(kg-K)
      Eta=Output(3)   !Viscosity - uPa-s
      Tcx=Output(4)   !Thermal conductivity - mW/(m-K)

c...Calculate the specific volume at a given temperature and pressure with the REFPROP1 subroutine
c...in English units and composition in mass fractions for 70% methane and 30% ethane.
      call SETFLUIDS ('METHANE;ETHANE',ierr)
      if (ierr.gt.0) write (*,*) herr
      z(1)=0.7d0
      z(2)=0.3d0
      iMass=1 !Mass fractions for composition
      iU=5    !Flag to use English units
      T=50    !F
      P=100   !psia
      call REFPROP1 ('TP','V',iU,iMass,T,P,z,V,q,ierr,herr)    !Volume in ft^3/lbm
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'T,P,V               ',ierr,iU,T,P,V

c...Calculate other thermodynamic properties with a call to ALLPROPS now that
c...the temperature and density are known.  Use molar SI as input units.
      call REFPROP1 (' ','m',0,iMass,0d0,0d0,z,wm,q,ierr,herr)        !Get the molar mass.
      call REFPROP ('F','K','UNITCONV_T',
     &         0,0,0,T,0d0,z,Output,hUnits,iUnit,x,y,x3,q,ierr,herr)  !Convert the temperature from F to K.
      T=Output(1)
      call REFPROP ('ft^3/lbm','mol/dm^3','UNITCONV_D',               !Convert the volume in ft^3/lbm to density in mol/dm^3.
     &         0,0,0,V,wm,z,Output,hUnits,iUnit,x,y,x3,q,ierr,herr)   !The molar mass must be sent since this is a conversion from mass to molar units.
      D=Output(1)

      iU=1     !Molar SI
      iMass=1  !Mass fractions
      hout='Z,s,Cv,Cp,W'
      call ALLPROPS(hOut,iU,iMass,1,T,D,z,Output,hUArray,iUArray,i,herr)
      if (i.gt.0) write (*,*) herr
      write (*,1000) 'D,Z,s,Cv,Cp,w     ',ierr,iU,D,Output(1:5)
      z=Output(1)  !Compressibility factor
      s=Output(2)  !Entropy - J/(mol-K)    This entropy will not match that from the GUI because the reference state applied to the mixture is set differently.  See the call to SETREF below to change this.
      w=Output(5)  !Speed of sound - m/s

c...Calculate the liquid saturation pressure and the mole and mass fractions in the vapor phase.
      call SETFLUIDS ('METHANE;ETHANE;PROPANE',ierr)  !Either this or the call to REFPROP in the next section is valid.
      iU=2   !Flag to use mass SI units, for SI units with celsius, use 3.
      iMass=1
      T=200  !K
      z(1)=0.8d0
      z(2)=0.15d0
      z(3)=0.05d0
      hOut='P,Dliq,Dvap,xmassliq,xmassvap,xmoleliq,xmolevap'   !The string "xmassvap" is the y mass fractions because of the "vap" appended to the end.  The string "xmass" simply means composition on a mass basis.
      call REFPROP (' ','Tliq',hOut,iU,iMass,0,T,0d0,
     &              z,Output,hUnits,iUnit,x,y,x3,q,ierr,herr)  !The liquid and vapor mole fractions are returned in the x and y arrays.  Note:  Always mole fractions for x, y, and x3.
      if (ierr.gt.0) write (*,*) herr
      P =Output(1)   !MPa
      Dl=Output(2)   !Saturated liquid density - kg/m^3
      Dv=Output(3)   !Saturated vapor density  - kg/m^3
      ymass(1:3)=Output(7:9)     !Because the xmass and xmole inputs to REFPROP have more than one output, the Output() array does not line up with the inputs.  Thus, the outputs for xmassliq are in slots 7 to 9, for xmassvap in 10 to 12, etc.
      ymole(1:3)=Output(13:15)   !The ymole outputs are in slots 13 to 15, but are actually the same as the values returned in the array y in the call to the REFPROP routine.  This example shows how the pieces are linked together.
      write (*,1000) 'P,Dl,Dv,ymass     ',ierr,iU,P,Dl,Dv,ymass(1:3)
      write (*,1000) '  xmole,ymole     ',ierr,iU,x(1:3),ymole(1:3)    !The x() array values returned from the REFPROP routine are the same as those in slots 10 to 12 for the xmoleliq string, and in this example are also the same as the input array z() except on a molar basis.

c...Setup a mixture of natural gas components and calculate the enthalpy given pressure and entropy.
      D=2     !kg/m^3
      iU=3    !Use mass SI with T in Celsius
      iMass=0 !Molar composition
      z(1)=.7d0
      z(2)=.2d0
      z(3)=.1d0
      call REFPROP ('METHANE;ETHANE;PROPANE','Dliq','T,P,S,H,W',
     &      iU,iMass,0,D,0d0,z,Output,hUnits,iUnit,x,y,x3,q,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'D,T,P,S,H,W       ',ierr,iU,D,Output(1:5)
      T=Output(1)   !C
      P=Output(2)   !MPa
      H=Output(4)   !kJ/kg

      stop

c   *** See the file MANUAL.TXT in the FORTRAN directory for more routines ***


c-----------------------------------------------------------------------
c   The code below uses the methods from 9.1 and before to calculate
c   properties.  For calculation speed, this is still the best method
c   to use.


c...Call SETREF to set the reference state.  Valid inputs are:
c           'NBP':  h,s=0 at normal boiling point(s)
c           'ASH':  h,s=0 for sat liquid at -40 C (ASHRAE convention)
c           'IIR':  h=200 kJ/kg, s=1.0 kJ/kg-K for sat liq at 0 C (IIR convention)
c           'DEF':  default reference state as specified in fluid file
c           'OTH':  other, as specified by h0, s0, t0, p0
c...Only need to call SETREF if 'DEF' state is not used.
c...This call sets h=2000 J/mol and s=100 J/mol-K at 298.15 K and 101.325 kPa
c     call SETREF ('OTH',1,z,2000.d0,100.d0,298.15d0,101.325d0,ierr,herr)
c...If the reference state for a mixture is to be set to 'Currently defined
c...mixture' (as done in the graphical interface), then call SETREF with this
c...(be sure that the composition array z is initialized):
c     call SETREF ('DEF',2,z,0.d0,0.d0,0.d0,0.d0,ierr,herr)


c...Get saturation properties given t,z; for i=1: z is liquid phase
c.....                                   for i=2: z is vapor phase
      i=1
      t=100
      call SATT (t,z,i,p,dl,dv,x,y,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'P,Dl,Dv,xl(1),xv(1)   ',ierr,p/o,dl,dv,x(1),y(1)
      i=2
      call SATT (t,z,i,p,dl,dv,x,y,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'P,Dl,Dv,xl(1),xv(1)   ',ierr,p/o,dl,dv,x(1),y(1)

c...Calculate saturation properties at a given p. i is same as SATT
      i=2
      call SATP (p,z,i,t,dl,dv,x,y,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'T,Dl,Dv,xl(1),xv(1)   ',ierr,t,dl,dv,x(1),y(1)

c...Other saturation routines are given in SAT_SUB.FOR

      t=300
      p=20000

c...Calculate d from t,p,z
c...If phase is known: (j=1: Liquid, j=2: Vapor)
      j=1
      call TPRHO (t,p,z,j,0,d,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'T,P,D                 ',ierr,t,p/o,d

c...If phase is not known, call TPFLSH
c...Calls to TPFLSH are much slower than TPRHO since SATT must be called first.
c.....(If two phase, quality is returned as q)
      call TPFLSH (t,p,z,d,dl,dv,x,y,q,e,h,s,cv,cp,w,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'T,P,D,H,CP            ',ierr,t,p/o,d,h,cp

c...Calculate pressure (p), internal energy (e), enthalpy (h), entropy (s),
c.....isochoric (cv) and isobaric (cp) heat capacities, speed of sound (w),
c.....and Joule-Thomson coefficient (hjt) from t,d,z
c.....(subroutines THERM2 and THERM3 contain more properties, see PROP_SUB.FOR)
      call THERM (t,d,z,p,e,h,s,cv,cp,w,hjt)

c...Calculate derivatives of enthalpy with respect to T, P, and D
      call DHD1(t,d,z,dhdt_d,dhdt_p,dhdd_t,dhdd_p,dhdp_t,dhdp_d)
      write (*,1000) 'Enthalpy derivatives  ',ierr,
     &       dhdt_d,dhdt_p,dhdd_t,dhdd_p/1000.d0,dhdp_t

c...Calculate surface tension
      tt=100
      call SURFT (tt,dl,z,sigma,ierr,herr)
      write (*,1000) 'T,SURF. TN.           ',ierr,tt,sigma

c...Calculate viscosity (eta) and thermal conductivity (tcx)
      call TRNPRP (t,d,z,eta,tcx,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'VIS.,TH.CND.          ',ierr,eta,tcx*1000.D0

c...General property calculation with inputs of t,d,z
      call TDFLSH (t,d,z,pp,dl,dv,x,y,q,e,h1,s,cv,cp,w,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'T, D, P from TDFLSH   ',ierr,t,d,pp/o

c...General property calculation with inputs of p,h,z
      call PHFLSH (p,h,z,tt,dd,dl,dv,x,y,q,e,s,cv,cp,w,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'T, D, P from PHFLSH   ',ierr,tt,dd,p/o

c...General property calculation with inputs of t,h,z
c     kr--flag specifying desired root for multi-valued inputs:
c         1=return lower density root
c         2=return higher density root
      kr=1
      call THFLSH (t,h,z,
     &             kr,pp,dd,dl,dv,x,y,q,e,s,cv,cp,w,ierr,herr)
      if (ierr.gt.0) write (*,*) herr
      write (*,1000) 'T, D, P from THFLSH   ',ierr,t,dd,pp/o

c   *** See the file MANUAL.TXT in the FORTRAN directory for more routines ***

      stop
 1000 format (1x,a18,i4,i3,25f9.4)
      end
