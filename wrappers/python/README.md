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

## Other python wrappers of REFPROP:

These wrappers are all community developed/supported:

* [CoolProp wrapper](http://www.coolprop.org/coolprop/wrappers/Python/index.html#python)