#ifndef CONSOLE_H
#define CONSOLE_H

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "screenwriter.h"
#include "tools.hpp"
#include "../Loaders/properties.h"
#include <iostream>

//________________________________________________________________________//
//      *************************************************************     //
//                               Timer Class
//                  Holds timer variables and class functions
//      *************************************************************     //
class Console
{
    static std::vector<std::string> cbuffer; // Console buffer

    //------------------------------
    //Private Member Class Functions
    //------------------------------
    ScreenWriter ctext;
    Properties *props;

public:
    //--------------------------
    //Public Class Declarations
    //--------------------------
    //static std::vector<std::string> cbuffer; // Console buffer
    //static std::vector<std::string> cbuffer; // Console buffer

    //-----------------------------
    //Public Member Class Functions
    //-----------------------------
    // Initialie the class
    void Init(Properties *props);

    void Draw(void);

    void Clear(void);

    static void cPrint(std::string line);
};

#endif

