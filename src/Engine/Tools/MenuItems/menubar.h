#ifndef MENUBAR_H
#define MENUBAR_H

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include "../../Loaders/properties.h"
#include "../input_struct.h"
#include "dropdownmenu.h"

//*****************************************//
// For returning selection state of the Bar//
//*****************************************//
struct BarSelection
{
    bool isset;
    int menu;
    int option;

    BarSelection()
    {
        reset();
    };

    void reset()
    {
        isset=false;
    };
};

//*****************************************//
// Used to define a Single Menu on the Bar //
//*****************************************//
struct SingleBarMenuItem
{
    std::string title;
    std::vector<std::string> options;
};

//*****************************************//
//	           Menu Bar Class	           //
//*****************************************//
class MenuBar
{
    // Variables
    int NM;//Number of Menus
    std::vector<int> NO; // Number of options per menu
    std::vector<DropDownMenu> ddmenu;

public:
    // Initialize Box Class
    void Init(std::vector<SingleBarMenuItem> &baritems,Properties &props);

    // Update class events
    BarSelection UpdateBarEvents(InputStruct &input);

    // Cleanup function for destruction
    void Cleanup(void);

    // Draw the box and border to the screen center
    void DrawBar(void);
};

#endif
