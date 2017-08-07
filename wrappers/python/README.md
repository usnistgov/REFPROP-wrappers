# Python wrappers

Due to the fact that python is the primary working environment for one of the main REFPROP developers (Ian Bell), the support of REFPROP in python is quite robust.  There are a number of options, depending on what kind of interface you are looking for:

* Thin wrapper around the core functions exported in PASS_FTN.FOR:
    * ctypes wrapper
    * f2py wrapper
* Super powered wrapper around REFPROP (for REFPROP 10+) allowing for multiple copies of REFPROP in memory, etc.:
    * TO BE COMPLETED

## ctypes wrapper

There is a [ctypes](https://docs.python.org/3/library/ctypes.html) wrapper of REFPROP available, this wrapper is automatically generated for all functions exported from the DLL.  It sits in the ``ctypes`` folder relative to this folder.

Some important notes:

* The wrapper is single-threaded. It holds a pointer to an instance of REFPROP, and on windows, if multiple loads of the library are done, each reference points to the same instance.  As a result, you need to use separate *processes* to use multiple copies of REFPROP in parallel.  A stripped down example of the use of multiple processes (and therefore multiple copies of REFPROP) is provided in the file ``multiprocessing_example.py`` in the ``examples`` folder.
* You can regenerate the interface with the ``build_wrapper.py`` script, for instance to build the wrapper for an older version of REFPROP.  Requirements: ``numpy`` (for ``f2py``)
* While you can just copy-paste the ``ctREFPROP.py`` script to your desired location, or anywhere that is on ``sys.path``, you can also run the ``setup.py`` script in the ``ctypes`` folder to install the interface to the default package location on your computer via ``python setup.py install`` in the ``ctypes`` folder.

## f2py wrapper

There is also a ``f2py`` interface for REFPROP that you can compile yourself; this ``f2py`` wrapper is a native python extension module that provides a very thin wrapper around the REFPROP code.  It exports the same functions as the ``ctypes`` wrapper described above, with some important differences:

* The ``f2py`` wrapper needs to be compiled for your architecture.  No precompiled binaries are available, but in anaconda/miniconda, you can build the wrapper with the ``build_f2py_wrapper.py`` script, which should compile a shared library of the wrapper on your computer.  You will need to install the package mingw on windows to get access to the gfortran compiler.
* On windows, all function are exported with lower case, so ``SETUPdll`` becomes ``setupdll``, and so on. 

## Other python wrappers of REFPROP:

These wrappers are all community developed/supported:

* [Python wrapper of Bruce Wernick](http://trc.nist.gov/refprop/LINKING/WERNICK.ZIP)
* [Updated wrapper of Ben Thelen of Leukothea Pte. Ltd. ](http://trc.nist.gov/refprop/LINKING/THELEN.ZIP)
* [CoolProp wrapper](http://www.coolprop.org/coolprop/wrappers/Python/index.html#python)