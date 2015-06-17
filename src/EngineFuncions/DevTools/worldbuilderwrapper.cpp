#include "worldbuilderwrapper.h"

//******************************************//
//   World Builder Wrapper Initialization   //
//******************************************//
/*
Builder Initializer
*/
void WorldBuilderWrapper::Init (Engine *game)
{
    // Initialize Camera Class
    camera.Init(4.0,12.0,0.0,0.0,game->props.WinWidth,game->props.WinHeight);

    // Initialize the overhead "sky" lighting
    skylight.InitStatic(glm::vec3(0.0f,-0.5f,-0.5f),glm::vec3(1.0f),1.0f);

    // Initialize screen writing class
    text.Setup("../Fonts/FreeSans.ttf",game->props.WinWidth,game->props.WinHeight,game->props.FontSize);

    //*****************************
    //TESTING STUFF GOES BELOW HERE
    //*****************************
    shader.ShaderSet("basicobject");

    models.push_back((std::string)"asteroid1.obj");
    models.back().LoadModelDataToCPU();
    models.back().LoadModelToGPU();

    texture.Setup("asteroid1.png","textures.textureMap");
    texture.LoadTextureDataToCPU();
    texture.LoadTextureDataToGPU();

    //*****************
    // Register Shader
    //*****************
    camera.RegisterShaderWithCameraDataUBO(shader);
    skylight.RegisterShader(shader);

    // Initialize Timing
    dt=glfwGetTime();
};

//******************************************//
//              Update Events               //
//******************************************//
/*
Updates all events for the wrapper class
*/
void WorldBuilderWrapper::UpdateEvents(InputStruct &input)
{
    camera.UpdateCamera(input);
};

//******************************************//
//             Update the Data              //
//******************************************//
/*
Updates all data for the wrapper class
*/
void WorldBuilderWrapper::UpdateData()
{
    frametime=(1000.0f*(glfwGetTime()-dt));
    dt=glfwGetTime();

};

//******************************************//
//      Draw everything for the Wrapper     //
//******************************************//
/*
Draw everything for the wrapper class
*/
void WorldBuilderWrapper::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Draw Asteroid Object
    shader.Use();
    GLint modelLoc = glGetUniformLocation(shader.Program, "modelMat");

    glm::mat4 model;
    model=glm::translate(model,glm::vec3(0.0f,0.0f,0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    for (int i=0; i<(int)models.back().Nmesh; ++i)
    {
        texture.useTexture(shader,0);
        models.back().mesh[i].setMaterial(shader.Program);
        models.back().mesh[i].Draw();
    }

    glDisable(GL_DEPTH_TEST);

    std::stringstream ss;
    ss << frametime << " f/ms";
    text.RenderTextCentered(ss.str(),1,0.9f,1,0.85f,1.0f,glm::vec3(1.0f));
};

//******************************************//
//      World Builder Wrapper Cleanup       //
//******************************************//
/*
Cleaup the class
*/
void WorldBuilderWrapper::Cleanup()
{
    camera.Cleanup();
    text.Cleanup();
    texture.TextureCleanup();
    skylight.Cleanup();
    models.back().ClearMeshes();
};
