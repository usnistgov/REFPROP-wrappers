from distutils.core import setup
import setuptools

"""
To upload: 
conda activate py27 && python setup.py bdist_wheel && conda activate py37 && python setup.py bdist_wheel && conda deactivate && twine upload dist/*.whl
"""

setup(name='ctREFPROP',
      version='0.8.1',
      description='A ctypes wrapper for NIST REFPROP library',
      author='Ian H. Bell',
      author_email='ian.bell@nist.gov',
      packages = ['ctREFPROP'],
      zip_safe = False
     )

