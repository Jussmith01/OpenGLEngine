#include "menuicon.h"

//************************************
// Menu Icon Initilization Function
//************************************
/*
Initialize the menu button class.

PARAMETERS:

1) The first parameter is the naming scheme for the
button image in string. For instance, this parameter
would be "icons/imgfilename" if your buttons
image files were stored in bin/Data/Images/icons/
and the file names were,

inactive: imgfilename_inact.png
active: imgfilename_act.png

4) Scale of the icon. 10.0 is the image stretched over the screen.

5 and 6) the screen width and height.
*/
void MenuButtons::Init(std::string iconimage,float scale,Properties &props)
{
    // Define needed variables
    this->swidth=props.WinWidth;
    this->sheight=props.WinHeight;
    activebutton=-1;
    hoverbutton=-1;

    std::stringstream fn1,fn2; //File names for images
    fn1 << buttonimage << "_normal.png";
    fn2 << buttonimage << "_hover.png";

    // Setup button images on GPU
    float is=scale*0.1;

    //image[0].Init(fn1.str(),scale*is,scale*is,swidth,sheight);
    //image[1].Init(fn2.str(),scale*is,scale*is,swidth,sheight);
    //image[2].Init(fn3.str(),scale*is,scale*is,swidth,sheight);

    image[0].Init(fn1.str(),is,is,swidth,sheight);
    image[1].Init(fn2.str(),is,is,swidth,sheight);

    //Obtain Image Border
    lw=image[0].GetImageHalfLength();
    lh=image[0].GetImageHalfHeight();
};

//************************************
//      Set Text Colors Function
//************************************
/*
Change the color of the text under certain conditions.
The parameters are self explainatory.
*/
void MenuButtons::SetFontColors(glm::vec3 normal,glm::vec3 hover,glm::vec3 press)
{
    colors[0]=normal;
    colors[1]=hover;
    colors[2]=press;
};

//************************************
//     Draw the buttons Function
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void MenuButtons::DrawButtons()
{
    for (int i=0;i<(int)button.size();++i)
    {
        // Variables
        float x=button[i].x;
        float y=button[i].y;
        int bs=button[i].state;
        std::string cap=button[i].caption;

        // Draw
        //std::cout << "BUTTON " << i<< "x: " << x << " y: " << y << std::endl;
        image[bs].DrawImagePos(x,y);
        text.RenderTextCentered(cap,true,x,true,y,1.0,colors[bs]);
    }
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void MenuButtons::Cleanup()
{
    image[0].Cleanup();
    image[1].Cleanup();
    image[2].Cleanup();
    text.Cleanup();
    button.clear();
};

//************************************
//   Adds a new button to the Class
//************************************
/*
This function adds a new button into the class
at screen position x,y in normalized device
coordinates with a caption that reads "caption".
The active parameter set to true sets this button
as the default active button when these buttons are
printed to the screen.
*/
void MenuButtons::DefineNewButton(std::string caption,float x, float y,bool active)
{
    float xpos=x;
    float ypos=y;

    //tools::pixel_device_coordinate(xpos,ypos,swidth, sheight);

    //std::cout << "XPOS: " << xpos << " YPOS: " << xpos << std::endl;

    Button Btemp(caption,xpos,ypos);
    button.push_back(Btemp);
    if (active)
    {
        int ID=button.size()-1;
        SetActiveButton(ID);
    }
};

//************************************
//      Check Button States
//************************************
/*

*/
int MenuButtons::UpdateButtonEvents(InputStruct &input)
{
    //std::cout << "epress: " <<  epress << " erelease: " <<  erelease << " activebutton: " << activebutton << std::endl;
    double x,y;
    input.ReturnMousePos(x,y);

    bool aup=input.GetKey(GLFW_KEY_UP);
    bool adown=input.GetKey(GLFW_KEY_DOWN);

    aup=kh[0].CheckKeyState(aup,GLFW_KEY_UP);
    adown=kh[1].CheckKeyState(adown,GLFW_KEY_DOWN);

    // Find out which button the mouse is over, -1 if none
    CheckMouseOver(x,y);
    ArrowChangeActive(aup,adown);

    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);
    bool epress=input.GetKey(GLFW_KEY_ENTER);

    //Check if press and hold occures and change for a button
    CheckPress(mpress,hoverbutton);
    CheckPress(epress,activebutton);

    mpress=kh[2].CheckKeyState(mpress,GLFW_MOUSE_BUTTON_LEFT);
    epress=kh[3].CheckKeyState(epress,GLFW_KEY_ENTER);

    int ButtonID=-1;

    //std::cout << "ab: " << activebutton << " epress: " << epress << std::endl;
    //Tigestd::cout << "hb: " << hoverbutton << " mpress: " << mpress << std::endl;
    if((activebutton>=0 && epress) || (hoverbutton>=0 && mpress))
    {
        lastuse=glfwGetTime();
        ISound* snd=audioengine->play2D(soundpress.c_str(),false,false,true);
        while(!snd->isFinished()){};
        ButtonID=activebutton;
    }

    return ButtonID;
};

//************************************
//  Check if Mouse is Over a Button
//************************************
/*
This function checks if the mouse, at x,y coords,
is over the function, then sets that button
active if it is.
*/
void MenuButtons::CheckMouseOver(float x, float y)
{
    hoverbutton=-1;

    float xmp=x;
    float ymp=y;

    //tools::normal_device_coordinate(xmp,ymp,swidth, sheight);

    bool hover=false;
    for (int i=0;i<(int)button.size();++i)
    {

        hover=button[i].CheckIfOver(xmp,ymp,lw,lh);// Determine mouse over

        if (hover)
        {
            //std::cout << "HOVERING: " << i << std::endl;
            hoverbutton=i;

            if (activebutton!=i)
            {
                SetActiveButton(i);
            }
        }
    }
};

//*******************************************
//Determine if any button is in pressed state
//*******************************************
/*

*/
void MenuButtons::CheckPress(bool press,int ButtonID)
{
    if(ButtonID>=0 && press)
    {
        button[ButtonID].state=2;// Set State to button Pressed
    }
};


//*******************************************
//Determine if any button is in pressed state
//*******************************************
/*
This function checks for arrow up and arrow down calls to change
the active state of the buttons accordingly. Arrow down will
cycle from first to last defined and so on for arrow up.
*/
void MenuButtons::ArrowChangeActive(bool aup,bool adown)
{
    if (aup && activebutton>0)
    {
        alastuse=glfwGetTime();
        int ID=activebutton-1;
        SetActiveButton(ID);
    }

    if (adown && activebutton<(int)(button.size()-1))
    {
        alastuse=glfwGetTime();
        int ID=activebutton+1;
        SetActiveButton(ID);
    }
};

//*******************************************
//          Reset Button States
//*******************************************
/*
This is a tool function that resets all active states to zero.
Its typically used before setting a new active state within
this class.
*/
void MenuButtons::ResetButtonStates()
{
        for (int j=0;j<(int)button.size();++j)
        {
            button[j].state=0;// Reset States to Default
        }
};

//*******************************************
//             Set Active Button
//*******************************************
/*
This function sets the new buttonID give as the current active button.
This is used many times throughout the class to change button active
states under certain conditions.
*/
void MenuButtons::SetActiveButton(int ButtonID)
{
    //std::cout << soundactive.c_str() << std::endl;
    audioengine->play2D(soundactive.c_str());
    ResetButtonStates();
    activebutton=ButtonID;
    button[ButtonID].state=1;
};
