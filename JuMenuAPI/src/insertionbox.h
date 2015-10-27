#ifndef INSERTIONBOX_C
#define INSERTIONBOX_C

#include "../JuMenu.h"
#include "boxcolored.h"

namespace JuMenu {

using namespace irrklang;

//*****************************************//
//	           Menu Button Class	       //
//*****************************************//
class InsertionBox
{
    // Variables
    std::string sound;
    std::string data;
    float swidth,sheight;
    float width;
    float x,y;
    float lh,lw;

    bool BoxActive;
    bool Shift;
    bool Caps;

    int keyset;

    double dt;//For press timing

    // Tools
    ISoundEngine* audioengine;

    // Functions
    keyhandler kh;
    DisplayBox frame;
    ScreenWriter text;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(float posx,float posy,float width,std::string sound,Properties &props,ISoundEngine* audioengine,int keyset);

    // Cleanup function for destruction
    void Cleanup(void);

    // Returns ButtonID if button was Pressed and Released while over a Button
    void UpdateEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawInsBox();

    // Set the Data
    void SetData(std::string data);

    // Set Numeric Values
    void SetNumericalValues(InputStruct &input);

    // Set Values
    void SetValues(InputStruct &input);

    // Return the data in float form
    float FetchDataFloat();

    // Return the data in int form
    int FetchDataInteger();

    // Return the data in string form
    std::string FetchDataString();
};

};
#endif
