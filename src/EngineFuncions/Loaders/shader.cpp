#include "shader.h"
#include "../Tools/console.h"
#include "../Tools/tools.hpp"



//************************************
//      Shader Setup Function
//************************************
/*
Takes in a filename as a string, for example:

            ThisShader

then loads from bin/Shaders/ the files
ThisShader.fs and ThisShader.vs for fragment
shader and vertex shader respectively. The
function then compiles each shader, then
links them into a program where it stores
the program ID in the Program variable.
*/
void Shader::ShaderSet(std::string filename)
{
    //Produce Filename
    this->shdrname = filename;
    std::string dir = "../Shaders/";
    std::stringstream ssv;
    std::stringstream ssf;

    ssv << dir << filename << ".vs";
    ssf << dir << filename << ".fs";

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        // Open files
        std::ifstream vShaderFile(ssv.str().c_str());
        std::ifstream fShaderFile(ssf.str().c_str());
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into GLchar array
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::exception e)
    {
        Console::cPrint("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
        //std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    //**************************************************
    // 2. Compile shaders
    //**************************************************
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    //**************************************************
    // 		    Vertex Shader
    //**************************************************
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    };
    //**************************************************
    // 		  Fragment Shader
    //**************************************************
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    };

    //**************************************************
    // 		  Shader Program
    //**************************************************
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    //std::cout << "Shader " << filename <<  " compiled and ready." << std::endl;
};

//************************************
//      Use the Shader Function
//************************************
/*
Used during drawing during an opengl loop.
This function sets the glUseProgram() function
for the compiled Program.
*/
void Shader::Use()
{
    /*if(!glIsProgram(this->Program))
    {
        std::cout << "ERROR: Shader " << shdrname << " not found.\n";
        exit(1);
    }*/
    glUseProgram(this->Program);
};

//************************************
//    Delete the Shader Program
//************************************
/*
Used when removing a classes that will
effectively delete this shader program.
*/
void Shader::Cleanup()
{
    glDeleteProgram(this->Program);
};
