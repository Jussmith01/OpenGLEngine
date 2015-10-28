#ifndef CONSOLE_H
#define CONSOLE_H

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "screenwriter.h"
#include "tools.hpp"
#include "../Loaders/properties.h"

#include <iostream>

//_____________________________________________________________//
//      **************************************************     //
//                        Console Class
//       Holds the console functionality within one class
//       The class uses statically defined functions and
//       variables to track the console output
//      **************************************************     //
class Console
{
    //------------------
    // Static Variables
    //------------------
    static std::vector<std::string> cbuffer; // Console buffer
    static bool consoleActive;

    //------------------
    //  Class Variables
    //------------------
    double memsize;

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

    // Cleanup
    void Clear(void);

    // Get Class Memory Usage
    void SetMemoryUsage(void);

    //------------------------------
    //Static Public Member Functions
    //------------------------------

    // Console dump
    static void DumpConsole(bool errchk);

    // Update the class
    static void Update(bool F12);

    // Console printing functions
    static void cPrint(std::string line);
    static void cPrint(bool spacef,std::string line);
    static void cPrint(std::string line,bool spaceb);
    static void cPrint(bool spacef,std::string line,bool spaceb);

};

#endif

