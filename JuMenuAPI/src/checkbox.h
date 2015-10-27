#define GLEW_STATIC
#ifndef MENUCHKBOX_H
#define MENUCHKBOX_H

#include "../JuMenu.h"

namespace JuMenu {

using namespace irrklang;

//*****************************************//
//	           Menu Button Class	       //
//*****************************************//
class MenuCheckBox
{
    // Variables
    //glm::vec3 textcolor[2];
    //glm::vec4 bordercolor[2];
    //glm::vec4 bkgrndcolor[2];
    std::string sound;
    float swidth,sheight;
    float x,y;
    float lh,lw;
    bool checked;

    double dt;//For press timing

    // Tools
    ImageDisplay image[2];
    ISoundEngine* audioengine;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(std::string boximage,std::string boxsound,
              float scale,Properties &props,ISoundEngine* audioengine);

    // Cleanup function for destruction
    void Cleanup(void);

    // Returns ButtonID if button was Pressed and Released while over a Button
    void UpdateCheckBoxEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawCheckBox(double x,double y);

    void Set(bool State);

    void SetChecked(void);

    void SetUnchecked(void);

    bool CheckedState(void);
};

};

#endif
