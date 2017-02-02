import os
import glob
import subprocess
import sys
from numpy.f2py import f2py2e

FORTRAN_ROOT = os.path.abspath('..')
FORTRAN_ROOT = r'R:/914FILES'

interface_files = ['PASS_FTN.FOR']
interface_file_strings = [os.path.join(FORTRAN_ROOT, f) for f in interface_files]
args = ['python','-m','numpy.f2py','--quiet','-h','REFPROP.pyf','-m','REFPROP','--overwrite-signature'] + interface_file_strings
subprocess.call(args, shell = True)

source_files = glob.glob(os.path.join(FORTRAN_ROOT, "*.FOR"))
args = ['python','-m','numpy.f2py','-c','--f90flags="-fno-range-check -ffree-line-length-none"','--fcompiler=gnu95','--compiler=mingw32','REFPROP.pyf'] + source_files
subprocess.call(args, shell = True)