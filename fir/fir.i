%module fir

%include <std_string.i>

%{#include "fir.hpp"%}
%include "fir.hpp"

/* BEGIN Python syntax  ------------------------------------------------------- */
#ifdef SWIGPYTHON
/* Attach pythons __str__ method to a similar method in C++ */
%feature("python:slot", "tp_str", functype="reprfunc") Filter::__str__;
#endif
/* END Python syntax */
