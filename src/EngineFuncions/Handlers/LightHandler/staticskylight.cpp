#include "staticskylight.h"

//*********************************
//          Initialization
//*********************************
/*
Class initializer
*/
void StaticSkyLighting::InitStatic(glm::vec3 Dir,glm::vec3 Color,float Intensity)
{
    if (!setup)
    {
        setup=true;

        this->lightDir=Dir;
        this->lightColor=Color;
        this->lightIntensity=Intensity;

        SetStaticUBO();
        SetStaticUBOData();
    } else {
        std::cout << "Error: StaticSkyLighting class already initialized!\n";
    }
};

//************************************
// Initialize the skylightData UBO
//************************************
/*
This class sets the skylightData UBO so that
shaders can access the new data on the GPU.
*/
void StaticSkyLighting::SetStaticUBO()
{
    glGenBuffers(1, &skylightDataUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, skylightDataUBO);
    glBufferData(GL_UNIFORM_BUFFER, 48, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 1);

    glBindBufferRange(GL_UNIFORM_BUFFER, 1, skylightDataUBO, 0, 48);
};

//************************************
//    Update the cameraData UBO
//************************************
/*
Updates the data in the previously set
cameraData UBO.

A shader, with the cameraData UBO must
register with the

RegisterShaderWithCameraDataUBO(Shader &shader)

function of this class to obtain the
data from this UBO.
*/
void StaticSkyLighting::SetStaticUBOData()
{

    // Bind the UBO Buffer
    glBindBuffer(GL_UNIFORM_BUFFER, skylightDataUBO);

    // Set the intensity in the UBO
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16, &lightIntensity);

    // Set the initial color in the UBO
    glBufferSubData(GL_UNIFORM_BUFFER, 16, 16, glm::value_ptr(lightColor));

    // Set the initial direction in the UBO
    glBufferSubData(GL_UNIFORM_BUFFER, 32, 16, glm::value_ptr(lightDir));

    // Unbind the UBO Buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 1);
};

//*********************************
//   Register Shader for UBO
//*********************************
/*
This functions allows you to register a shader
with the class so that it can obtain the UBO
sky lighting Data. All shaders that will use
this UBO MUST register with this class!
*/
void StaticSkyLighting::RegisterShader(Shader &shader)
{
    //do this for all active shaders
    GLuint P = shader.Program;
    GLuint uniformBlockIndex = glGetUniformBlockIndex(P, "skylightData");
    glUniformBlockBinding(P, uniformBlockIndex, 1);
};

//*********************************
//      Cleanup the Class
//*********************************
/*
The only cleanup required is the deletion of
the skylightData UBO from the GPU.
*/
void StaticSkyLighting::Cleanup()
{
    glDeleteBuffers(1,&skylightDataUBO);
};
