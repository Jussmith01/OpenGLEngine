#include "menuicon.h"

//************************************
// Menu Icon Initilization Function
//************************************
/*
Initialize the menu icon class.

PARAMETERS:

1) The first parameter is the naming scheme for the
icon image in string. For instance, this parameter
would be "icons/imgfilename" if your icons
image files were stored in bin/Data/Images/icons/
and the file names were,

inactive: imgfilename_inact.png
active: imgfilename_act.png

4) Scale of the icon. 10.0 is the image stretched over the screen.

5 and 6) the screen width and height.
*/
void MenuIcons::Init(std::string iconimage,float scale,Properties &props)
{
    // Define needed variables
    this->swidth=props.WinWidth;
    this->sheight=props.WinHeight;
    activeicon=-1;
    hovericon=-1;

    std::stringstream fn1,fn2; //File names for images
    fn1 << iconimage << "_inact.png";
    fn2 << iconimage << "_act.png";

    // Setup icon images on GPU
    float is=scale*0.1;
    image[0].Init(fn1.str(),is,is,swidth,sheight);
    image[1].Init(fn2.str(),is,is,swidth,sheight);

    //Obtain Image Border
    lw=image[0].GetImageHalfLength();
    lh=image[0].GetImageHalfHeight();
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
        int is=icon[i].state;

        // Draw
        //std::cout << "icon " << i<< "x: " << x << " y: " << y << std::endl;
        image[is].DrawImagePos(x,y);
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
    image[0].Cleanup();
    image[1].Cleanup();
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
void MenuIcons::DefineNewIcon(float x, float y)
{
    float xpos=x;
    float ypos=y;

    //tools::pixel_device_coordinate(xpos,ypos,swidth, sheight);

    //std::cout << "XPOS: " << xpos << " YPOS: " << xpos << std::endl;

    Icon Itemp(xpos,ypos);
    icon.push_back(Itemp);
};

//************************************
//      Check icon States
//************************************
/*

*/
int MenuIcons::UpdateIconEvents(InputStruct &input)
{
    //std::cout << "epress: " <<  epress << " erelease: " <<  erelease << " activeicon: " << activeicon << std::endl;
    double x,y;
    input.ReturnMousePos(x,y);

    // Find out which icon the mouse is over, -1 if none
    CheckMouseOver(x,y);

    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);

    //Check if press and hold occures and change for a icon
    CheckPress(mpress,hovericon);

    mpress=kh.CheckKeyState(mpress,GLFW_MOUSE_BUTTON_LEFT);

    int IconID=-1;

    //std::cout << "ab: " << activebutton << " epress: " << epress << std::endl;
    //Tigestd::cout << "hb: " << hoverbutton << " mpress: " << mpress << std::endl;
    if(hovericon>=0 && mpress)
    {
        IconID=activeicon;
    }

    return IconID;
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

        hover=icon[i].CheckIfOver(xmp,ymp,lw,lh);// Determine mouse over

        if (hover)
        {
            //std::cout << "HOVERING: " << i << std::endl;
            hovericon=i;

            if (activeicon!=i)
            {
                SetActiveIcon(i);
            }
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
    if(IconID>=0 && press)
    {
        icon[IconID].state=2;// Set State to icon Pressed
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
            icon[j].state=0;// Reset States to Default
        }
};

//*******************************************
//             Set Active Icon
//*******************************************
/*
This function sets the new iconID give as the current active icon.
This is used many times throughout the class to change icon active
states under certain conditions.
*/
void MenuIcons::SetActiveIcon(int IconID)
{
    ResetIconStates();
    activeicon=IconID;
    icon[IconID].state=1;
};
