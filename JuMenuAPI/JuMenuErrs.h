#ifndef JUMENUERRS_H
#define JUMENUERRS_H

#include <iostream>

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
