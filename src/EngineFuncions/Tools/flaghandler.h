#ifndef flaghandler_header
#define flaghandler_header

#include "../../Headers/headerscpp.h"

//      *************************************************************     //
//                            Flag Handler Class
//                  	    Handles all input flags
//      *************************************************************     //
/*

*/
class FlagHandler
{
    //--------------------------
    //   Vector to hold flags
    //--------------------------
    std::vector<std::string> flags;

    //------------------------------
    //Private Member Class Functions
    //------------------------------
    void FlagParser()
    {
        //Set Defaults
        nosplash=false;

        for (int i=1; i<(int)flags.size(); ++i)
        {
            // -ns flag causes the launch to skip the splash screen
            if (flags[i].compare("-ns")==0) {nosplash=true;}
        }
    };

public:
    //--------------------------
    //     Public Variable
    //--------------------------
    bool nosplash;

    //-----------------------------
    //	Class Constructor
    //-----------------------------
    FlagHandler () {};

    void Init(int argc, char *argv[])
    {
        for (int i=0; i<argc; ++i)
        {
            flags.push_back(argv[i]);
        }

        FlagParser();
    };
};
#endif

