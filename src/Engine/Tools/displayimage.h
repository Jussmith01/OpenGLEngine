#ifndef DISPIMAGE_H
#define DISPIMAGE_H

#include <SOIL.h>
#include "../Loaders/shader.h"
#include "tools.hpp"

class ImageDisplay {

    /* Class State */
    bool is_ready;

    /* GL Vars */
    GLuint TextureID;
    Shader shader;
    GLuint VBO, VAO, EBO;

    /* Image Properties */
    float swidth,sheight;
    float aspect;
    float lw,lh;
    int w,h;

public:

    /* Default Constructor */
    ImageDisplay () : is_ready(false) {};

    /* Inititalizer */
    void Init (std::string file,float xscale,float yscale,float swidth,float sheight) {

        this->swidth=swidth;
        this->sheight=sheight;
        this->aspect=sheight/swidth;

        //Load Image to the GPU
        std::stringstream ss;
        ss << "../Data/Images/" << file;
        LoadImage(ss.str().c_str());

        //Load and setup the shader
        shader.ShaderSet("image");

        //Setup the display polygon
        SetupPolygon(xscale,yscale);

        //
        is_ready = true;
    };

    /* Load Images */
    void LoadImage (std::string file) {
        unsigned char *image = SOIL_load_image(file.c_str(),&w,&h,0,SOIL_LOAD_RGBA);
        if (image == NULL) {
            std::cout << "ERROR: Image failed to load: " << file.c_str() << "\n";
        }

        glGenTextures(1, &this->TextureID);
        glBindTexture(GL_TEXTURE_2D, this->TextureID);

        //Load Texture 1
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image);
        SOIL_free_image_data(image);

        glGenerateMipmap(GL_TEXTURE_2D);

        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    };

    void SetupPolygon (float xscale,float yscale) {
        float monitorcorrection=(h/(float)w)*(1/aspect);
        float yp = yscale*monitorcorrection;

        //Store the border of the polygon
        lw=xscale;
        lh=yp;

        // Set up vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
            // Positions              // Texture Coords
            xscale,  yp,   1.0f, 1.0f, // Top Right
            xscale, -yp,   1.0f, 0.0f, // Bottom Right
            -xscale, -yp,   0.0f, 0.0f, // Bottom Left
            -xscale,  yp,   0.0f, 1.0f  // Top Left
        };
        GLuint indices[] = {  // Note that we start from 0!
            0, 3, 1, // First Triangle
            1, 3, 2  // Second Triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // TexCoord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); // Unbind VAO
    };

    void DrawImage () {
        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glUniform1i(glGetUniformLocation(shader.Program, "Texture"), 0);

        // Activate shader
        shader.Use();

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    void DrawImagePos (float x,float y) {
        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glUniform1i(glGetUniformLocation(shader.Program, "Texture"), 0);

        // Activate shader
        shader.Use();

        //Set Position
        //std::cout << "LOCTESTx: " << xconv << " LOCTESTy: " << yconv << std::endl;
        glUniform1f(glGetUniformLocation(shader.Program, "xpos"),x);
        glUniform1f(glGetUniformLocation(shader.Program, "ypos"),y);

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    void DrawImagePosScale (float x,float y,float scale) {
        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glUniform1i(glGetUniformLocation(shader.Program, "Texture"), 0);

        // Activate shader
        shader.Use();

        //Set Position
        //std::cout << "LOCTESTx: " << xconv << " LOCTESTy: " << yconv << std::endl;
        glUniform1f(glGetUniformLocation(shader.Program, "xpos"),x);
        glUniform1f(glGetUniformLocation(shader.Program, "ypos"),y);

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    void Cleanup () {
        is_ready = false;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1,&TextureID);
        shader.Cleanup();
    }

    float GetImageHalfLength() {
        return lw;
    };

    float GetImageHalfHeight() {
        return lh;
    };
};

#endif
