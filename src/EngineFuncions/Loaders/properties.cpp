#include "properties.h"

//These are included here in console also contains a Properties class pointer
#include "../Tools/tools.hpp"
#include "../Tools/console.h"

//-------------------------------------------//
//        Properties Class Functions         //
//-------------------------------------------//

// Read the configuration file
void Properties::read_gcf()
{
    Console::cPrint(true,"|-----------------------------------|");
    Console::cPrint(tools::appendStrings("Reading Properties file: ",filename));

    lines = 0;
    std::string line;
    std::ifstream file (filename.c_str());
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            data.push_back(line);
            ++lines;
        }
        file.close();
    }
    else
    {
        Console::cPrint(tools::appendStrings("Error: Unable to open file: ",filename));
        Console::DumpConsole(true);
        exit(EXIT_FAILURE);
    }
    this->lines = lines;
};

// Save the configuration file
void Properties::save_gcf(std::stringstream *pdata)
{
    Console::cPrint("Saving Properties...");
    std::ofstream file;
    file.open(filename.c_str());

    //_________________________________________
    // ADD NEW PARAMETERS BELOW THIS LINE HERE
    //-----------------------------------------
    file << "fullscreen=" << pdata[0].str() << std::endl;
    file << "resauto=" << pdata[1].str() << std::endl;
    file << "reswidth=" << pdata[2].str() << std::endl;
    file << "resheight=" << pdata[3].str() << std::endl;
    file << "msaa=" << pdata[4].str() << std::endl;

    file.close();

};

// Sets the properties stored in data
void Properties::set_properties()
{
    //*********************
    // Parse the File Data
    //*********************
    for (int i = 0; i < lines; ++i)
    {
        int pos = data[i].find_first_of("=");
        std::string vname = data[i].substr(0,pos);


        //_________________________________________
        // ADD NEW PARAMETERS BELOW THIS LINE HERE
        //-----------------------------------------
        std::string cs = "reswidth";
        if (vname.compare(cs) == 0)
        {
            std::string val = data[i].substr(pos+1);
            WinWidth = atoi(val.c_str());
        }

        cs = "resauto";
        if (vname.compare(cs) == 0)
        {
            std::string val = data[i].substr(pos+1);
            ResAuto = atoi(val.c_str());
        }

        cs = "resheight";
        if (vname.compare(cs) == 0)
        {
            std::string val = data[i].substr(pos+1);
            WinHeight = atoi(val.c_str());
        }

        cs = "fullscreen";
        if (vname.compare(cs) == 0)
        {
            std::string val = data[i].substr(pos+1);
            FullScreen = atoi(val.c_str());
        }

        cs = "msaa";
        if (vname.compare(cs) == 0)
        {
            std::string val = data[i].substr(pos+1);
            MSAA = atoi(val.c_str());
        }
    }

    GetPrimaryResolution(ResAuto);
    GetPrimaryMonitorSize();

    //********************
    // Some Calculations
    //********************
    //Calculate DPmm
    DPmm = sqrt(WinWidth * WinHeight)/sqrt(MonWidth * MonHeight);
    //FontSize = 0.0002f * (float)pow(DPmm,2) + 0.15f;
    FontSize = 0.06f * (float)DPmm;
    //FontSize = 0.2f;
    //FontSize = 0.000000075f * (float)(WinWidth * WinHeight);
    ImgLength = (float)sqrt(DPmm);
    LineSpace = 0.48f * (float)DPmm + 15.0f;
    aspectratio=MonWidth/(float)MonHeight;

    //********************
    // Results -> Console
    //********************
    Console::cPrint(tools::appendStrings("Resolution: ",WinWidth,"x",WinHeight));
    Console::cPrint(tools::appendStrings("Monitor Size: ",MonWidth,"x",MonHeight));
    Console::cPrint(tools::appendStrings("MSAA Sampling: ",MSAA));
    Console::cPrint(tools::appendStrings("DPmm: ",DPmm," pixels/mm^2 "," Aspect Ratio: ",aspectratio));
    Console::cPrint(tools::appendStrings("Calculated Font Scaling: ",FontSize));

    //*************************
    //   Clear Working Memory
    //*************************
    data.clear();

    Console::cPrint("|-----------------------------------|",true);
};

// Auto obtain the primary monitor resolution
void Properties::GetPrimaryResolution(int ResAuto)
{
    if (ResAuto == 1)
    {
        GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
        if (Monitor)
        {
            const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);
            WinWidth = Mode->width;
            WinHeight = Mode->height;
        }
        else
        {
            Console::cPrint("Error Obtaining Primary Monitor.");
            Console::DumpConsole(true);
            exit(1);
        };
    }
};

// Obtain the primary monitors size
void Properties::GetPrimaryMonitorSize()
{
    GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
    if (Monitor)
    {
        glfwGetMonitorPhysicalSize(Monitor,&MonWidth,&MonHeight);
    }
    else
    {
        Console::cPrint("Error Obtaining Primary Monitor.");
        Console::DumpConsole(true);
        exit(1);
    };
};
