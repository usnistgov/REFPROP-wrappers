from distutils.core import setup
import setuptools

"""
To upload: 
activate py27 && python setup.py bdist_wheel && activate py37 && python setup.py bdist_wheel && deactivate && twine upload dist/*.whl
"""

setup(name='ctREFPROP',
      version='0.8',
      description='A ctypes wrapper for NIST REFPROP library',
      author='Ian H. Bell',
      author_email='ian.bell@nist.gov',
      packages = ['ctREFPROP'],
      zip_safe = False
     )

