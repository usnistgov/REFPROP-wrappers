# An experiment of calling REFPROP from FORTRAN on windows 

This doesn't work because the symbols exported by Intel Fortran for REFPROP from the DLL (SETUPdll for instance), don't match the symbols that are desired when compiling a .FOR file (SETUPDLL).  In other languages (e.g., Python, C++, basically anything else), this works just fine. 

On windows:

```
mkdir build 
cd build
cmake .. -DREFPROP_FORTRAN_PATH=A:/PATH/TO/FORTRAN -G "Visual Studio 11 2012 Win64"
cmake --build . --config Release
```