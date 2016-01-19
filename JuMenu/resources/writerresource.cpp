#include "writerresource.h"

//************************************
//   Screen writer Setup Function
//************************************
/*
Takes in a font dir/filename as a string, for
example:

                ThisFont

then loads the font into a VBO on the GPU.
Also initializes, all freetype functions and
variables for properly setting up the glyphs.
Also loads the font shader 'text'. All of
these things are stored in the class for
easily displaying text to screen via functions.
The swidth, sheight parameters are the screen
size and the fontresize is a parameter to
resize font. This is meant to modify font
size based on DPI, however this function does
not do this for you.
*/
void ScreenWriter::Setup(std::string font,float swidth,float sheight,float fontresize)
{
    this->swidth=swidth;
    this->sheight=sheight;
    this->fontresize=fontresize;
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    //Set the font size
    FT_Set_Pixel_Sizes(face, 0, 64);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    //Load Font INformationsudo aptitude --purge reinstall linux-sound-base alsa-base alsa-utils linux-image-`uname -r` linux-ubuntu-modules-`uname -r` libasound2
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,GL_RED,GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character =
        {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    //Free FT Stuff
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //Load the shaders
    this->textshader.ShaderSet("text");

    //Set the projection matrix
    proj = glm::ortho(0.0f, swidth, 0.0f, sheight);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};

//************************************
//   Render Text at an x,y Position
//************************************
/*
Renders the text supplied in 'text' parameter
to the screen. x,y are horizontal and vertical
positions normalized (0 to 1) from the bottom
left corner of the screen to the top right. The
Text is bottom left aligned with the supplied
position. So (1,1) would put the text off the
screen. Scale also scales the text. While
*/
void ScreenWriter::RenderTextPos(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    glDepthFunc(GL_ALWAYS);
    // Activate corresponding render state
    textshader.Use();

    //Load Uniforms
    glUniform3f(glGetUniformLocation(textshader.Program, "textColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(textshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    //Set Textures
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    scale *= fontresize;

    x = x * swidth;
    y = y * sheight;

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthFunc(GL_LESS);
};

//*****************************************
//Render Text at an x,y Position - Centered
//*****************************************
/*
Renders the text supplied in 'text' parameter
to the screen. x,y are horizontal and vertical
positions normalized (-1 to 1) from the center
of the screen based on the classes input screen
sizes. The x,y coords are aligned with the
center of the text. So a coord of (0,0) will
put the text in the exact center of the screen.

Centering can individually be turned off via
the input parameters xset and yset. An
xset=true centers the text horizontally.
Similar for the yset parameter.
*/
void ScreenWriter::RenderTextCentered(std::string text
                                      ,bool xset, GLfloat xcorr
                                      ,bool yset, GLfloat ycorr
                                      , GLfloat scale, glm::vec3 color)
{
    glDepthFunc(GL_ALWAYS);
    // Activate corresponding render state
    textshader.Use();

    //Load Uniforms
    glUniform3f(glGetUniformLocation(textshader.Program, "textColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(textshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    //Set Textures
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    scale *= fontresize;

    //Normalize coordinates to xcorr={-1,1} ycorr={-1,1}
    ycorr=-1*ycorr;
    tools::pixel_device_coordinate(xcorr,ycorr,swidth,sheight);

    // Some variables
    GLfloat length=0;
    GLfloat top=0,bottom=0;

    // Iterate through all characters
    std::string::const_iterator c1;
    for (c1 = text.begin(); c1 != text.end(); c1++)
    {
        Character ch = Characters[*c1];
        length += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)

        GLfloat ypos = (ch.Size.y - ch.Bearing.y) * scale;
        GLfloat h = ch.Size.y * scale;

        if (top<ypos+h)
        {
            top=ypos+h;
        }
        if (bottom>ypos)
        {
            bottom=ypos;
        }
    }

    GLfloat x=xcorr-(length/2.0f);
    GLfloat y=ycorr-((top-bottom)/2.0f);

    if (!xset)
    {
        x=xcorr;
    }
    if (!yset)
    {
        y=ycorr;
    }

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthFunc(GL_LESS);
}

//**********************************************************
//Render Text at an x,y Position - Centered - Left Justified
//**********************************************************
/*
Renders the text supplied in 'text' parameter
to the screen. x,y are horizontal and vertical
positions normalized (-1 to 1) from the center
of the screen based on the classes input screen
sizes. The x,y coords are aligned with the
center of the text. So a coord of (0,0) will
put the text in the exact center of the screen.

Centering can individually be turned off via
the input parameters xset and yset. An
xset=true centers the text horizontally.
Similar for the yset parameter.
*/
void ScreenWriter::RenderTextLeftJustified(std::string text
        ,GLfloat xcorr,GLfloat ycorr
        ,GLfloat scale,glm::vec3 color)
{
    glDepthFunc(GL_ALWAYS);
    // Activate corresponding render state
    textshader.Use();

    //Load Uniforms
    glUniform3f(glGetUniformLocation(textshader.Program, "textColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(textshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    //Set Textures
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    scale *= fontresize;

    //Normalize coordinates to xcorr={-1,1} ycorr={-1,1}
    ycorr=-1*ycorr;
    tools::pixel_device_coordinate(xcorr,ycorr,swidth,sheight);

    // Some variables
    GLfloat top=0,bottom=0;

    // Iterate through all characters
    std::string::const_iterator c1;
    for (c1 = text.begin(); c1 != text.end(); c1++)
    {
        Character ch = Characters[*c1];

        GLfloat ypos = (ch.Size.y - ch.Bearing.y) * scale;
        GLfloat h = ch.Size.y * scale;

        if (top<ypos+h)
        {
            top=ypos+h;
        }
        if (bottom>ypos)
        {
            bottom=ypos;
        }
    }

    GLfloat x=xcorr;
    GLfloat y=ycorr-((top-bottom)/2.0f);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthFunc(GL_LESS);
}

//**********************************************************
//Render Text at an x,y Position - Centered - Right Justified
//**********************************************************
/*
Renders the text supplied in 'text' parameter
to the screen. x,y are horizontal and vertical
positions normalized (-1 to 1) from the center
of the screen based on the classes input screen
sizes. The x,y coords are aligned with the
center of the text. So a coord of (0,0) will
put the text in the exact center of the screen.

Centering can individually be turned off via
the input parameters xset and yset. An
xset=true centers the text horizontally.
Similar for the yset parameter.
*/
void ScreenWriter::RenderTextRightJustified(std::string text
        ,GLfloat xcorr,GLfloat ycorr
        ,GLfloat scale,glm::vec3 color)
{
    glDepthFunc(GL_ALWAYS);
    // Activate corresponding render state
    textshader.Use();

    //Load Uniforms
    glUniform3f(glGetUniformLocation(textshader.Program, "textColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(textshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    //Set Textures
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    scale *= fontresize;

    //Normalize coordinates to xcorr={-1,1} ycorr={-1,1}
    ycorr=-1*ycorr;
    tools::pixel_device_coordinate(xcorr,ycorr,swidth,sheight);

    // Some variables
    GLfloat top=0,bottom=0;

    GLfloat strleng=0;

    // Iterate through all characters
    std::string::const_iterator c1;
    for (c1 = text.begin(); c1 != text.end(); c1++)
    {
        Character ch = Characters[*c1];

        strleng+= (ch.Advance >> 6) * scale;

        GLfloat ypos = (ch.Size.y - ch.Bearing.y) * scale;
        GLfloat h = ch.Size.y * scale;

        if (top<ypos+h)
        {
            top=ypos+h;
        }
        if (bottom>ypos)
        {
            bottom=ypos;
        }
    }

    GLfloat x=xcorr-strleng;
    GLfloat y=ycorr-((top-bottom)/2.0f);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthFunc(GL_LESS);
}

//*****************************************
//          Render Blocked Text
//*****************************************
/*

*/
void ScreenWriter::RenderTextBlocked(std::string text
                                     ,GLfloat xcorr,GLfloat ycorr
                                     ,GLfloat scale,GLfloat linespace
                                     ,GLfloat blocklength,glm::vec3 color)
{
    glDepthFunc(GL_ALWAYS);
    // Activate corresponding render state
    textshader.Use();

    //Load Uniforms
    glUniform3f(glGetUniformLocation(textshader.Program, "textColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(textshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    //Set Textures
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    scale *= fontresize;

    //Normalize coordinates to xcorr={-1,1} ycorr={-1,1}
    ycorr=-1*ycorr;
    tools::pixel_device_coordinate(xcorr,ycorr,swidth,sheight);

    // Some variables
    //GLfloat length=0;
    GLfloat top=0,bottom=0;

    // Iterate through all characters
    std::string::const_iterator c1;
    for (c1 = text.begin(); c1 != text.end(); c1++)
    {
        Character ch = Characters[*c1];

        GLfloat ypos = (ch.Size.y - ch.Bearing.y) * scale;
        GLfloat h = ch.Size.y * scale;

        if (top<ypos+h)
        {
            top=ypos+h;
        }
        if (bottom>ypos)
        {
            bottom=ypos;
        }
    }

    GLfloat x=xcorr;
    GLfloat y=ycorr;
    //GLfloat y=ycorr-(top-bottom);

    // Iterate through all characters
    bool endline=false;
    int cntr=0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        //std::cout << "Text: " << text[cntr] << " TextID: " << ch.TextureID << std::endl;
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)

        if ((x-xcorr)>blocklength*swidth)
        {
            endline = true;
        }

        std::stringstream spcchk;
        spcchk << text[cntr];
        if (endline && spcchk.str().compare(" ")==0)
        {
            x=xcorr;
            y-=2.5*linespace*scale;
            endline=false;
        }
        ++cntr;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthFunc(GL_LESS);
}

//*****************************************
//     Cleanup The Class for Deletion
//*****************************************
/*
Free up the VAO,VBO and cleanup the text
shader.
*/
void ScreenWriter::Cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    textshader.Cleanup();
};
