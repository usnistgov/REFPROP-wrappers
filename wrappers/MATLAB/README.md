
# REFPROP for MATLAB

Note: As of REFPPROP 10, the only interface between REFPROP and MATLAB that is officially supported is via Python.  The previous interface, which still works in version 10, can be found in the legacy folder relative to this file.  No official support is provided for the legacy interface due to the challenges of maintaining the interface with various versions of MATLAB.  If you still want to use the old wrapper and you run into troubles, please open an issue here and hopefully another MATLAB user can help you: [New Issue](https://github.com/usnistgov/REFPROP-wrappers/issues/new).  Pull requests updating and fixing the legacy interface will be considered and are welcomed.

The Python wrapper is written and maintained by Ian Bell.  For questions, bugs, or issues, please open an issue in this repository on github: https://github.com/usnistgov/REFPROP-wrappers/issues/new

## Setup

You will need to acquire a Python interpreter, the easiest method to do so if you do not already have Python installed on your computer is to download the installer from python.org: [link to downloads](https://www.python.org/downloads/).  There is also information on [the Mathworks website](https://www.mathworks.com/help/matlab/matlab_external/install-supported-python-implementation.html).  The Python wrapper of REFPROP only uses methods that are in the Python standard library, so the standard installation of Python would work fine.  If you would like to have a more full-featured Python installation that will work with the MATLAB interface of REFPROP, you can install a full-fledged Python installation from Anaconda: [Anaconda download](https://www.anaconda.com/download/).

In your MATLAB shell, you can inquire about what Python version MATLAB intends to use with a command like:

```
>> pyversion

       version: '2.7'
    executable: 'D:\Anaconda\python.exe'
       library: 'D:\Anaconda\python27.dll'
          home: 'D:\Anaconda'
      isloaded: 0
```
Good.  It found Python, and has not loaded it yet.  You are ready!

If you obtained Python from an Anaconda installation then you can open an Anaconda prompt (In Start Menu->Anaconda Prompt (64-bit) or somewhere similar depending on your sytem) and query the path of your Python installation with a command like:
```
(base) C:\Users\ihb>where python
C:\Users\ihb\Miniconda3\python.exe
```
and then set the path to your Python in MATLAB:
```
>> pyversion C:\Users\ihb\Miniconda3\python.exe
```

If you have multiple copies of Python on your computer already (in conda environments, for instance), then you can tell MATLAB which one you want it to use by passing the absolute path to the python executable to ``pyversion``.  For instance:

```

>> pyversion d:\Anaconda\envs\py36\python.exe
>> pyversion

       version: '3.6'
    executable: 'd:\Anaconda\envs\py36\python.exe'
       library: 'd:\Anaconda\envs\py36\python36.dll'
          home: 'd:\Anaconda\envs\py36'
      isloaded: 0
```

## Install ctREFPROP

Finally, you need to install the ctREFPROP package (a ``ctypes``-based interface to REFPROP) into your given copy of python.  This one-liner calls the ``pip`` program of Python to install the ctREFPROP package from the PYPI package index.  Watch out for the spaces in the arguments, they are important!:
``` MATLAB
[v,e] = pyversion; system(['"', e, '" -m pip install --user -U ctREFPROP'])
```

If your python installation does not have ``pip`` installed (it is usually installed by default), you can install it with
``` MATLAB
[v,e] = pyversion; system(['"', e, '" -m easy_install pip'])
```

If you run into SSL problems, you may want to install the ctREFPROP wrapper directly from an Anaconda prompt. To do so, open an Anaconda prompt (Start->Anaconda prompt on windows).  Then run:
``` 
pip install -U ctREFPROP
```
If that doesn't work, please open an issue on github.  If installing directly from pip still doesn't work within an Anaconda prompt, then you may want to try to install the wheel from PYPI.  First go to https://pypi.org/project/ctREFPROP/#files and download the appropriate wheel for your version of Python (3 or 2), then in an anaconda prompt, do
```
pip install -U c:/path/to/ctREFPROP-0.7.3-py3-none-any.whl
```
where the path to the .whl file that you downloaded to your computer should be updated as needed.

## Use

To initialize REFPROP, you tell it what the root path of your REFPROP installation is:
```
RP = py.ctREFPROP.ctREFPROP.REFPROPFunctionLibrary('C:\Program Files (x86)\REFPROP')
```
and to confirm that everything is working correctly, let's print out the version of REFPROP that you have loaded:
```
>> RP.RPVersion()

ans = 

  Python str with no properties.

    10.0068
```
As the first practical example of the use of the MATLAB interface, let's calculate the normal boiling point temperature of water at one atmosphere (1 atm = 101325 Pa). 

```
>> MOLSI = RP.GETENUMdll(int8(0),'MOLAR BASE SI').iEnum;
>> iMass = int8(0); % 0: molar fractions; 1: mass fractions
>> iFlag = int8(0); % 0: don't call SATSPLN; 1: call SATSPLN
>> z = {1.0};
>> r = RP.REFPROPdll('Water','PQ','T',MOLSI,iMass,iFlag,101325,0.0,z)

ans = 

  Python REFPROPdlloutput with properties:

    Output
    hUnits
    herr
    iUCode
    ierr
    q
    x
    x3
    y
    z
```
Multiple outputs are returned in a data structure ([see the documentation for the REFPROPdll function](http://refprop-docs.readthedocs.io/en/latest/DLL/high_level.html#f/_/REFPROPdll)).  Then you can obtain the first output by indexing the ``Output`` list.  This interface calls directly into the DLL (via Python), so the documentation is exactly what you need to know for MATLAB too.

As of ctREFPROP 0.7, the return type is an [array.array](https://docs.python.org/3/library/array.html) in Python in order to make the Python interface more computationally efficient (and thus the MATLAB one too).  In order to make use of this output in MATLAB, you can either 

A) Convert the array.array to a list with ``py.list(r.Output)`` (less efficient option)
B) Convert the array.array to a MATLAB array with ``double(r.Output)`` (more efficient option) [MATLAB docs](https://www.mathworks.com/help/matlab/matlab_external/handling-data-returned-from-python.html)

The normal boiling point temperature is then obtained from the Output array:
```
>> o = double(r.Output);
>> o(1)

ans =

  373.1243
```

Success!

## Additional reading

Mini-tutorial available in Python as a jupyter notebook: [Link to tutorial](https://nbviewer.jupyter.org/github/usnistgov/REFPROP-wrappers/blob/master/wrappers/python/notebooks/Tutorial.ipynb) .  The notebook can be accessed at [wrappers/python/notebooks](https://github.com/usnistgov/REFPROP-wrappers/tree/master/wrappers/python/notebooks)
