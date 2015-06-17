#include "requestinterface.h"

//-------------------------------------------//
//       Request Interface Class Functions   //
//-------------------------------------------//
/*
Used to draw a request box and return 0 or 1
dependent on which button is selected. The
text content of the box and each button is
set as arguments in the Init function.
*/
// The following functions are for the graphics settings graphical interface
void RequestInterface::Init(std::string content,std::string rtn0,std::string rtn1,Engine* game,ISoundEngine* audioengine)
{
    this->game=game;
    this->audioengine=audioengine;

    // Initialize Timeout Variables
    this->touse=false;
    this->toset=false;

    // Save content string
    this->content=content;

    // Setup text
    text.Setup("../Fonts/FreeSans.ttf",game->props.WinWidth,game->props.WinHeight,game->props.FontSize);

    // Setup the box
    box.Init(0.2,0.2,0.005,game->props);
    box.SetColors(glm::vec4(0.5,0.5,0.0,0.5),glm::vec4(0.4,0.6,0.0,1.0));

    // Setup the buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",0.5,game->props,audioengine);
    buttons.DefineNewButton(rtn0,-0.1f, -0.1f,true);
    buttons.DefineNewButton(rtn1,0.1f, -0.1f,false);
};

int RequestInterface::Update(InputStruct &input)
{
    int bID1 = buttons.UpdateButtonEvents(input);
    int bID2 = UpdateTimeout();

    int bID;
    if (bID1==-1 || bID2==-1)
    {
        bID=-1;
    }

    if (bID1==0 || bID2==0)
    {
        bID=0;
        buttons.SetActiveButton(0);
        toset=false;
    }

    if (bID1==1 || bID2==1)
    {
        bID=1;
        buttons.SetActiveButton(1);
        toset=false;
    }

    if (input.GetKey(GLFW_KEY_ESCAPE))
    {
        bID=1;
        input.SetKeyFalse(GLFW_KEY_ESCAPE);
        buttons.SetActiveButton(1);
        toset=false;
    }

    return bID;
};

void RequestInterface::SetTimeout(float totime,int selection)
{
    this->touse=true;
    this->totime=totime;
    this->ctime=ctime;
    this->selection=selection;
};

int RequestInterface::UpdateTimeout()
{
    int rtn=-1;

    if (touse)
    {
        if (!toset)
        {
            toset=true;
            stime=glfwGetTime();
        }
        else
        {
            ctime=totime-(glfwGetTime()-stime);
            if (ctime < 0)
            {
                rtn=selection;
            }
        }
    }

    return rtn;
};

void RequestInterface::Draw()
{
    // Render box
    box.DrawBoxPos(0.0,0.0);

    // Render GPI Title
    text.RenderTextCentered(content,1,0.0,1,0.1,1.2,glm::vec3(1.0));

    // Render Countdown Timer
    if (touse)
    {
        std::stringstream ss;
        ss << (int)ctime;
        text.RenderTextCentered(ss.str(),1,0.0,1,0.0,1.0,glm::vec3(1.0));
    }

    // Draw the buttons to screen
    buttons.DrawButtons();
};

void RequestInterface::Cleaup()
{
    buttons.Cleanup();
    box.Cleanup();
    text.Cleanup();
};

