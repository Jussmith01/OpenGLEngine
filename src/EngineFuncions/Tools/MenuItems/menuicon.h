#ifndef MENUICON_H
#define MENUICON_H

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"

#include "../displayimage.h"
#include "../screenwriter.h"
#include "../tools.hpp"
#include "../input_struct.h"
#include "../../Loaders/properties.h"

//*****************************************//
//	              Icon Struct   	       //
//*****************************************//
/*
Icons struct is used by the MenuIcons class
to hold data and functions pertaining to a
single icon. While the class itself holds
data and functions pertaining to the full
list of icons which are defined within the
class.
*/
struct Icon
{
    bool state; // Icon State, true=active false=inactive
    float x,y; // Icon Position

    // Constuctors
    Icon () {}; // Default

    Icon (float x, float y) // Overloaded constructor
    {
        this->x=x;
        this->y=y;
        this->state=false;
    };

    //***************************************
    // Check if the mouse is over the Struct
    //***************************************
    /*
    Function returns true if the mouse
    is over the icon given the xmp,ymp
    mouse coordinates and lw,lh button
    width and height.
    */
    bool CheckIfOver(float xmp,float ymp,float lw,float lh)
    {
        // Check X over
        bool xchk=false;
        float xl=x-lw;
        float xr=x+lw;
        if (xmp>=xl && xmp<=xr)
        {
            xchk=true;
        }

        // Check y over
        bool ychk=false;
        float yb=y-lh;
        float yt=y+lh;
        if (ymp>=yb && ymp<=yt)
        {
            ychk=true;
        }

        // Check if both are true
        bool Over=false;
        if (xchk && ychk)
        {
            Over=true;
        }

        //std::cout << "xleft: " << xl << " xright: " << xr << " xmp: " << xmp << " yb: " << yb << " yt: " << yt << " ymp: " << ymp << std::endl;

        // Return Result
        return Over;
    };
};

//*****************************************//
//	           Menu Icons Class  	       //
//*****************************************//
class MenuIcons
{
    // Variables
    std::vector<Icon> icon;
    float swidth,sheight;
    float lw,lh;
    int activeicon;
    int hovericon;

    keyhandler kh;

    float lastuse;
    float alastuse;

    // Tools
    ImageDisplay image[2];

    // Check if mouse is over button
    void CheckMouseOver(float x,float y);

    //Check if button is pressed
    void CheckPress(bool press,int IconID);

public:
    // Initialize Button Class
    void Init(std::string iconimage,float scale,Properties &props);

    // Cleanup function for destruction
    void Cleanup();

    // Adds a new button to the class
    void DefineNewIcon(float x, float y);

    // Returns ButtonID if button was Pressed and Released while over a Button
    int UpdateIconEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawIcons(void);

    // Function resets all buttons to default state
    void ResetIconStates();

    // Function sets the active button given button ID.
    void SetActiveIcon(int IconID);
};


#endif
