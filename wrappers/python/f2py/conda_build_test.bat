conda create -y -n f2pytest python==3.6 numpy mingw
activate f2pytest 
python build_f2py_wrapper.py 
conda env remove -y -n f2pytest