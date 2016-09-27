%module REFPROP

%include "std_string.i" // This %include allows the use of std::string natively
%include "std_vector.i" // This allows for the use of STL vectors natively(ish)

%include "typemaps.i"

// Instantiate needed templates
namespace std {
   %template(DoubleVector) vector<double>;
}

/*
If exception handling is desired, this block can be uncommented
%include "exception.i" //
%exception { 
    try {
        $action
    }  catch (std::exception &e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    } catch (...) {
        SWIG_exception(SWIG_RuntimeError, "unknown exception");
    }
}
*/

// This stuff will get included verbatim in wrapper .cxx file
%{
#include "REFPROP.h"
%}

// This will be parsed by SWIG
%include "REFPROP.h"