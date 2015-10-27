#define GLEW_STATIC
#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "../JuMenu.h"

namespace JuMenu {

using namespace irrklang;
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

//*****************************************//
//	            Button Struct   	       //
//*****************************************//
/*

*/
struct Button
{
    int state; // Button State
    float x,y; // Button Position
    std::string caption; // Button Text

    // Constuctors
    Button () {}; // Default

    Button (std::string caption,float x, float y) // Overloaded constructor
    {
        this->x=x;
        this->y=y;
        this->caption=caption;
        this->state=0;
    };

    //***********************
    // Initialize the Struct
    //***********************
    /*
    Function returns true if the mouse
    is over the button given the xmp,ymp
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
//	           Menu Button Class	       //
//*****************************************//
class MenuButtons
{
    // Variables
    glm::vec3 colors[3];
    std::string soundactive;
    std::string soundpress;
    std::vector<Button> button;
    float swidth,sheight;
    float lw,lh;
    int activebutton;
    int hoverbutton;

    keyhandler kh[4];

    float lastuse;
    float alastuse;

    // Tools
    ScreenWriter text;
    ImageDisplay image[3];
    ISoundEngine* audioengine;

    // Check if mouse is over button
    void CheckMouseOver(float x,float y);

    //Check if button is pressed
    void CheckPress(bool press,int ButtonID);

public:
    // Initialize Button Class
    void Init(std::string buttonimage,std::string buttonsound,std::string font,
              float scale,Properties &props,ISoundEngine* audioengine);

    // Sets the font colors for 1) normal 2) hovering over and 3) pressing
    void SetFontColors(glm::vec3 normal,glm::vec3 hover,glm::vec3 press);

    // Cleanup function for destruction
    void Cleanup();

    // Adds a new button to the class
    void DefineNewButton(std::string caption,float x, float y,bool active);

    // Returns ButtonID if button was Pressed and Released while over a Button
    int UpdateButtonEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawButtons(void);

    // input arrow up or arrow down true to change the active button
    void ArrowChangeActive(bool aup,bool adown);

    // Function resets all buttons to default state
    void ResetButtonStates();

    // Function sets the active button given button ID.
    void SetActiveButton(int ButtonID);
};

};

#endif
