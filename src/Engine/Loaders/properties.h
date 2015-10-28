#ifndef PROPS_H
#define PROPS_H

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"

//-------------------------------------------//
//               Properties Class            //
//-------------------------------------------//
struct Properties
{
        std::string filename;

        std::vector<std::string> data;
        int lines;

        // Input parameters
        int ResAuto;
        int WinWidth;
        int WinHeight;
        int FullScreen; // Fullscreen mode toggle 1 on 0 off
        int MSAA; // MSAA level

        // Text Formatting Data
        int DPmm; // Dots per millimeter
        float FontSize; // A scalar calculated from DPmm
        float ImgLength;
        float LineSpace; // Calculated from DPmm

        // Other obtained variables
        int MonWidth; // Monitor physical width in mm
        int MonHeight; // Monitor physical height in mm
        float aspectratio;

        void init (void)
        {
                filename = "../graphicsconfig.gcf";
                read_gcf();
                set_properties();
        };

        Properties () {};
        ~Properties () {};

        void read_gcf(void);
        void save_gcf(std::stringstream *pdata);
        void set_properties(void);
        void GetPrimaryResolution(int ResAuto);
        void GetPrimaryMonitorSize(void);
};

#endif
