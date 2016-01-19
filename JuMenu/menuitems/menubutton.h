#define GLEW_STATIC
#ifndef JUMENUBUTTON_H
#define JUMENUBUTTON_H

#include "../JuMenuDeps.h"
#include "../JuMenuResourceManager.h"

namespace JuMenu {

using namespace irrklang;
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

//*****************************************//
//	            Button Struct   	       //
//*****************************************//
/*

*/
struct Button {
    int state; // Button State
    float x,y; // Button Position
    std::string caption; // Button Text
    std::string ident;
    std::string action;

    // Constuctors
    Button () {}; // Default

    Button (std::string ident,std::string caption,std::string action,float x, float y) {
        this->x=x;
        this->y=y;
        this->ident=ident;
        this->caption=caption;
        this->action=action;
        this->state=0;
    };

    //***********************
    // Initialize the Struct
    //***********************
    /*
    Function returns true if the mouse
    is over the button given the xmp,ymp
    mouse coordinates and lw,lh button
    width and height.
    */
    bool CheckIfOver(float xmp,float ymp,float lw,float lh) {
        // Check X over
        bool xchk=false;
        float xl=x-lw;
        float xr=x+lw;
        if (xmp>=xl && xmp<=xr)
            xchk=true;

        // Check y over
        bool ychk=false;
        float yb=y-lh;
        float yt=y+lh;
        if (ymp>=yb && ymp<=yt)
            ychk=true;

        // Check if both are true
        bool Over=false;
        if (xchk && ychk)
            Over=true;

        // Return Result
        return Over;
    };
};

//*****************************************//
//	           Menu Button Class	       //
//*****************************************//
class MenuButtons {

    // Variables
    glm::vec3 colors[3];

    // Buttons and button parameters
    std::vector<Button> button;
    float swidth,sheight;
    float lw,lh;
    float x,y;

    float spread;
    float scale;

    int activebutton;
    int hoverbutton;

    // Key handlers for checking press and release
    keyhandler kh[4];

    float lastuse;
    float alastuse;

    // Set From m_setmenubuttons
    std::string orient;
    std::string soundactive;
    std::string soundpress;

    // Provided Resources
    std::shared_ptr<MenuResourceBase> image1;
    std::shared_ptr<MenuResourceBase> image2;
    std::shared_ptr<MenuResourceBase> image3;
    //std::shared_ptr<MenuResourceBase> swrite;

    // Set From Construction
    std::string ident;
    MenuResourceManager *pmrm;

    // Check if mouse is over button
    void m_checkmouseover(float x,float y);

    //Check if button is pressed
    void m_checkpress(bool press,int ButtonID);

    // Setup the menu dropbox via the setup string (Called from constructor)
    void m_setmenubuttons( std::string &dstr ) ;

    // Set the classes parameters via input string (ran from m_setmenubuttons)
    void m_setparameters( std::string dstr );

    // Add a button to the class (ran from m_setmenubuttons)
    void m_addbutton( std::string dstr );

public:

    // Constructor
    MenuButtons ( std::string ident,std::string &dstr,MenuResourceManager *pmrm )
        : ident(ident),pmrm(pmrm) {
        m_setmenubuttons(dstr);
    };

    // Initialize Button Class
    void Init( std::string buttonimage,std::string buttonsound,std::string font,
               float scale,float x, float y,float spread,std::string orient );

    // Sets the font colors for 1) normal 2) hovering over and 3) pressing
    void SetFontColors(glm::vec3 normal,glm::vec3 hover,glm::vec3 press);

    // Cleanup function for destruction
    void cleanup();

    // Adds a new button to the class
    void DefineNewButton(std::string ident,std::string caption,std::string action,std::string active);

    // Returns ButtonID if button was Pressed and Released while over a Button
    //int update(InputStruct &input);
    void update(void);

    // Draw all buttons to the screen
    void draw(void);

    // input arrow up or arrow down true to change the active button
    void ArrowChangeActive(bool aup,bool adown);

    // Function resets all buttons to default state
    void ResetButtonStates();

    // Function sets the active button given button ID.
    void SetActiveButton(int ButtonID);

    /** Request Functions **/
};

};

#endif
