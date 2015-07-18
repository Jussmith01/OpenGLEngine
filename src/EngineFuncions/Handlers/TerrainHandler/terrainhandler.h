#ifndef TERRAINHANDLER_C
#define TERRAINHANDLER_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include "../ModelHandler/base_classes.h"
#include "../../Loaders/shader.h"
#include "../../Loaders/texture.h"
#include "../../Tools/tools.hpp"
#include "../../Tools/ogltools.hpp"
#include "../../Tools/glmtools.hpp"

class TerrainHandler
{
    /* These will hold the filenames of the Textures and Shaders */
    std::string TextureFiles[4];
    std::string ShaderFiles;

    /* These vectors hold the mesh data for the terrain */
    std::vector< std::vector<Vertex> > meshVerts;
    std::vector< std::vector<GLuint> > idxs;

    /*  A vector of render data buffers  */
    std::vector< ogltools::BufferHandler > buffers;

    /* Variables for determining if data is set on the GPU */
    bool GPUDataSet;
    bool GPUTexSet;
    bool GPUShdrSet;

    /* Mesh center positions */
    std::vector< glm::vec2 > positions;

    /* Texture handlers */
    Texture texture[4];

    /* Shader Handler */
    Shader shader;

    /* Materials Data Storage */
    Material materials;

    /* Height used to determine landscape texturing */
    glm::vec4 relativeHeight;

    /*---------------------------
    Internal Class Functionality
    ---------------------------*/
    //Main Draw
    void Draw();

public:
    /*---------------------------
         Public Constructors
    ---------------------------*/
    TerrainHandler();

    /*---------------------------
      Data Accessing Functions
    ---------------------------*/
        // Index Access Provider
    std::vector< std::vector<Vertex> >& AccessMeshVerts() {return meshVerts;}
    // Index Access Provider
    std::vector< std::vector<GLuint> >& AccessIdxs() {return idxs;}
    // Positions Access Provider
    std::vector< glm::vec2 >& AccessPositions() {return positions;}
    // Shader Access Provider
    Shader& AccessShader() {return shader;}
    // Set the GPU vertex data bool
    void SetGPUData() {GPUDataSet=!GPUDataSet;};
    // Set the GPU texture data bool
    void SetGPUTex() {GPUTexSet=!GPUTexSet;};
    // Set the GPU shader data bool
    void SetGPUShdr() {GPUShdrSet=!GPUShdrSet;};
    // Get the GPU vertex data bool
    bool GetGPUData() {return GPUDataSet;};
    // Get the GPU texture data bool
    bool GetGPUTex() {return GPUTexSet;};
    // Get the GPU shader data bool
    bool GetGPUShdr() {return GPUShdrSet;};
    // Get the relative height data
    glm::vec4* AccessRelHeight() {return &relativeHeight;};
    // Get the relative height data
    glm::vec4 GetRelHeight() {return relativeHeight;};
    // Get the materials data
    Material GetMaterial() {return materials;};
    // Set the shader filename
    void SetShaderFile(std::string file) {ShaderFiles=file;};
    // Set the texture filenames
    void SetTexFiles(std::string f1,std::string f2,std::string f3,std::string f4);
    // Set the material values
    void SetupMaterials(glm::vec3 Ka,glm::vec3 Kd,glm::vec3 Ks,float shininess);

    /*---------------------------
          Class Functionality
    ---------------------------*/
    // Export the terrain
    /*
    The exported file is used by the game
    engine and cannot be reloaded into the
    editor. If you want to re-edit terrian
    later use the save function from
    the TerrainGeneration class.
    */
    void ExportTerrain(std::string filename);
    // Draw call with load check
    void DrawCall();
    // Cleanup the class
    void Cleanup();
    // Get Memory Use
    long int GetGPUMemoryReqs();
    // Return the number of verts in all meshes
    int GetNumberVerts();

    /*---------------------------
     Loading/Unloading Functions
    ---------------------------*/
    // This sets the terrain meshes on the GPU
    void SetTerrainOnGPU();
    // Sets up the meshes on the GPU
    void setupMeshes();
    // Fully load landscape textures to the CPU and GPU
    void SetTextures();
    // Unset the Textures on the GPU
    void UnsetTextures();
    // Load the landscape shaders to the GPU
    void SetShader();
    // Remove the shader from the GPU
    void UnsetShader();
    // Set Materials
    void setMaterialUniform();
    // Set Relative Height Parameters
    void SetRelativeHeightUniform();
};

#endif
