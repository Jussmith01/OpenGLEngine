#ifndef STATICSKYLIGHTING_C
#define STATICSKYLIGHTING_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include "../../Loaders/shader.h"

class StaticSkyLighting
{
    bool setup;

    // Light Data
    glm::vec3 lightDir;
    glm::vec3 lightColor;
    GLfloat lightIntensity;

    // UBO ID
    GLuint skylightDataUBO;
    /* !!!!THIS UBO IS ALWAYS BOUND TO

        GL_UNIFORM_BUFFER index 1
    */

public:

    //Constructor and Destructor
    StaticSkyLighting() {setup=false;};
    ~StaticSkyLighting() {};

    /*__________________________________*/
    //------------------------------------
    //  Static Skylighting Functionality
    //------------------------------------

    //*********************************
    //       Initialize Static
    //*********************************
    void InitStatic(glm::vec3 initialDir,glm::vec3 initialColor,float intensity);

    //************************************
    // Initialize the skylightData UBO
    //************************************
    void SetStaticUBO();

    //************************************
    //    Update the cameraData UBO
    //************************************
    void SetStaticUBOData ();

    //*********************************
    //   Register Shader for UBO
    //*********************************
    void RegisterShader(Shader &shader);

    //*********************************
    //      Cleanup the Class
    //*********************************
    void Cleanup();
};

#endif
