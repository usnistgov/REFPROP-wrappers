c     Example program using routines in REFPROP.
c     This example shows how to use the much faster pseudo-pure fluid (PPF)
c     equations of state (see Lemmon, Int. J. Thermophys., 24:991, 2003)
c     in mixture situations where transport routines are also required.
c     Because transport routines have not been added to the PPF files, one
c     must load in the full mixture as well as the PPF file.  The subroutine
c     PUREFLD then allows flipping back and forth between the PPF file and
c     the full mixture setup.
c       Units are K, kPa, mol/dm^3, mole fraction, J/mol, J/mol-K, m/s
c       uPa-s, W/m-K, N/m

      program PPF_EX
      implicit double precision (a-h,o-z)
      implicit integer (i-k,m,n)
      parameter (ncmax=20)   !max number of components in mixture
      dimension x(ncmax),xliq(ncmax),xvap(ncmax)
      character hrf*3, herr*255
      character*255 hf(ncmax),hfmix

c...Load in the full mixture setup for R407C, as well as the PPF file.
      nc=4
      hf(1)='R32.FLD'
      hf(2)='R125.FLD'
      hf(3)='R134A.FLD'
      hf(4)='R407C.PPF'
      hfmix='hmx.bnc'
      hrf='DEF'
      call SETUPdll(nc,hf,hfmix,hrf,ierr,herr)
      if (ierr.ne.0) write (*,*) herr
      x(1)=.381109419953993   !These compositions for R407C were taken
      x(2)=.179558888662016   !from the R407C.MIX file.
      x(3)=.439331691383991
      x(4)=0                  !The composition should be set to zero since
                              !this is not part of the mixture.

c...Change the number of components to 3 so that the program will not try
c...to access the coefficients for the PPF file.
      call SETNCdll(3)

c...Call PUREFLD to calculate thermodynamic properties from the PPF file.
c...This shuts down any calculation using values from R32+R125+R134a, and
c...uses the compact formulation for R407C only to get the thermo properties.
      call PUREFLDdll(4)

c...For saturation states, SATT or SATP must be called once for the liquid
c.....phase and once for the vapor phase since composition is not part of the
c.....PPF equation of state
c...Call SATT to get liquid phase pressure and density at 300 K:
      t=300.d0
      i=1
      call SATTdll(t,x,i,p,dl,dv,xliq,xvap,ierr,herr)
      write (*,1000) 'P,Dl,Dv,xl(1),xv(1)   ',p,dl,dv,xliq(1),xvap(1)
c...Call SATT to get vapor phase pressure and density at 300 K:
      i=2
      call SATTdll(t,x,i,p,dl,dv,xliq,xvap,ierr,herr)
      write (*,1000) 'P,Dl,Dv,xl(1),xv(1)   ',p,dl,dv,xliq(1),xvap(1)

      t=300
      p=20000

c...call TPFLSH to get density from temperature and pressure
      call TPFLSHdll(t,p,x,d,dl,dv,xliq,xvap,q,e,h,s,cv,cp,w,ierr,herr)
      write (*,1000) 'T,P,D,H,CP            ',t,p,d,h,cp

c...To calculate transport properties, switch back to the mixture because
c...the PPF file does not yet contain transport equations.
      call PUREFLDdll(0)

c...Calculate viscosity (eta) and thermal conductivity (tcx)
      call TRNPRPdll(t,d,x,eta,tcx,ierr,herr)
      write (*,1000) 'VIS.,TH.CND.          ',eta,tcx*1000.D0

      stop
 1000 format (1x,a22,5f11.4)
      end
