#ifndef MENUICON_H
#define MENUICON_H

#include "../JuMenu.h"

namespace JuMenu {

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
    std::string iconimage;
    float scale;
    float lw,lh;

    // The class assumes these images are the same size
    ImageDisplay image[2];

    // Constuctors
    Icon () {}; // Default

    Icon (float x,float y,std::string iconimage,float scale,float swidth,float sheight) // Overloaded constructor
    {
        this->x=x;
        this->y=y;
        this->state=false;

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

    void Cleanup()
    {
        image[0].Cleanup();
        image[1].Cleanup();
    };

    //***************************************
    // Check if the mouse is over the Icon
    //***************************************
    /*
    Function returns true if the mouse
    is over the icon given the xmp,ymp
    mouse coordinates and lw,lh button
    width and height.
    */
    bool CheckIfOver(float xmp,float ymp)
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
    int activeicon;
    int hovericon;

    keyhandler kh;

    // Check if mouse is over button
    void CheckMouseOver(float x,float y);

    //Check if button is pressed -- set activeicon
    void CheckPress(bool press,int IconID);

public:
    // Initialize Button Class
    void Init(Properties *props);

    // Cleanup function for destruction
    void Cleanup();

    // Adds a new button to the class
    void DefineNewIcon(float x, float y,std::string iconimage,float scale);

    // Returns ButtonID if button was Pressed and Released while over a Button
    void UpdateIconEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawIcons(void);

    // Function resets all buttons to default state
    void ResetIconStates();

    // Function gets the active icon ID.
    int GetActiveIcon();
};

};


#endif
