#ifndef JUMENUERRS_H
#define JUMENUERRS_H

#include <iostream>

//________________________________________________________________________//
//      *************************************************************     //
//                      Compiler Error Handling
//      *************************************************************     //
/* Check for sufficient compiler version */
#if defined(__GNUC__) || defined(__GNUG__)
    #if !(__GNUC__ >= 4 && __GNUC_MINOR__ >= 9)
        #error "Insufficient GNU Compiler Version -- 4.9 or greater required"
    #endif
#else
    #warning "Currently only GNU compilers are supported and tested, but go ahead if you know what you're doing."
#endif

/*----------------------------------------------
               Throw Exception
    Pass the macro a char string with an error
    and throw a std::string exception.
-----------------------------------------------*/
#define JmenuThrow(_errstr)                                \
{                                                          \
    if (!std::string(_errstr).empty())                     \
    {                                                      \
        std::stringstream _loc;                            \
        _loc << "In file: " << __FILE__ << ":" << __LINE__;\
        throw _loc.str()                                   \
            + std::string("\n")                            \
            + std::string("In Function: ")                 \
            + std::string(__FUNCTION__)                    \
            + std::string("() -- ")                        \
            + std::string(_errstr)                         \
            + std::string("\n");                           \
    }                                                      \
};

#endif
