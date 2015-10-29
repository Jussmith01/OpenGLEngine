#define GLEW_STATIC
#ifndef SCREENWRITER_C
#define SCREENWRITER_C

#include <map>
#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "tools.hpp"
#include "../Loaders/shader.h"

//FREETYPE LIBRARIES
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

//*****************************************//
//	     Screen Writer Class	   //
//*****************************************//
class ScreenWriter
{
    Shader textshader;
    glm::mat4 proj;
    GLuint VAO, VBO;
    float swidth,sheight;
    float fontresize;

    std::vector< glm::vec4 > vertexData;

    std::map<GLchar, Character> Characters;

public:
    // Default constructor
    ScreenWriter() {};

    // Setup constructor
    ScreenWriter(std::string font,float swidth,float sheight,float fontresize) {
        Setup(font,swidth,sheight,fontresize);
    };

    // Setup the class
    void Setup(std::string font,float swidth,float sheight,float fontresize);

    // Renders text to the screen at a given x,y position in pixels -- CHANGE THIS TO A NORMALIZED COORDINTATE SYSTEM
    void RenderTextPos(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    // Renders text to the center of the screen with
    void RenderTextCentered(std::string text,bool xset, GLfloat xcorr,bool yset,GLfloat ycorr, GLfloat scale, glm::vec3 color);

    void RenderTextLeftJustified(std::string text
                                 ,GLfloat xcorr,GLfloat ycorr
                                 ,GLfloat scale,glm::vec3 color);

    void RenderTextRightJustified(std::string text
                                 ,GLfloat xcorr,GLfloat ycorr
                                 ,GLfloat scale,glm::vec3 color);

    void RenderTextBlocked(std::string text,GLfloat xcorr,GLfloat ycorr
                           ,GLfloat scale,GLfloat linespace
                           ,GLfloat blocklength,glm::vec3 color);

    // Cleanup
    void Cleanup(void);
};


#endif
