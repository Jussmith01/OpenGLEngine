#include "checkbox.h"

//************************************
// Check Box Initilization Function
//************************************
/*
Initialize the menu check box class.

PARAMETERS:

*/
void JuMenu::MenuCheckBox::Init(std::string boximage,std::string boxsound,
                        float scale,Properties &props,ISoundEngine* audioengine)
{
    // Define needed variables
    this->swidth=props.WinWidth;
    this->sheight=props.WinHeight;
    checked=false;

    // Setup Image Filenames
    std::stringstream fn1,fn2; //File names for images
    fn1 << boximage << "_unchk.png";
    fn2 << boximage << "_chk.png";

    // Setup box images on GPU
    float is=scale;

    image[0].Init(fn1.str(),is,is,swidth,sheight);
    image[1].Init(fn2.str(),is,is,swidth,sheight);

    //Obtain Image Border
    lw=image[0].GetImageHalfLength();
    lh=image[0].GetImageHalfHeight();

    //Setup Audio Engine
    this->audioengine = audioengine;

    // Setup audio file
    std::stringstream s1;
    s1 << "../Data/Sounds/" << boxsound << "_press.wav";
    sound = s1.str();

    dt=0.0;
};

//************************************
//          Set the Checkbox
//************************************
/*

*/
void JuMenu::MenuCheckBox::Set(bool State)
{
    checked=State;
};

//************************************
//    Set the Checkbox to Checked
//************************************
/*

*/
void JuMenu::MenuCheckBox::SetChecked()
{
    checked=true;
};

//************************************
//    Set the Checkbox to Unchecked
//************************************
/*

*/
void JuMenu::MenuCheckBox::SetUnchecked()
{
    checked=false;
};

//************************************
//         Draw the Check Box
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void JuMenu::MenuCheckBox::DrawCheckBox(double x,double y)
{
    // Draw
    //std::cout << "BUTTON " << i<< "x: " << x << " y: " << y << std::endl;
    this->x=x;
    this->y=y;
    //std::cout << "checked: " << checked << std::endl;
    image[(int)checked].DrawImagePos(x,y);
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void JuMenu::MenuCheckBox::Cleanup()
{
    image[0].Cleanup();
    image[1].Cleanup();
};


//************************************
//      Check Button States
//************************************
/*

*/
void JuMenu::MenuCheckBox::UpdateCheckBoxEvents(InputStruct &input)
{
    double xmp,ymp;
    input.ReturnMousePos(xmp,ymp);
    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);

    // Find out which button the mouse is over, -1 if none
    bool hover=CheckMouseOver(xmp,ymp);

    //std::cout << "hover: " << hover << " mpress: " << mpress << std::endl;
    if(hover && mpress && (glfwGetTime()-dt)>0.5)
    {
        dt=glfwGetTime();
        ISound* snd=audioengine->play2D(sound.c_str(),false,false,true);
        while(!snd->isFinished()) {};

        if (!checked)
        {
            checked=true;
        }
        else
        {
            checked=false;
        }
    }
};

//************************************
//  Check if Mouse is Over the Box
//************************************
/*
This function checks if the mouse, at x,y coords,
is over the function, then sets that button
active if it is.
*/
bool JuMenu::MenuCheckBox::CheckMouseOver(float xmp,float ymp)
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

//************************************
//     Returns the Checked State
//************************************
/*

*/
bool JuMenu::MenuCheckBox::CheckedState()
{
    // Return Result
    return checked;
};
