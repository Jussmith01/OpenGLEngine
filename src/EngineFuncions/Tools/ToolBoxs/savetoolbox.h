#ifndef SAVETOOLBOX_C
#define SAVETOOLBOX_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"

#include "../MenuItems.h"
#include "../displayimage.h"
#include "../screenwriter.h"
#include "../input_struct.h"

#include <irrKlang.h>
#include <ik_ISound.h>
using namespace irrklang;
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

//*****************************************//
//	            Save Toolbox	           //
//*****************************************//
class SaveToolbox
{
    // Variables
    std::string sound;
    float swidth,sheight;
    float x,y;
    float lh,lw;

    double dt;//For press timing

    // Tools
    Properties *props;
    ISoundEngine* audioengine;

    DisplayBox frame;
    ScreenWriter text;

    // Testing!
    InsertionBox insertbox;
    MenuButtons buttons;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(std::string buttontext,float posx, float posy,Properties* props,ISoundEngine* audioengine);

    // Cleanup function for destruction
    void Cleanup();

    // Returns ButtonID if button was Pressed and Released while over a Button
    int UpdateEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawToolBox();

    // Get Filename
    std::string GetFileName();
};


#endif
