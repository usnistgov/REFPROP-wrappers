from distutils.core import setup
import setuptools

setup(name='ctREFPROP',
      version='0.1',
      description='A ctypes wrapper for NIST REFPROP library',
      author='Ian H. Bell',
      author_email='ian.bell@nist.gov',
      packages = ['ctREFPROP'],
      zip_safe = False
     )