"""
An example of the use of the multiprocessing library in concert with the 
ctREFPROP wrapper to evaluate many thermodynamic state points

By Ian Bell, NIST, 2018 (ian.bell@nist.gov)
"""

# Python standard library
import os, timeit
from multiprocessing import Pool

# Pip installable packages
from ctREFPROP.ctREFPROP import REFPROPFunctionLibrary

# Conda packages
import numpy as np

def parallel_evaluate(mixtures):
    """ 
    Fully encapsulated for multiprocessing support 
    Fresh copy of REFPROP will be loaded into each instance, for parallelism
    """
    root = os.environ['RPPREFIX']
    R = REFPROPFunctionLibrary(os.path.join(root, 'REFPRP64.dll')) # Change this as needed
    R.SETPATHdll(root)
    out = []
    names, compositions = mixtures
    ierr, herr = R.SETUPdll(2, '|'.join([f+'.FLD' for f in names]), 'HMX.BNC', 'DEF')
    if ierr != 0:
        print(ierr, herr)
        return herr
    
    kphase = 2 # Vapor
    kguess = 0 # Don't use guesses
    rho, ierr, herr = R.TPRHOdll(300.0, 10.0, compositions, kphase, kguess,-1)
    out.append(rho)
    return out 
            
if __name__=='__main__':
    
    # Build the inputs
    inputs = []
    pairs = [['METHANE','ETHANE'],['PROPANE','DECANE']]
    for pair in pairs:
        for z0 in np.linspace(0,1,30):
            inputs.append((pair, np.array([z0,1-z0])))

    # Serial evaluation
    tic = timeit.default_timer()
    outserial = list(map(parallel_evaluate, inputs))
    toc = timeit.default_timer()
    print(toc-tic, 's for serial evaluation')

    # Parallel evaluation
    p = Pool(4)
    tic = timeit.default_timer()
    outparallel = p.map(parallel_evaluate, inputs)
    toc = timeit.default_timer()
    print(toc-tic, 's for parallel evaluation')