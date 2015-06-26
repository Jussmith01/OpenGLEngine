#include "menuicon.h"

//************************************
// Menu Icon Initilization Function
//************************************
/*
Initialize the menu icon class.

PARAMETERS:
Properties Class

*/
void MenuIcons::Init(Properties *props)
{
    // Define needed variables
    this->swidth=props->WinWidth;
    this->sheight=props->WinHeight;
    hovericon=-1;
};


//************************************
//     Draw the icons Function
//************************************
/*
This function carries out all draw calls to draw
the icons to the screen.
*/
void MenuIcons::DrawIcons()
{
    for (int i=0;i<(int)icon.size();++i)
    {
        // Variables
        float x=icon[i].x;
        float y=icon[i].y;
        int is=(int)icon[i].state;

        // Draw
        //std::cout << "icon " << i << "x: " << x << " y: " << y << std::endl;
        icon[i].image[is].DrawImagePos(x,y);
    }
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the icons
*/
void MenuIcons::Cleanup()
{
    for (int i=0;i<(int)icon.size();++i)
    {
        icon[i].Cleanup();
    }

    icon.clear();
};

//************************************
//   Adds a new icon to the Class
//************************************
/*
This function adds a new icon into the class
at screen position x,y in normalized device
coordinates with a caption that reads "caption".
The active parameter set to true sets this icon
as the default active icon when these icons are
printed to the screen.
*/
void MenuIcons::DefineNewIcon(float x,float y,std::string iconimage,float scale)
{
    float xpos=x;
    float ypos=y;

    Icon Itemp(xpos,ypos,iconimage,scale,swidth,sheight);
    icon.push_back(Itemp);
};

//************************************
//      Check icon States
//************************************
/*

*/
void MenuIcons::UpdateIconEvents(InputStruct &input)
{
    //std::cout << "epress: " <<  epress << " erelease: " <<  erelease << " activeicon: " << activeicon << std::endl;
    double x,y;
    input.ReturnMousePos(x,y);

    // Find out which icon the mouse is over, -1 if none
    CheckMouseOver(x,y);

    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);

    mpress=kh.CheckKeyState(mpress,GLFW_MOUSE_BUTTON_LEFT);

    //Check if press and hold occures and change for a icon
    CheckPress(mpress,hovericon);

    //std::cout << "ActiveIcon: " << activeicon << std::endl;
};

//************************************
//  Check if Mouse is Over a Button
//************************************
/*
This function checks if the mouse, at x,y coords,
is over the function, then sets that button
active if it is.
*/
void MenuIcons::CheckMouseOver(float x, float y)
{
    hovericon=-1;

    float xmp=x;
    float ymp=y;

    //tools::normal_device_coordinate(xmp,ymp,swidth, sheight);

    bool hover=false;
    for (int i=0;i<(int)icon.size();++i)
    {
        hover=icon[i].CheckIfOver(xmp,ymp);// Determine mouse over

        if (hover)
        {
            //std::cout << "HOVERING: " << i << std::endl;
            hovericon=i;
        }
    }
};

//*******************************************
//Determine if any icon is in pressed state
//*******************************************
/*

*/
void MenuIcons::CheckPress(bool press,int IconID)
{
    //int RTNID=-1;
    if(IconID>=0 && press)
    {
        if (activeicon==IconID)
        {
            icon[IconID].state=false;
        } else {
            ResetIconStates();
            icon[IconID].state=true;
        }


        if (icon[IconID].state)
        {
            activeicon=IconID;
            //RTNID=IconID;
        } else {
            activeicon=-1;
        }
    }
};

//*******************************************
//          Reset icon States
//*******************************************
/*
This is a tool function that resets all active states to zero.
Its typically used before setting a new active state within
this class.
*/
void MenuIcons::ResetIconStates()
{
        for (int j=0;j<(int)icon.size();++j)
        {
            icon[j].state=false;// Reset States to Default
        }
};

//*******************************************
//             Get Active Icon
//*******************************************
/*
This function retrieves the active iconID
*/
int MenuIcons::GetActiveIcon()
{
    return activeicon;
};
