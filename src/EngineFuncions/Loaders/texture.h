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
    // Our program variables
    GLuint TextureID;
    std::string uniform;
    std::string filename;

    //Check if loaded to GPU
    bool CPUload;
    bool GPUload;


    //Image Variables
    int w,h;
    unsigned char *image;
    double memsize;

    public:
    // Constructor reads and builds our shader -- First NDiff filenames are diffuse, last NSpec
    Texture ()
    {
        GPUload=false;
        CPUload=false;
        memsize=0;
    };

    Texture (std::string file,std::string uniform) : Texture()
    {
        Setup(file,uniform);
    };

    ~Texture () {}

    void Setup(std::string file,std::string uniform);

    void LoadTextureDataToCPU();

    void LoadTextureDataToGPU();

    void TextureDeleteFromGPU();

    void TextureDeleteFromCPU();

    void TextureCleanup();

    void useTexture (Shader &shader, GLint texIdx);

    void ProduceMemoryUsage();

    double MemSize();
};

#endif // TEXTURELOADER_H
