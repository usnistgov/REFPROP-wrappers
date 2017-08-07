"""
Important note on windows: you may need to manually copy the libmsvcrXXX.a files to libmsvcrXXXd.a 
"""
import os
import glob
import sys

from subprocess import Popen, PIPE
import subprocess

FORTRAN_ROOT = os.path.abspath('..')
FORTRAN_ROOT = r'D:\Code\REFPROP-cmake\FORTRANsrc'

# Call f2py to generate .pyf file
interface_files = ['PASS_FTN.FOR']
interface_file_strings = [os.path.join(FORTRAN_ROOT, f) for f in interface_files]
print('Writing the .pyf file with numpy.f2py, please be patient...')
print(interface_file_strings)
args = ['python','-m','numpy.f2py','--quiet','-h','f2pyREFPROP.pyf','-m','f2pyREFPROP','--overwrite-signature'] + interface_file_strings
p = Popen(args, stdin=PIPE, stdout=PIPE, stderr=PIPE)
output, err = p.communicate()
rc = p.returncode
with open('stdout.txt','w') as fp:
    fp.write(output.decode('utf-8'))
with open('stderr.txt','w') as fp:
    fp.write(err.decode('utf-8'))

source_files = glob.glob(os.path.join(FORTRAN_ROOT, "*.FOR"))
args = ['python','-m','numpy.f2py','-c','--f90flags="-fno-range-check -ffree-line-length-none"','--fcompiler=gnu95','--compiler=mingw32','f2pyREFPROP.pyf'] + source_files
subprocess.call(args, shell = True)