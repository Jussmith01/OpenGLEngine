#include "boxcolored.h"

//************************************
// Check Box Initilization Function
//************************************
/*
Initialize the menu check box class.

PARAMETERS:

*/
void DisplayBox::Init(double scalex, double scaley,double border,Properties &props)
{
    // Setup Variables
    this->scalex=scalex;
    this->scaley=scaley;
    this->bwidth=border;

    this->swidth=props.WinWidth;
    this->sheight=props.WinHeight;

    this->bdrcolor=glm::vec4(1.0f);
    this->bkgcolor=glm::vec4(1.0f);

    // Setup Shader
    shader.ShaderSet("box");

    // Setup Boxes on GPU
    SetupBackground();
    SetupBorder();
};


//************************************
//        Set Colors Function
//************************************
/*
Change the color of the boxes border and background
*/
void DisplayBox::SetColors(glm::vec4 bkgcolor,glm::vec4 bdrcolor)
{
    this->bkgcolor=bkgcolor;
    this->bdrcolor=bdrcolor;
};

//************************************
//     Draw the box centered
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void DisplayBox::DrawBox()
{
    // Activate shader
    shader.Use();

    // Draw bg
    GLint bkgColorLoc = glGetUniformLocation(shader.Program, "incolor");
    glUniform4f(bkgColorLoc,bkgcolor.x,bkgcolor.y,bkgcolor.z,bkgcolor.w);

    glBindVertexArray(VAObg);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Draw border
    GLint bdrColorLoc = glGetUniformLocation(shader.Program, "incolor");
    glUniform4f(bdrColorLoc,bdrcolor.x,bdrcolor.y,bdrcolor.z,bdrcolor.w);

    glBindVertexArray(VAObd);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};

//************************************
//     Draw the Box at a Position
//************************************
/*
*/
void DisplayBox::DrawBoxPos(float x,float y)
{
    // Activate shader
    shader.Use();

    // Set the position uniforms
    glUniform1f(glGetUniformLocation(shader.Program, "xpos"),x);
    glUniform1f(glGetUniformLocation(shader.Program, "ypos"),y);

    // Set incolor uniform
    GLint bkgColorLoc = glGetUniformLocation(shader.Program, "incolor");
    glUniform4f(bkgColorLoc,bkgcolor.x,bkgcolor.y,bkgcolor.z,bkgcolor.w);

    // Draw bg
    glBindVertexArray(VAObg);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Activate shader
    shader.Use();

    // Set the position uniforms
    glUniform1f(glGetUniformLocation(shader.Program, "xpos"),x);
    glUniform1f(glGetUniformLocation(shader.Program, "ypos"),y);

    // Set incolor uniform
    GLint bdrColorLoc = glGetUniformLocation(shader.Program, "incolor");
    glUniform4f(bdrColorLoc,bdrcolor.x,bdrcolor.y,bdrcolor.z,bdrcolor.w);

    // Draw border
    glBindVertexArray(VAObd);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void DisplayBox::Cleanup()
{
    glDeleteVertexArrays(1, &VAObg);
    glDeleteBuffers(1, &VBObg);
    glDeleteBuffers(1, &EBObg);

    glDeleteVertexArrays(1, &VAObd);
    glDeleteBuffers(1, &VBObd);
    glDeleteBuffers(1, &EBObd);

    shader.Cleanup();
};

//************************************
//         Setup the Background
//************************************
/*

*/
void DisplayBox::SetupBackground ()
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    float bx=scalex;
    float by=scaley;

    //std::cout << "bx: " << bx << " by: " << by <<  std::endl;

    GLfloat vertices[] =
    {
         // Positions              // Texture Coords
         bx,  by, // Top Right
         bx, -by, // Bottom Right
        -bx, -by, // Bottom Left
        -bx,  by  // Top Left
    };

    GLuint indices[] =    // Note that we start from 0!
    {
        0, 2, 1, // First Triangle
        0, 3, 2  // Second Triangle
    };

    glGenVertexArrays(1, &VAObg);
    glGenBuffers(1, &VBObg);
    glGenBuffers(1, &EBObg);

    glBindVertexArray(VAObg);

    glBindBuffer(GL_ARRAY_BUFFER, VBObg);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObg);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
};

//************************************
//         Setup the Border
//************************************
/*

*/
void DisplayBox::SetupBorder ()
{
    float aspc=sheight/swidth;
    //std::cout << "aspc: " << aspc << std::endl;

    float xoe=scalex+bwidth*aspc;
    float yoe=scaley+bwidth;
    float xie=scalex;
    float yie=scaley;

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        // Positions
         xie,  yie, // x0
         xie, -yie, // x1
        -xie, -yie, // x2
        -xie,  yie, // x3
         xoe,  yoe, // x4
         xoe,  yie, // x5
         xoe, -yie, // x6
         xoe, -yoe, // x7
        -xoe, -yoe, // x8
        -xoe, -yie, // x9
        -xoe,  yie, // x10
        -xoe,  yoe  // x11
    };
    GLuint indices[] =    // Note that we start from 0!
    {
         0, 6, 5,
         0, 1, 6,
         9, 7, 6,
         9, 8, 7,
        10, 2, 3,
        10, 9, 2,
        11, 5, 4,
        11, 10, 5
    };

    glGenVertexArrays(1, &VAObd);
    glGenBuffers(1, &VBObd);
    glGenBuffers(1, &EBObd);

    glBindVertexArray(VAObd);

    glBindBuffer(GL_ARRAY_BUFFER, VBObd);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObd);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
};
