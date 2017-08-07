import numpy as np

from multiprocessing import Pool
from ctREFPROP.ctREFPROP import REFPROPFunctionLibrary

import os

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
    outserial = list(map(parallel_evaluate, inputs))

    # Parallel evaluation
    p = Pool(5)
    outparallel = p.map(parallel_evaluate, inputs)
    print(outparallel)