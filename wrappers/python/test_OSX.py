import os
import sys
from ctypes_wrapper import REFPROPFunctionLibrary

root = os.environ['RPPREFIX']
R = REFPROPFunctionLibrary(os.path.join(root, 'librefprop.dylib'))
R.SETPATHdll(root)
ierr,herr = R.SETUPdll(1,'WATER.FLD','HMX.BNC','DEF')
if ierr != 0:
	raise ValueError(str(ierr) + ':' + herr)
z = [1.0]
T = 300
Q = 1.0
o = R.TQFLSHdll(T,Q,z)
print(o)