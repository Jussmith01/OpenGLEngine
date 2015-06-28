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
    //******************
    // Static Variables
    //******************
    static std::vector<std::string> cbuffer; // Console buffer
    //static bool consoleActive;

    //------------------------------
    //Private Member Class Functions
    //------------------------------
    ScreenWriter ctext;
    Properties *props;

public:
    //--------------------------
    //Public Class Declarations
    //--------------------------

    //-----------------------------
    //Public Member Class Functions
    //-----------------------------
    // Initialie the class
    void Init(Properties *props);

    // Draw to the screen
    void Draw(void);

    // Update the class with state inputs
    //static void Update(bool F12);

    // Console printing functions
    static void cPrint(std::string line);

    // Cleanup
    void Clear(void);
};

#endif

