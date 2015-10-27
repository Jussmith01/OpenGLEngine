#include "dropdownmenu.h"

//************************************
// Drop Down Menu Initilization Function
//************************************
/*
Initialize the drop down menu class.

PARAMETERS:

*/
void JuMenu::DropDownMenu::Init(std::string title,double x, double y,float length,float height,float scale,
                       Properties &props ,std::vector<std::string> &data)
{
    // Define needed variables
    this->x=x;
    this->y=y;
    this->swidth=props.WinWidth;
    this->sheight=props.WinHeight;
    this->scale=scale;
    this->length=length*scale;
    this->height=height*scale;
    this->active=false;
    this->mouseover=0;
    this->title=title;

    for(int i=0; i<(int)data.size(); ++i)
    {
        this->data.push_back(data[i]);
    }

    selection=-1;

    // Setup box images on GPU
    //float is=height*0.8;

    // Setup Boxes
    lh=this->height;
    lw=this->length;

    box[0].Init(this->length/2.0,this->height,0.005,props);
    box[0].SetColors(glm::vec4(0.7f),glm::vec4(0.4f,0.4f,0.4f,1.0f));

    box[1].Init(this->length,1.5*this->height*(float)data.size(),0.005,props);
    box[1].SetColors(glm::vec4(0.7f),glm::vec4(0.4f,0.4f,0.4f,1.0f));

    menushift=1.5*(this->height*(float)data.size())+1.5*this->height;

    //Setup screen writer
    text.Setup("../Fonts/FreeSans.ttf",swidth,sheight,props.FontSize*scale);

    textcolor[0]=glm::vec3(1.0,1.0,1.0);
    textcolor[1]=glm::vec3(1.0,0.0,0.0);

    dt=0.0;
};

//************************************
//            Set Colors
//************************************
/*

*/
void JuMenu::DropDownMenu::SetColors(glm::vec3 c1,glm::vec3 c2,glm::vec4 c3,glm::vec4 c4,glm::vec4 c5,glm::vec4 c6)
{
    textcolor[0]=c1;
    textcolor[1]=c2;

    box[0].SetColors(c3,c4);
    box[1].SetColors(c5,c6);
};

//************************************
//         Set the Selection
//************************************
/*

*/
void JuMenu::DropDownMenu::SetSelection(int i)
{
    selection=i;
};

//************************************
//         Draw the Check Box
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void JuMenu::DropDownMenu::Draw()
{
    // Draw
    if (active)
    {
        box[1].DrawBoxPos(x,y-menushift);

        for(int i=0; i<(int)data.size(); ++i)
        {
            if (i==mouseover)
            {
                text.RenderTextCentered(data[i],1,x,1,y-2.5*(i+1)*height,height*60,textcolor[1]);
            }
            else
            {
                text.RenderTextCentered(data[i],1,x,1,y-2.5*(i+1)*height,height*60,textcolor[0]);
            }
        }
    }

    box[0].DrawBoxPos(x-length/2.0f,y);
    //text.RenderTextCentered(data[selection],1,0.0,1,0.5,scale,textcolor[0]);
    text.RenderTextCentered(title,1,x-length/2.0f,1,y,height*75,textcolor[0]);
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void JuMenu::DropDownMenu::Cleanup()
{
    box[0].Cleanup();
    box[1].Cleanup();
    text.Cleanup();
    data.clear();
};


//************************************
//      Check Button States
//************************************
/*

*/
void JuMenu::DropDownMenu::UpdateEvents(InputStruct &input)
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

        if (!active)
        {
            active=true;
        }
        else
        {
            active=false;
        }
    }

    if(active)
    {
        mouseover=CheckMouseOverSelection(xmp,ymp);
        if (mpress && (glfwGetTime()-dt)>0.5)
        {
            selection=mouseover;
            active=false;
            input.SetMouseKeyFalse(GLFW_MOUSE_BUTTON_LEFT);
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
bool JuMenu::DropDownMenu::CheckMouseOver(float xmp,float ymp)
{
    // Check X over
    bool xchk=false;
    float xl=x-length/2.0f-lw/2.0f;
    float xr=x-length/2.0f+lw/2.0f;
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
//  Check if Mouse is Over the Box
//************************************
/*
This function checks if the mouse, at x,y coords,
is over the function, then sets that button
active if it is.
*/
int JuMenu::DropDownMenu::CheckMouseOverSelection(float xmp,float ymp)
{
    int rtnmo=selection;
    for(int i=0; i<(int)data.size(); ++i)
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
        float yb=(y-lh*1.5)-2.5*(i+1)*height;
        float yt=(y+lh*1.5)-2.5*(i+1)*height;
        if (ymp>=yb && ymp<=yt)
        {
            ychk=true;
        }

        // Check if both are true
        if (xchk && ychk)
        {
            rtnmo=i;
        }
    }
    //std::cout << "xleft: " << xl << " xright: " << xr << " xmp: " << xmp << " yb: " << yb << " yt: " << yt << " ymp: " << ymp << std::endl;

    // Return Result
    return rtnmo;
};

//************************************
//     Returns the Checked State
//************************************
/*

*/
int JuMenu::DropDownMenu::SelectionState()
{
    // Return Result
    int slc=selection;
    selection=-1;
    return slc;
};
