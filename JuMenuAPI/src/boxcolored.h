#define GLEW_STATIC
#ifndef JUDISPLAYBOX_H
#define JUDISPLAYBOX_H

#include "../JuMenuDeps.h"

namespace JuMenu {

//*****************************************//
//	         Display Box Class	           //
//*****************************************//
class DisplayBox
{
    // Variables
    glm::vec4 bdrcolor;
    glm::vec4 bkgcolor;
    double scalex,scaley,bwidth;
    float swidth,sheight;

    // OGL Variables
    Shader shader;
    GLuint VBObg, VAObg, EBObg;//For Background
    GLuint VBObd, VAObd, EBObd;//For Border

public:
    // Initialize Box Class
    void Init(double scalex, double scaley,double border,Properties &props);

    // Sets the border and bg colors, allows for alpha
    void SetColors(glm::vec4 bkgcolor,glm::vec4 bdrcolor);

    // Cleanup function for destruction
    void Cleanup(void);

    // Draw the box and border to the screen center
    void DrawBox(void);

    // Draw the box and border to the screen at x,y position (NDC)
    void DrawBoxPos(float x,float y);

    //****************
    // AUTO FUNCTIONS
    //****************

    // Setup the mesh for the background
    void SetupBackground(void);

    // Setup the mesh for the border
    void SetupBorder(void);
};

};

#endif
