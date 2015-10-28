#ifndef INPUTSTRUCT_C
#define INPUTSTRUCT_C

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "../engine.h"
#include "tools.hpp"
#include "../Loaders/properties.h"

//************************************
//      The Key Handler Struct
//************************************
/*
This type can be used to return when a
key has been press has been pressed then
released. It works by calling

        CheckKeyState(bool arg)

with an argument (arg) of type bool. This
function returns false until the following
occurs: arg=true then at a later time
arg=false, upon reading the latter arg=false
the function returns true then resets. This
prevents holding a button and getting multiple
trues with just a single press of the button,
while at the same time allowing this to be
fixed within a class of your choice rather
than at the event handler level.
*/
struct keyhandler
{
    private:

    bool chkset[1024];

    public:
    keyhandler()
    {
        for(int i=0;i<1024;++i)
            chkset[i]=false;
    };

    //***********************
    // Check The Keys State
    //***********************
    bool CheckKeyState(bool keyval,int keyidx)
    {
        bool rtn;
        if(!chkset[keyidx])
        {
            if(keyval)
            {
                chkset[keyidx]=true;
            }
            rtn=false;
        } else {
            if(keyval)
            {
                rtn=false;
            } else {
                rtn=true;
                chkset[keyidx]=false;
            }
        }
        return rtn;
    };
};

//************************************
//      The Input Handler Struct
//************************************
/*
This stuct is used for passing input data
around the program so that event handling
can be carried out centrally within a
given state.
*/
struct InputStruct
{
    // Basic Variables
    bool keys[1024]; // Keyboard Keys
    bool mousekeys[8]; //Mouse Keys
    double mx,my; // Mouse Position
    double sx,sy; // Scroll Position

    // Basic Motion Tracking

    GrafixEngine *game;

    InputStruct() {};

    //***********************
    // Initialize the Struct
    //***********************
    /*
    This initializes the the struct taking
    and Engine pointer as an arguement so
    that the Engine can be registered and
    used by the class. Also Initializes the
    all keys to false;
    */
    void Init(GrafixEngine *game)
    {
        // Register the game Engine with the clasee
        this->game=game;

        // Initialize all keys to false
        for (int i=0;i<1024;++i)
        {
            keys[i]=false;
        }

        // Initialize all mouse keys to false
        for (int i=0;i<8;++i)
        {
            mousekeys[i]=false;
        }

        //Initialize positions
        mx=0.0f;my=0.0f;
        sx=0.0f;sy=0.0f;
    };

    //***********************
    // Keyboard Key Handling
    //***********************
    /*
    SetKeyTrue - Sets the given key(i) to true
    SetKeyFalse - Sets the given key(i) to false
    GetKey - Get the truth value of the given key(i)
    GetKeyAndReset - Get the truth value of the
                     given key and reset it. Good
                     for non-repeat buttons.
    */
    void SetKeyTrue(int i)
    {
        keys[i]=true;
    };

    void SetKeyFalse(int i)
    {
        keys[i]=false;
    };

    bool GetKey(int i)
    {
        return keys[i];
    };

    bool GetKeyAndReset(int i)
    {
        bool rtn=keys[i];
        keys[i]=false;
        return rtn;
    };

    //********************
    // Mouse Key Handling
    //********************
    /*
    SetMouseKeyTrue - Sets the given key(i) to true
    SetMouseKeyFalse - Sets the given key(i) to false
    GetMouseKey - Get the truth value of the given key(i)
    GetMouseKeyAndReset - Get the truth value of the
                          given key and reset it. Good
                          for non-repeat buttons.
    */
    void SetMouseKeyTrue(int i)
    {
        mousekeys[i]=true;
    };

    void SetMouseKeyFalse(int i)
    {
        mousekeys[i]=false;
    };

    bool GetMouseKey(int i)
    {
        return mousekeys[i];
    };

    bool GetMouseKeyAndReset(int i)
    {
        bool rtn=mousekeys[i];
        mousekeys[i]=false;
        return rtn;
    };


    //**********************
    // Mouse Input Handling
    //**********************
    /*
    SetMouseGLFW - Sets the position of the mouse in
                   Normalized Device Coordinates using
                   glfwGetCursorPos() function.
    SetMousePosPDC - Sets the mouse pos given Pixel
                     Device Coordinates(PDC), converts
                     to Normalized Device Coordinates
                     NDC.
    SetMousePosNDC - Sets the mouse pos given NDC.
    ReturnMousePos - Returns the mouse pos, always
                     works in NDC.

    */
    void SetMouseGLFW()
    {
        double xpos;
        double ypos;
        glfwGetCursorPos(game->window,&xpos,&ypos);
        tools::normal_device_coordinate(xpos,ypos,game->props.WinWidth,game->props.WinHeight); // Convert mouse coords to NDC
        mx=xpos;
        my=ypos;
    };

    void SetMousePosPDC(double x,double y)
    {
        double xpos=x;
        double ypos=y;
        //std::cout << xpos << " " << ypos <<  std::endl;
        tools::check_mouse_reset(game->window,xpos,ypos,game->props.WinWidth,game->props.WinHeight,0.0f); // Prevent mouse from leaving window
        tools::normal_device_coordinate(xpos,ypos,game->props.WinWidth,game->props.WinHeight); // Convert mouse coords to NDC
        mx=xpos;
        my=ypos;
    };

    void SetMousePosNDC(double x,double y)
    {
        mx=x;
        my=y;
    };

    void ReturnMousePos(double &x,double &y)
    {
        x=mx;
        y=my;
    };

    //*********************
    //Scroll Input Handling
    //*********************
    /*
    SetScrollPos - Sets the position of the scrolling
    ReturnScrollPos - Returns the position of the scrolling
    */
    void SetScrollPos(double x,double y)
    {
        sx+=x;
        sy+=y;
    };

    void ReturnScrollPos(double &x,double &y)
    {
        x=sx;
        y=sy;
    };
};

#endif
