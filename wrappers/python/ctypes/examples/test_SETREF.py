"""
An example of calling the legacy API of REFPROP 

By Ian Bell, NIST, 2018, ian.bell@nist.gov

temperature                     K
pressure, fugacity              kPa
density                         mol/L
composition                     mole fraction
quality                         mole basis (moles vapor/total moles)
enthalpy, internal energy       J/mol
Gibbs, Helmholtz free energy    J/mol
entropy, heat capacity          J/(mol.K)
speed of sound                  m/s
Joule-Thomson coefficient       K/kPa
d(p)/d(rho)                     kPa.L/mol
d2(p)/d(rho)2                   kPa.(L/mol)^2
viscosity                       microPa.s (10^-6 Pa.s)
thermal conductivity            W/(m.K)
dipole moment                   debye
surface tension                 N/m
"""

from __future__ import print_function

# Standard library imports
import os, sys

# pip installable imports
import ctREFPROP.ctREFPROP as ct

r = ct.REFPROPFunctionLibrary(os.environ['RPPREFIX'],'dll')
r.SETPATHdll(os.environ['RPPREFIX'])

z = [0.788840469789739, 0.209691770450437, 0.001467759759824] + [0]*17

T = 323.4354576 # K
p = 0.094743763   # kPa

ierr, herr = r.SETUPdll(3, 'NITROGEN.FLD|OXYGEN.FLD|WATER.FLD', 'HMX.BNC', 'DEF')
if ierr > 0:
    print(ierr, herr)
    sys.exit()
ierr,herr = r.SETREFdll("DEF",1,z,0,0,0,0)
if ierr > 0:
    print(ierr, herr)
    sys.exit()
D, Dl, Dv, x, y, q, e, h, s, Cv, Cp, w, ierr, herr = r.TPFLSHdll(T,p,z)
if ierr > 0:
    print(ierr,herr)
    sys.exit()
else:
    wm = r.WMOLdll(z)
    print('wm',wm,'kg/kmol')
    print('h',h/wm,'kJ/kg')
    print('s',s,'J/(mol*K)')
    print('s',s/wm,'kJ/(kg*K)')