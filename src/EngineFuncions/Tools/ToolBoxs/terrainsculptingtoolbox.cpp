#include "terrainmodificationtoolbox.h"

//************************************
// Drop Box Initilization Function
//************************************
/*
Initialize the menu dropbox box class.

PARAMETERS:

*/
void TerrainModificationToolbox::Init(float posx, float posy,Properties *props,ISoundEngine* audioengine,TerrainModificationData &tmdata)
{
    this->props=props;
    this->audioengine=audioengine;

    this->x=posx;
    this->y=posy;

    lh=0.2f;
    lw=0.3f;

    // Setup the frame
    frame.Init(0.3f,0.2f,0.01f,*props);
    frame.SetColors(glm::vec4(0.6f,0.6f,0.6f,0.5f),glm::vec4(0.2f,0.2f,0.2f,1.0f));

    // Setup the insertion boxes
    insertbox[0].Init(x+0.05,y+0.15,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss1;
    ss1 << tmdata.heightMult;
    insertbox[0].SetData(ss1.str());

    insertbox[1].Init(x+0.05,y+0.05,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss2;
    ss2 << tmdata.relheight[0];
    insertbox[1].SetData(ss2.str());

    insertbox[2].Init(x+0.05,y-0.05,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss3;
    ss3 << tmdata.relheight[1];
    insertbox[2].SetData(ss3.str());

    insertbox[3].Init(x+0.05,y-0.15,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss4;
    ss4 << tmdata.relheight[2];
    insertbox[3].SetData(ss4.str());

    // Setup the screen writer class
    text.Setup("../Fonts/FreeSans.ttf",props->WinWidth,props->WinHeight,props->FontSize);

    // Setup the buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",0.5,*props,audioengine);
    buttons.DefineNewButton("Modify",x+0.20f,y+0.10f,false);
    buttons.DefineNewButton("Cancel",x+0.20f,y-0.10f,true);
};

//************************************
//         Draw the The ToolBox
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void TerrainModificationToolbox::DrawToolBox()
{
    // Draw
    frame.DrawBoxPos(x,y);

    text.RenderTextLeftJustified("Height Multiplier: ",x-0.27,y+0.17,1.0f,glm::vec3(1.0f));
    insertbox[0].DrawInsBox();
    text.RenderTextLeftJustified("Low->Mid Switch (0-1): ",x-0.27,y+0.07,1.0f,glm::vec3(1.0f));
    insertbox[1].DrawInsBox();
    text.RenderTextLeftJustified("Mid->High Switch (0-1): ",x-0.27,y-0.03,1.0f,glm::vec3(1.0f));
    insertbox[2].DrawInsBox();
    text.RenderTextLeftJustified("Switch Mixing (0-1): ",x-0.27,y-0.13,1.0f,glm::vec3(1.0f));
    insertbox[3].DrawInsBox();

    buttons.DrawButtons();
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void TerrainModificationToolbox::Cleanup()
{
    frame.Cleanup();
    insertbox[0].Cleanup();
    insertbox[1].Cleanup();
    insertbox[2].Cleanup();
    insertbox[3].Cleanup();
    text.Cleanup();
    buttons.Cleanup();
};


//************************************
//       Check Input Events
//************************************
/*

*/
int TerrainModificationToolbox::UpdateEvents(InputStruct &input)
{
    double xmp,ymp;
    input.ReturnMousePos(xmp,ymp);
    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);

    insertbox[0].UpdateEvents(input);
    insertbox[1].UpdateEvents(input);
    insertbox[2].UpdateEvents(input);
    insertbox[3].UpdateEvents(input);

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
bool TerrainModificationToolbox::CheckMouseOver(float xmp,float ymp)
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

//************************************
//           Get the Data
//************************************
/*
*/
TerrainModificationData TerrainModificationToolbox::FetchTerrainModificationData()
{
    TerrainModificationData tmdata;

    tmdata.heightMult=insertbox[0].FetchDataString();
    tmdata.relheight[0]=insertbox[1].FetchDataString();
    tmdata.relheight[1]=insertbox[2].FetchDataString();
    tmdata.relheight[2]=insertbox[3].FetchDataString();

    return tmdata;
};
