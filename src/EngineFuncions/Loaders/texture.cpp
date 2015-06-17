#include "texture.h"


//************************************
//      Shader Setup Function
//************************************
/*

*/
void Texture::Setup(std::string file,std::string uniform)
{
    std::string dir = "../Data/Textures/";
    this->filename=dir+file;
    this->uniform=uniform;
};

void Texture::LoadTextureDataToCPU()
{
    if(!CPUload)
    {
        CPUload=true;

        //THIS NEEDS TO BE THREAD SAFE FOR LINUX, NOT SURE ABOUT OTHER OSs
        image=SOIL_load_image(filename.c_str(), &w, &h, 0, SOIL_LOAD_RGBA);
        if (image == NULL)
        {
            std::cout << "ERROR: Texture failed to load: " << filename.c_str() << "\n";
        }
    }
};

void Texture::LoadTextureDataToGPU()
{
    //Texture 1 Define
    if(!GPUload)
    {
        GPUload=true;

        glGenTextures(1, &TextureID);

        glBindTexture(GL_TEXTURE_2D, this->TextureID);
        //Load Texture 1
        //int w, h;
        //else {cout << "SUCCESS: Texture loaded: " << filename.c_str() << "\n";}
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData[Num].c_str());
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "Binding Texture: " << filename << " to Address: " << this->TextureID << "\n";
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

void Texture::TextureDeleteFromGPU ()
{
    std::cout << "Clearing GPU data...\n";
    GPUload=false;
    glDeleteTextures(1,&TextureID);
};

void Texture::TextureDeleteFromCPU ()
{
    std::cout << "Clearing CPU data...\n";
    CPUload=false;
    delete [] image;
};

void Texture::TextureCleanup ()
{
    std::cout << "Clearing Texture Data...\n";

    if (GPUload)
    {
        GPUload=false;
        TextureDeleteFromGPU();
    }

    if (CPUload)
    {
        CPUload=false;
        TextureDeleteFromCPU();
    }
};

void Texture::useTexture (Shader &shader, GLint texIdx)
{
    glActiveTexture(GL_TEXTURE0 + texIdx);
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    glUniform1i(glGetUniformLocation(shader.Program, uniform.c_str()), texIdx);
};
