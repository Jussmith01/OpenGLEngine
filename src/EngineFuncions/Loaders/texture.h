#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include <SOIL/SOIL.h>
#include "shader.h"
#include "../Tools/console.h"

//**************************
//Vertex Object Loader Class
//**************************
class Texture
{
public:
    // Our program variables
    GLuint TextureID;
    std::string uniform;
    std::string filename;

    //Check if loaded to GPU
    bool CPUload;
    bool GPUload;


    //Uniform Types
    int w,h;
    unsigned char *image;

    // Constructor reads and builds our shader -- First NDiff filenames are diffuse, last NSpec
    Texture ()
    {
        GPUload=false;
        CPUload=false;
    };

    Texture (std::string file,std::string uniform)
    {
        Setup(file,uniform);
        GPUload=false;
        CPUload=false;
    };

    ~Texture () {}

    void Setup(std::string file,std::string uniform);

    void LoadTextureDataToCPU();

    void LoadTextureDataToGPU();

    void TextureDeleteFromGPU();

    void TextureDeleteFromCPU();

    void TextureCleanup();

    void useTexture (Shader &shader, GLint texIdx);
};

#endif // TEXTURELOADER_H
