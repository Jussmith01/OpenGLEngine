#include "dropbox.h"

//************************************
// Drop Box Initilization Function
//************************************
/*
Initialize the menu dropbox box class.

PARAMETERS:

*/
void JuMenu::MenuDropBox::Init(std::string boximage,std::string boxsound,
                       double x, double y,float length,float height,float scale,
                       Properties &props,ISoundEngine* audioengine
                       ,std::vector<std::string> &data) {
    // Define needed variables
    this->x=x;
    this->y=y;
    this->swidth=props.WinWidth;
    this->sheight=props.WinHeight;
    this->scale=scale;
    this->length=length;
    this->height=height;
    this->active=false;
    this->mouseover=0;

    for(int i=0; i<(int)data.size(); ++i)
    {
        this->data.push_back(data[i]);
    }

    selection=0;

    // Setup Image Filenames
    std::stringstream fn1,fn2; //File names for images
    fn1 << boximage << "_undrop.png";
    fn2 << boximage << "_drop.png";

    // Setup box images on GPU
    float is=height*0.8;

    image[0].Init(fn1.str(),is,is,swidth,sheight);
    image[1].Init(fn2.str(),is,is,swidth,sheight);

    // Obtain Image Border
    lw=image[0].GetImageHalfLength();
    lh=image[0].GetImageHalfHeight();

    // Setup Audio Engine
    this->audioengine = audioengine;

    // Setup audio file
    std::stringstream s1;
    s1 << "../Data/Sounds/" << boxsound << "_press.wav";
    sound = s1.str();

    // Setup Boxes
    lh=height;
    lw=length;

    box[0].Init(length,height,0.005,props);
    box[0].SetColors(glm::vec4(0.7f),glm::vec4(0.4f,0.4f,0.4f,1.0f));

    box[1].Init(length,2*height*(float)data.size(),0.005,props);
    box[1].SetColors(glm::vec4(0.7f),glm::vec4(0.4f,0.4f,0.4f,1.0f));

    menushift=2*(height*(float)data.size())+1.5*height;

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
void JuMenu::MenuDropBox::SetColors(glm::vec3 c1,glm::vec3 c2,glm::vec4 c3,glm::vec4 c4,glm::vec4 c5,glm::vec4 c6)
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
void JuMenu::MenuDropBox::SetSelection(int i)
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
void JuMenu::MenuDropBox::DrawDropBox()
{
    // Draw
    if (active)
    {
        box[1].DrawBoxPos(x,y-menushift);

        for(int i=0; i<(int)data.size(); ++i)
        {
            if (i==mouseover)
            {
                text.RenderTextCentered(data[i],1,x,1,y-4.0*(i+1)*height,height*50,textcolor[1]);
            }
            else
            {
                text.RenderTextCentered(data[i],1,x,1,y-4.0*(i+1)*height,height*50,textcolor[0]);
            }
        }
    }
    box[0].DrawBoxPos(x,y);
    //text.RenderTextCentered(data[selection],1,0.0,1,0.5,scale,textcolor[0]);
    //text.RenderTextCentered(data[selection],1,x,1,y,height*50,textcolor[0]);
    //image[(int)active].DrawImagePos(x+length,y);
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void JuMenu::MenuDropBox::Cleanup()
{
    image[0].Cleanup();
    image[1].Cleanup();
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
void JuMenu::MenuDropBox::UpdateDropBoxEvents(InputStruct &input)
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
bool JuMenu::MenuDropBox::CheckMouseOver(float xmp,float ymp)
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
//  Check if Mouse is Over the Box
//************************************
/*
This function checks if the mouse, at x,y coords,
is over the function, then sets that button
active if it is.
*/
int JuMenu::MenuDropBox::CheckMouseOverSelection(float xmp,float ymp)
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
        float yb=(y-lh*2.0)-4.0*(i+1)*height;
        float yt=(y+lh*2.0)-4.0*(i+1)*height;
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
int JuMenu::MenuDropBox::SelectionState()
{
    // Return Result
    return selection;
};
