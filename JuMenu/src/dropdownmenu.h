#ifndef JUDROPDOWNMENU_H
#define JUDROPDOWNMENU_H

#include "../JuMenuDeps.h"
#include "boxcolored.h"

//*****************************************//
//	         Drop Down Menu Class	       //
//*****************************************//
namespace JuMenu {

class DropDownMenu {
    // Variables
    glm::vec3 textcolor[2];
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

    std::string title;
    std::vector<std::string> data;

    double dt;//For press timing

    // Tools
    DisplayBox box[2];
    ScreenWriter text;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    DropDownMenu() {};

    // Initialize Button Class
    void Init(std::string title,double x, double y,float length,float height,float scale,
              Properties &props,std::vector<std::string> &data);

    // Cleanup function for destruction
    void Cleanup();

    // Returns ButtonID if button was Pressed and Released while over a Button
    void UpdateEvents(InputStruct &input);

    // Set the colors
    void SetColors(glm::vec3 c1,glm::vec3 c2,glm::vec4 c3,glm::vec4 c4,glm::vec4 c5,glm::vec4 c6);

    // Draw all buttons to the screen
    void Draw();

    void SetSelection(int i);

    int SelectionState(void);

    int CheckMouseOverSelection(float xmp,float ymp);
};

};

#endif
