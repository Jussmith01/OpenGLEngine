#include "terraincreationtoolbox.h"

//************************************
// Drop Box Initilization Function
//************************************
/*
Initialize the menu dropbox box class.

PARAMETERS:

*/
void TerrainCreationToolbox::Init(float posx, float posy,Properties *props,ISoundEngine* audioengine,TerrainCreationData &tcdata)
{
    this->props=props;
    this->audioengine=audioengine;

    this->x=posx;
    this->y=posy;

    lh=0.21f;
    lw=0.3f;

    // Setup the frame
    frame.Init(0.3f,0.21f,0.01f,*props);
    frame.SetColors(glm::vec4(0.6f,0.6f,0.6f,0.5f),glm::vec4(0.2f,0.2f,0.2f,1.0f));

    // Setup the insertion boxes
    insertbox[0].Init(x+0.05,y+0.15,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss1;
    ss1 << log2(atoi(tcdata.terrainSize.c_str())-1);
    insertbox[0].SetData(ss1.str());

    insertbox[1].Init(x+0.05,y+0.09,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss2;
    ss2 << tcdata.heightVariation[0];
    insertbox[1].SetData(ss2.str());

    insertbox[2].Init(x+0.05,y+0.03,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss3;
    ss3 << tcdata.heightVariation[1];
    insertbox[2].SetData(ss3.str());

    insertbox[3].Init(x+0.05,y-0.03,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss4;
    ss4 << tcdata.heightVariation[2];
    insertbox[3].SetData(ss4.str());

    insertbox[4].Init(x+0.05,y-0.09,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss5;
    ss5 << tcdata.NSmooth;
    insertbox[4].SetData(ss5.str());

    insertbox[5].Init(x+0.05,y-0.15,0.04f,"solid",*props,audioengine,0);
    std::stringstream ss6;
    ss6 << tcdata.SubDivision;
    insertbox[5].SetData(ss6.str());

    // Setup the screen writer class
    text.Setup("../Fonts/FreeSans.ttf",props->WinWidth,props->WinHeight,props->FontSize);

    // Setup the buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",0.5,*props,audioengine);
    buttons.DefineNewButton("Create",x+0.20f,y+0.10f,false);
    buttons.DefineNewButton("Cancel",x+0.20f,y-0.10f,true);
};

//************************************
//         Draw the Check Box
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void TerrainCreationToolbox::DrawToolBox()
{
    // Draw
    frame.DrawBoxPos(x,y);

    std::stringstream ss;
    ss << "Terrain Size (" << pow(2,insertbox[0].FetchDataInteger())+1 << "):";
    text.RenderTextLeftJustified(ss.str(),x-0.27,y+0.15,1.0f,glm::vec3(1.0f));
    insertbox[0].DrawInsBox();
    text.RenderTextLeftJustified("Initial Heights (0-1000): ",x-0.27,y+0.09,1.0f,glm::vec3(1.0f));
    insertbox[1].DrawInsBox();
    text.RenderTextLeftJustified("Initial Drop (0-1000): ",x-0.27,y+0.03,1.0f,glm::vec3(1.0f));
    insertbox[2].DrawInsBox();
    text.RenderTextLeftJustified("Initial Rise (0-1000): ",x-0.27,y-0.03,1.0f,glm::vec3(1.0f));
    insertbox[3].DrawInsBox();
    text.RenderTextLeftJustified("Smooth Cycles: ",x-0.27,y-0.09,1.0f,glm::vec3(1.0f));
    insertbox[4].DrawInsBox();
    text.RenderTextLeftJustified("Terrain SubDivision: ",x-0.27,y-0.15,1.0f,glm::vec3(1.0f));
    insertbox[5].DrawInsBox();

    buttons.DrawButtons();
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void TerrainCreationToolbox::Cleanup()
{
    frame.Cleanup();
    insertbox[0].Cleanup();
    insertbox[1].Cleanup();
    insertbox[2].Cleanup();
    insertbox[3].Cleanup();
    insertbox[4].Cleanup();
    insertbox[5].Cleanup();
    text.Cleanup();
    buttons.Cleanup();
};


//************************************
//      Check Button States
//************************************
/*

*/
int TerrainCreationToolbox::UpdateEvents(InputStruct &input)
{
    double xmp,ymp;
    input.ReturnMousePos(xmp,ymp);
    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);

    insertbox[0].UpdateEvents(input);
    insertbox[1].UpdateEvents(input);
    insertbox[2].UpdateEvents(input);
    insertbox[3].UpdateEvents(input);
    insertbox[4].UpdateEvents(input);
    insertbox[5].UpdateEvents(input);

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
bool TerrainCreationToolbox::CheckMouseOver(float xmp,float ymp)
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
TerrainCreationData TerrainCreationToolbox::FetchTerrainCreationData()
{
    TerrainCreationData tcdata;

    std::stringstream ss;
    ss << pow(2,insertbox[0].FetchDataInteger())+1;

    tcdata.terrainSize=ss.str();
    tcdata.heightVariation[0]=insertbox[1].FetchDataString();
    tcdata.heightVariation[1]=insertbox[2].FetchDataString();
    tcdata.heightVariation[2]=insertbox[3].FetchDataString();
    tcdata.NSmooth=insertbox[4].FetchDataString();
    tcdata.SubDivision=insertbox[5].FetchDataString();

    return tcdata;
};
