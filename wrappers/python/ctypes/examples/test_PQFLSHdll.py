"""
A simpified example of calling the PQFLSH function from
the legacy version <10.0 API

By Ian Bell, 2018, ian.bell@nist.gov
"""
# Standard library imports
import os
import sys

# pip installable packages
from ctREFPROP.ctREFPROP import REFPROPFunctionLibrary

root = os.environ['RPPREFIX']
R = REFPROPFunctionLibrary(root)
R.SETPATHdll(root)

ierr, herr = R.SETUPdll(1,'WATER.FLD','HMX.BNC','DEF')
if ierr != 0:
    raise ValueError(str(ierr) + ':' + herr)

z = [1.0]
p = 101.325
Q = 1.0
o = R.PQFLSHdll(p,Q,z,1)
print(o)
