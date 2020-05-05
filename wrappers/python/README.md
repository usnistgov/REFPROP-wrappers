# Python wrappers

Due to the fact that Python is the primary working environment for one of the main REFPROP developers (Ian Bell), the support of REFPROP in Python is quite robust.  

## ctypes wrapper

There is a [ctypes](https://docs.python.org/3/library/ctypes.html) wrapper of REFPROP available, this wrapper is automatically generated for all functions exported from the DLL.  If you notice any inconsistencies between the DLL and the functions in the wrapper, please [open an issue](https://github.com/usnistgov/REFPROP-wrappers/issues/new) ASAP.   It sits in the ``ctypes`` folder relative to this folder.  It can be installed from PYPI with:

```
pip install ctREFPROP
```

Mini-tutorial available as a jupyter notebook: [Link to tutorial](https://nbviewer.jupyter.org/github/usnistgov/REFPROP-wrappers/blob/master/wrappers/python/notebooks/Tutorial.ipynb) .  The notebook can be accessed at [wrappers/python/notebooks](https://github.com/usnistgov/REFPROP-wrappers/tree/master/wrappers/python/notebooks)

Example code to print the normal boiling point temperature of propane using the interface in REFPROP 10 (see below if you use REFPROP 9.x):

``` python
import os, numpy as np
from ctREFPROP.ctREFPROP import REFPROPFunctionLibrary

def NBP():
    RP = REFPROPFunctionLibrary(os.environ['RPPREFIX'])
    RP.SETPATHdll(os.environ['RPPREFIX'])
    print(RP.RPVersion())
    MOLAR_BASE_SI = RP.GETENUMdll(0,"MOLAR BASE SI").iEnum

    r = RP.REFPROPdll("PROPANE","PQ","T",MOLAR_BASE_SI, 0,0,101325, 0, [1.0])
    print(r.ierr, r.herr, r.Output[0])

if __name__=='__main__':
    # If the RPPREFIX environment variable is not already set by your installer (e.g., on windows), 
    # then uncomment this line and set the absolute path to the location of your install of 
    # REFPROP
    # os.environ['RPPREFIX'] = r'D:\Code\REFPROP-cmake\build\10\Release\\'
    
    # Print the version of REFPROP in use and the NBP
    NBP()
    
```

or the same example with the legacy API from REFPROP 9.1:

``` python
import os, numpy as np
from ctREFPROP.ctREFPROP import REFPROPFunctionLibrary

def NBP():
    RP = REFPROPFunctionLibrary(os.environ['RPPREFIX'])
    print(RP.RPVersion())
    RP.SETPATHdll(os.environ['RPPREFIX'])
    r = RP.SETUPdll(1,"PROPANE.FLD","HMX.BNC","DEF")
    assert(r.ierr == 0)
    print(RP.PQFLSHdll(101.325, 0, [1.0], 0))

if __name__=='__main__':
    # Print the version of REFPROP in use and the NBP
    NBP()
```

Some important notes:

* The wrapper is single-threaded. It holds a pointer to an instance of REFPROP, and on windows, if multiple loads of the library are done, each reference points to the same instance.  As a result, you need to use separate *processes* to use multiple copies of REFPROP in parallel.  A stripped down example of the use of multiple processes (and therefore multiple copies of REFPROP) is provided in the file ``multiprocessing_example.py`` in the ``examples`` folder.
* There are examples of calling the ctREFPROP interface in the examples folder: https://github.com/usnistgov/REFPROP-wrappers/tree/master/wrappers/python/ctypes/examples
* You can regenerate the interface with the ``build_wrapper.py`` script, for instance to build the wrapper for an older version of REFPROP.  Requirements: ``numpy`` (for ``f2py``)
* While you can just copy-paste the ``ctREFPROP.py`` script to your desired location, or anywhere that is on ``sys.path``, you can also run the ``setup.py`` script in the ``ctypes`` folder to install the interface to the default package location on your computer via ``python setup.py install`` in the ``ctypes`` folder.

## Parallel calls

As mentioned above, calls to REPROP are NOT(!) threadsafe.  Therefore to call in parallel, you must fork threads, load the fluids in each thread in serial, and then make your calls in parallel.  Here is an example in Python with all the bits and pieces you'll need:

``` python
import timeit

# Important!
# This import and setup will happen inside the process
from ctREFPROP.ctREFPROP import REFPROPFunctionLibrary
import CoolProp.CoolProp as CP
root = 'D:/Program Files (x86)/REFPROP' # Change this path as needed
RP = REFPROPFunctionLibrary(root)
RP.SETPATHdll(root)

def set_fluids(names):
    """ This happens inside the process that is forked, when it is initialized. IMPORTANTLY, IN SERIAL!! """
    RP.SETFLUIDSdll('*'.join(names))

def calc1(inputs):
    """
    Do the calculation for one mixture
    """
    T,p,z = inputs
    return RP.REFPROPdll('','TP','D',RP.MOLAR_BASE_SI,0,0,T,p,z).Output[0]

def calc_all(mapfcn):
    """
    Using the function passed in, evaluate all the inputs
    """
    inputs = []
    for i in range(10):
        T = 308.3084286
        p = 397780
        z = [0.23328,0.13008,0.05558,0.09246,0.01915,0.04628,0.02459,0.00474,0.00472,0.0046,0.0062,0.00426,0.00298,0.00614,0.00584,0.07216,0.00886,0.27808]
        inputs.append((T,p,z))
    return mapfcn(calc1, inputs) # calc1 is the function to be called, inputs the inputs

if __name__=='__main__':
    
    # Start up the timer
    tic = timeit.default_timer()

    # 1. Set up the map
    # Either comment out A or B, and if you use multiple cores, you may need to change Nprocesses

    names = [
    "Methane","Ethane","Ethylene","Propane","Propylene","Isobutane","Butane","trans-Butene",
    "Butene","Isobutene","Isopentane","Pentane","Pentene","Carbon dioxide","Oxygen","Nitrogen",
    "Carbon monoxide","Hydrogen"
    ]

    # A) Leave this section uncommented to use a serial evaluation
    # mapfcn = map; set_fluids(names)

    # B) Leave this section uncommented to carry out the calculations in parallel
    import multiprocessing
    Nprocesses = multiprocessing.cpu_count()-1
    p = multiprocessing.Pool(Nprocesses, initializer = set_fluids, initargs = (names,))
    mapfcn = p.map

    # Do the calculations
    print(calc_all(mapfcn))

    toc = timeit.default_timer()
    print('Took {0:g} seconds to carry out the evaluations w/ {1:d} processes'.format(toc-tic, Nprocesses))
```

## Other python wrappers of REFPROP:

These wrappers are all community developed/supported:

* [CoolProp wrapper](http://www.coolprop.org/coolprop/wrappers/Python/index.html#python)