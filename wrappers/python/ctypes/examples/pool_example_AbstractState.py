"""
This script demonstrates how one could call REFPROP via CoolProp's 
AbstractState interface. As always, you need to have a significant enough amount 
of work to be done for parallelism to make sense. The overhead of calling
with processes is decidedly non-trivial. Doing this in C++ is much faster, and the 
scaling is much better, but alas REFPROP is not thread-safe, so if you wanted to 
do a similar exercise in C++ you would need to use REFPROP-manager and keep 
multiple copies of REFPROP in memory.
"""

import timeit, os

import numpy as np
import CoolProp.CoolProp as CP

# This is a placeholder variable that will be instantiated in each process
AS = None

Ncalls = 10000

from dataclasses import dataclass
from typing import List

@dataclass
class Inputs:
    T: float
    p: float
    z: List

def set_fluids(names: List[str]):
    """ This happens inside the process that is forked, when it is initialized. IMPORTANTLY, IN SERIAL!! """
    global AS
    AS = CP.AbstractState('REFPROP', '&'.join(names))

def calc1(arg: Inputs):
    """
    Do the calculation for one mixture
    """
    AS.set_mole_fractions(arg.z)
    AS.update(CP.PT_INPUTS, arg.p, arg.T)
    return AS.rhomolar()

if __name__=='__main__':
    
    # 1. Set up the map
    # Either comment out A or B, and if you use multiple cores, you may need to change Nprocesses

    names = [
    "Methane","Ethane","Ethylene","Propane","Propylene","Isobutane","Butane","trans-Butene",
    "Butene","Isobutene","Isopentane","Pentane","Pentene","Carbon dioxide","Oxygen","Nitrogen",
    "Carbon monoxide","Hydrogen"
    ]
    inputs = []
    for i in range(Ncalls):
        T = 308.3084286
        p = 397780+i
        z = [0.23328,0.13008,0.05558,0.09246,0.01915,0.04628,0.02459,0.00474,0.00472,0.0046,0.0062,0.00426,0.00298,0.00614,0.00584,0.07216,0.00886,0.27808]
        inputs.append(Inputs(T=T,p=p,z=z))

    # A) Leave this section uncommented to use a serial evaluation
    mapfcn = map; set_fluids(names)

    # Start up the timer
    tic = timeit.default_timer()

    # Do the calculations in serial
    outserial = np.array(list(mapfcn(calc1, inputs)))

    toc = timeit.default_timer()
    print('Took {0:g} seconds to carry out the evaluations in serial'.format(toc-tic))

    print('First 6 returned values:', outserial[0:6].tolist())

    # B) Leave this section uncommented to carry out the calculations in parallel
    import concurrent.futures, multiprocessing, functools
    Nprocesses = multiprocessing.cpu_count()-1

    # Start up the timer
    tic = timeit.default_timer()

    with concurrent.futures.ProcessPoolExecutor(max_workers=Nprocesses, initializer=set_fluids, initargs=(names,)) as executor:
        # Wrap the map function of the executor to pass also the chunksize argument
        m = functools.partial(executor.map, chunksize=Ncalls//Nprocesses)
        
        # Do the calculations
        outparallel = np.array(list(m(calc1, inputs)))

    toc = timeit.default_timer()
    print('Took {0:g} seconds to carry out the evaluations w/ {1:d} processes'.format(toc-tic, Nprocesses))

    print('First 6 returned values:', outparallel[0:6].tolist())