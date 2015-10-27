#include "insertionbox.h"

//************************************
// Check Box Initilization Function
//************************************
/*
Initialize the menu check box class.

PARAMETERS:

*/
void JuMenu::InsertionBox::Init(float posx,float posy,float width,std::string sound,Properties &props,ISoundEngine* audioengine,int keyset)
{
    // Define needed variables
    this->swidth=props.WinWidth;
    this->sheight=props.WinHeight;

    this->width=width;

    this->x=posx;
    this->y=posy;

    this->keyset=keyset;

    BoxActive=false;

    //Setup Text
    text.Setup("../Fonts/FreeSans.ttf",this->swidth,this->sheight,props.FontSize);

    frame.Init(width,0.025f,0.005f,props);
    frame.SetColors(glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.2f,0.2f,0.2f,1.0f));

    // Obtain Image Border
    lw=width;
    lh=0.025;

    // Setup Audio Engine
    this->audioengine = audioengine;

    // Setup audio file
    std::stringstream s1;
    s1 << "../Data/Sounds/" << sound << "_press.wav";
    sound = s1.str();

    dt=0.0;
    Shift=false;
    Caps=false;
};

//************************************
//          Set the Data
//************************************
/*

*/
void JuMenu::InsertionBox::SetData(std::string data)
{
    this->data=data;
};

//************************************
//         Draw the Check Box
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void JuMenu::InsertionBox::DrawInsBox()
{
    // Draw Frame
    frame.DrawBoxPos(x,y);

    // Draw Text
    std::string contents=data;
    std::string bar="_";

    if(BoxActive)
    {
        if ((int)(glfwGetTime()*1.5)%2 == 0)
            contents=contents+bar;
    }

    text.RenderTextLeftJustified(contents,x-width*0.95f,y,0.9f,glm::vec3(0.0f));
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void JuMenu::InsertionBox::Cleanup()
{
    frame.Cleanup();
    text.Cleanup();
};


//************************************
//      Check Button States
//************************************
/*

*/
void JuMenu::InsertionBox::UpdateEvents(InputStruct &input)
{
    double xmp,ymp;
    input.ReturnMousePos(xmp,ymp);
    bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);

    // Find out which button the mouse is over, -1 if none
    bool hover=CheckMouseOver(xmp,ymp);

    // Delete a char off the end
    bool del=kh.CheckKeyState(input.GetKey(GLFW_KEY_BACKSPACE),GLFW_KEY_BACKSPACE);
    if(del && BoxActive && !data.empty())
    {
        data.pop_back();
    }

    if (BoxActive)
    {
        if(keyset==0)
        {
            SetNumericalValues(input);
        } else if (keyset==1) {
            SetValues(input);
        }
    }

    //std::cout << "hover: " << hover << " mpress: " << mpress << std::endl;
    if(mpress)
    {
        BoxActive=false;
    }

    if(hover && mpress)
    {
        BoxActive=true;
        //input.SetMouseKeyFalse(GLFW_MOUSE_BUTTON_LEFT);
        //std::cout << "TESTING CLICK OVER!" << std::endl;
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
bool JuMenu::InsertionBox::CheckMouseOver(float xmp,float ymp)
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
//          Set Numeric Values
//************************************
/*
Set only number type values
*/
void JuMenu::InsertionBox::SetNumericalValues(InputStruct &input)
{
    // Add a char to the end
    char val[]={'0','1','2','3','4','5','6','7','8','9'};
    for (int i=48;i<58;++i)
    {
        if(kh.CheckKeyState(input.GetKeyAndReset(i),i))
        {
            data.push_back(val[i-48]);
        }
    }

    for (int i=320;i<330;++i)
    {
        if(kh.CheckKeyState(input.GetKeyAndReset(i),i))
        {
            data.push_back(val[i-320]);
        }
    }

    if(kh.CheckKeyState(input.GetKeyAndReset(45),45) || kh.CheckKeyState(input.GetKeyAndReset(333),333))
    {
        char val[]="-";
        data.push_back(val[0]);
    }

    if(kh.CheckKeyState(input.GetKeyAndReset(46),46) || kh.CheckKeyState(input.GetKeyAndReset(330),330))
    {
        char val[]=".";
        data.push_back(val[0]);
    }
};

//************************************
//          Set Numeric Values
//************************************
/*
Set only number type values
*/
void JuMenu::InsertionBox::SetValues(InputStruct &input)
{
    // Setup Caps
    if (kh.CheckKeyState(input.GetKeyAndReset(GLFW_KEY_CAPS_LOCK),GLFW_KEY_CAPS_LOCK))
    {
        if (!Caps)
        {
            Caps=true;
        } else {
            Caps=false;
        }
    }

    //Setup Shift
    if (input.GetKeyAndReset(GLFW_KEY_LEFT_SHIFT))
    {
        Shift=true;
    } else {
        Shift=false;
    }

    // Lower case chars
    char charl[]={
                'a','b','c','d','e','f','g','h','i','j',
                'k','l','m','n','o','p','q','r','s','t',
                'u','v','w','x','y','z'
               };

    // Upper case chars
    char charu[]={
                'A','B','C','D','E','F','G','H','I','J',
                'K','L','M','N','O','P','Q','R','S','T',
                'U','V','W','X','Y','Z'
               };

    for (int i=65;i<91;++i)
    {
        if(kh.CheckKeyState(input.GetKeyAndReset(i),i))
        {
            if(Caps || Shift)
            {
                data.push_back(charu[i-65]);
            } else {
                data.push_back(charl[i-65]);
            }
        }
    }
    // Add a char to the end
    char val[]={'0','1','2','3','4','5','6','7','8','9'};
    for (int i=48;i<58;++i)
    {
        if(kh.CheckKeyState(input.GetKeyAndReset(i),i))
        {
            data.push_back(val[i-48]);
        }
    }

    for (int i=320;i<330;++i)
    {
        if(kh.CheckKeyState(input.GetKeyAndReset(i),i))
        {
            data.push_back(val[i-320]);
        }
    }

    if(kh.CheckKeyState(input.GetKeyAndReset(45),45) || kh.CheckKeyState(input.GetKeyAndReset(333),333))
    {
        if(Caps || Shift)
        {
            char val[]="_";
            data.push_back(val[0]);
        } else {
            char val[]="-";
            data.push_back(val[0]);
        }
    }

    if(kh.CheckKeyState(input.GetKeyAndReset(46),46) || kh.CheckKeyState(input.GetKeyAndReset(330),330))
    {
        char val[]=".";
        data.push_back(val[0]);
    }
};

//************************************
//        Fetch Data (Float)
//************************************
/*
Fetch the Data of float
*/
float JuMenu::InsertionBox::FetchDataFloat()
{
    return atof(data.c_str());
};

//************************************
//        Fetch Data (Float)
//************************************
/*
Fetch the Data of float
*/
int JuMenu::InsertionBox::FetchDataInteger()
{
    return atoi(data.c_str());
};

//************************************
//        Fetch Data (Float)
//************************************
/*
Fetch the Data of float
*/
std::string JuMenu::InsertionBox::FetchDataString()
{
    return data;
};
