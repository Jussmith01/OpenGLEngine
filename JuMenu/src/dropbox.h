#define GLEW_STATIC
#ifndef JUMENUDROPBOX_H
#define JUMENUDROPBOX_H

#include "../JuMenuDeps.h"
#include "boxcolored.h"

namespace JuMenu {

using namespace irrklang;
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

//*****************************************//
//	           Menu Button Class	       //
//*****************************************//
class MenuDropBox
{
    // Variables
    glm::vec3 textcolor[2];
    std::string sound;
    float swidth,sheight;
    float x,y;
    float lh,lw;
    float length;
    float height;
    float scale;
    float menushift;

    int selection;
    int mouseover;
    bool active;

    std::vector<std::string> data;

    double dt;//For press timing

    // Tools
    ImageDisplay image[2];
    DisplayBox box[2];
    ScreenWriter text;
    ISoundEngine* audioengine;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(std::string boximage,std::string boxsound,
              double x, double y,float length,float height,float scale,
              Properties &props,ISoundEngine* audioengine
              ,std::vector<std::string> &data);

    // Cleanup function for destruction
    void Cleanup();

    // Returns ButtonID if button was Pressed and Released while over a Button
    void UpdateDropBoxEvents(InputStruct &input);

    // Set the colors
    void SetColors(glm::vec3 c1,glm::vec3 c2,glm::vec4 c3,glm::vec4 c4,glm::vec4 c5,glm::vec4 c6);

    // Draw all buttons to the screen
    void DrawDropBox();

    void SetSelection(int i);

    int SelectionState(void);

    int CheckMouseOverSelection(float xmp,float ymp);
};

};

#endif
