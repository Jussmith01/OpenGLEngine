#ifndef TERRAINSCULPTINGTOOLBOX_C
#define TERRAINSCULPTINGTOOLBOX_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"

#include "../MenuItems.h"
#include "../displayimage.h"
#include "../screenwriter.h"
#include "../input_struct.h"
#include "../tools.hpp"

#include <irrKlang.h>
#include <ik_ISound.h>
using namespace irrklang;

//*****************************************//
//	     Terrain Creation Toolbox	       //
//*****************************************//
class TerrainSculptingToolbox
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

    // Testing
    MenuButtons buttons;
    MenuIcons icons;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(float posx, float posy,Properties* props,ISoundEngine* audioengine);

    // Cleanup function for destruction
    void Cleanup();

    // Returns ButtonID if button was Pressed and Released while over a Button
    int UpdateEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawToolBox();
};


#endif
