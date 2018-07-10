%module fir

/* Handle all known stl types for target native language */
%include <stl.i>

/* Handle std::vector<double> */
%template(doublevec) std::vector<double>;

/* Catch a set of execptions */
%include catch_exceptions.i

/* Generate basic parameter type documentation */
%feature("autodoc", "3");

/* The #include directive copies the contents into the wrapper src */
%{#include "fir.hpp"%}
/* The %include directive lets SWIG parse the header and expose its methods */
%include "fir.hpp"

/* Attach pythons __str__ method to a similar method in C++ */
%feature("python:slot", "tp_str", functype="reprfunc") Filter::__str__;

/* Load java library in JNI code */
%pragma(java) jniclasscode=%{
static
{
    try
    {
        System.loadLibrary("firjava");
    }
    catch (UnsatisfiedLinkError e)
    {
        System.err.println("Native code library (firjava) failed to load. \n" + e);
        System.exit(1);
    }
}
%}
