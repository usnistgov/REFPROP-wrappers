# Python wrappers

Due to the fact that Python is the primary working environment for one of the main REFPROP developers (Ian Bell), the support of REFPROP in Python is quite robust.  

## ctypes wrapper

There is a [ctypes](https://docs.python.org/3/library/ctypes.html) wrapper of REFPROP available, this wrapper is automatically generated for all functions exported from the DLL.  If you notice any inconsistencies between the DLL and the functions in the wrapper, please [open an issue](https://github.com/usnistgov/REFPROP-wrappers/issues/new) ASAP.   It sits in the ``ctypes`` folder relative to this folder.  It can be installed from PYPI with:

```
pip install ctREFPROP
```

Example code to print the normal boiling point temperature of propane:
``` python
import os, numpy as np
from ctREFPROP.ctREFPROP import REFPROPFunctionLibrary

if __name__=='__main__':
    # If needed...
    os.environ['RPPREFIX'] = r'D:\Code\REFPROP-cmake\build\10\Release\\'
    
    RP = REFPROPFunctionLibrary(os.environ['RPPREFIX'])
    print(RP.RPVersion())
    MOLAR_BASE_SI = RP.GETENUMdll(0,"MOLAR BASE SI").iEnum

    r = RP.REFPROPdll("PROPANE","PQ","T",MOLAR_BASE_SI, 0,0,101325, 0, [1.0])
    print(r.ierr, r.herr, r.Output[0])
```

Some important notes:

* The wrapper is single-threaded. It holds a pointer to an instance of REFPROP, and on windows, if multiple loads of the library are done, each reference points to the same instance.  As a result, you need to use separate *processes* to use multiple copies of REFPROP in parallel.  A stripped down example of the use of multiple processes (and therefore multiple copies of REFPROP) is provided in the file ``multiprocessing_example.py`` in the ``examples`` folder.
* There are examples of calling the ctREFPROP interface in the examples folder: https://github.com/usnistgov/REFPROP-wrappers/tree/master/wrappers/python/ctypes/examples
* You can regenerate the interface with the ``build_wrapper.py`` script, for instance to build the wrapper for an older version of REFPROP.  Requirements: ``numpy`` (for ``f2py``)
* While you can just copy-paste the ``ctREFPROP.py`` script to your desired location, or anywhere that is on ``sys.path``, you can also run the ``setup.py`` script in the ``ctypes`` folder to install the interface to the default package location on your computer via ``python setup.py install`` in the ``ctypes`` folder.

## Other python wrappers of REFPROP:

These wrappers are all community developed/supported:

* [Python wrapper of Bruce Wernick](http://trc.nist.gov/refprop/LINKING/WERNICK.ZIP)
* [Updated wrapper of Ben Thelen of Leukothea Pte. Ltd. ](http://trc.nist.gov/refprop/LINKING/THELEN.ZIP)
* [CoolProp wrapper](http://www.coolprop.org/coolprop/wrappers/Python/index.html#python)