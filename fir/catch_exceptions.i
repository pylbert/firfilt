%{#include <stdexcept>%}
%include "exception.i"

%exception
{
try
{
    $action
}
catch (std::invalid_argument& e)
{
    std::string s1("Invalid Argument: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_ValueError, s1.c_str());
}
catch (std::domain_error& e)
{
    std::string s1("Domain Error: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_ValueError, s1.c_str() );
}
catch (std::overflow_error& e)
{
    std::string s1("Overflow Error: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_OverflowError, s1.c_str() );
}
catch (std::out_of_range& e)
{
    std::string s1("Out of Range: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_IndexError, s1.c_str() );
}
catch (std::length_error& e)
{
    std::string s1("Length Error: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_IndexError, s1.c_str() );
}
catch (std::logic_error& e)
{
    std::string s1("Logic Error: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_RuntimeError, s1.c_str() );
}
catch (std::bad_alloc& e)
{
    SWIG_exception(SWIG_MemoryError, e.what() );
}
catch (std::runtime_error& e)
{
    std::string s1("Runtime Error: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_RuntimeError, s1.c_str());
}
catch (std::exception& e)
{
    /* catch other std::exceptions here */
    std::string s1("System Error: "), s2(e.what());
    s1 = s1 + s2;
    SWIG_exception(SWIG_SystemError, s1.c_str() );
}
catch (...)
{
    /* catch everything else */
    SWIG_exception(SWIG_UnknownError, "Unknown error" );
}
}
