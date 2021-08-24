from distutils.core import setup
import setuptools

"""
To upload: 
conda activate py38 && python setup.py bdist_wheel && conda deactivate && twine upload dist/*.whl
"""

setup(name='ctREFPROP',
      version='0.10.2',
      description='A ctypes wrapper for NIST REFPROP library',
      author='Ian H. Bell',
      author_email='ian.bell@nist.gov',
      packages = ['ctREFPROP'],
      zip_safe = False
     )

