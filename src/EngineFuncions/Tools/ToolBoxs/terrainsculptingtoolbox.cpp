#include "terrainsculptingtoolbox.h"

//******************************************
// Sculpting Toolbox Initilization Function
//******************************************
/*
Initialize the menu dropbox box class.

PARAMETERS:

*/
void TerrainSculptingToolbox::Init(float posx, float posy,Properties *props,ISoundEngine* audioengine)
{
    this->props=props;
    this->audioengine=audioengine;

    this->x=posx;
    this->y=posy;

    lh=0.3f;
    lw=0.1f;

    // Setup the frame
    frame.Init(0.1f,0.3f,0.01f,*props);
    frame.SetColors(glm::vec4(0.6f,0.6f,0.6f,0.5f),glm::vec4(0.2f,0.2f,0.2f,1.0f));

    // Setup the screen writer class
    text.Setup("../Fonts/FreeSans.ttf",props->WinWidth,props->WinHeight,props->FontSize);

    // Setup the buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",0.5,*props,audioengine);
    buttons.DefineNewButton("Close",x,y-0.24f,true);
};

//************************************
//         Draw the The ToolBox
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void TerrainSculptingToolbox::DrawToolBox()
{
    // Draw
    text.RenderTextCentered("Sculpting Tool",1,x,1,0.25,1.1,glm::vec3(1.0f));
    frame.DrawBoxPos(x,y);
    buttons.DrawButtons();
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void TerrainSculptingToolbox::Cleanup()
{
    frame.Cleanup();
    text.Cleanup();
    buttons.Cleanup();
};


//************************************
//       Check Input Events
//************************************
/*

*/
int TerrainSculptingToolbox::UpdateEvents(InputStruct &input)
{
    double xmp,ymp;
    input.ReturnMousePos(xmp,ymp);
    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);

    int bpress = buttons.UpdateButtonEvents(input);

    // Find out which button the mouse is over, -1 if none
    bool hover=CheckMouseOver(xmp,ymp);

    if (hover && mpress)
    {
        input.SetMouseKeyFalse(GLFW_MOUSE_BUTTON_LEFT);
    }

    int rtn=-1;
    if (bpress==0)
    {
        rtn=0;
        buttons.SetActiveButton(0);
    }

    if (bpress==1)
    {
        rtn=1;
        buttons.SetActiveButton(1);
    }

    return rtn;
};

//************************************
//  Check if Mouse is Over the Box
//************************************
/*
This function checks if the mouse, at x,y coords,
is over the function, then sets that button
active if it is.
*/
bool TerrainSculptingToolbox::CheckMouseOver(float xmp,float ymp)
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

    // Return Result
    return Over;
};
