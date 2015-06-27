#ifndef TERRAINGENERATOR_C
#define TERRAINGENERATOR_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include "../../Loaders/shader.h"
#include "../../Loaders/texture.h"
#include "../worldbuildertools/randlib.h"
#include "../worldbuildertools/boxclass.h"
#include "../../Handlers/ModelHandler/base_classes.h"
#include "../../Tools/rtscamera.h"
#include "../../Tools/tools.hpp"
#include "../../Tools/ogltools.hpp"
#include "../../Tools/glmtools.hpp"
#include "../../Tools/micro_timer.h"
#include "../../Tools/ToolBoxs/terraincreationtoolbox.h"
#include "../../Tools/ToolBoxs/terrainmodificationtoolbox.h"
#include "../../Tools/ToolBoxs/materialmodificationtoolbox.h"

//******************************************//
//            World Builder Class           //
//******************************************//
//This Class controls all aspects of world
//the world builder functionality
class TerrainGeneration
{

    //******************************
    //	Terrain Data
    //******************************
    //************
    // Saved Data
    //************
    std::vector< std::vector<int> > HeightData;

    int terrainSize; // Size of the terrain
    int NSmooth; // Number of smoothing cycles to run
    int subdiv; // Number of meshes to subdivide into
    float heightMult;

    glm::vec4 relativeHeight;
    glm::ivec3 heightVariation;

    Material materials;

    /*These will hold the filenames of the Textures and Shaders*/
    std::string TextureFiles[4];
    std::string ShaderFiles;

    //************
    // Built Data
    //************
    std::vector< Vertex > verts;

    std::vector< std::vector<Vertex> > meshVerts;
    std::vector< std::vector<GLuint> > idxs;
    //std::vector< glm::vec2 > positions; // mesh positions

    /*  Render data  */
    std::vector< ogltools::BufferHandler > buffers;

    // Class Variables
    bool GPUDataSet;
    bool GPUTexSet;
    bool GPUShdrSet;

    int Nsd;
    float lowShift;

    float sizeScale;

    // Class functions
    Texture texture[4];
    microTimer generalTimer;

public:
    // Public for use with register functions
    Shader shader;

    /*MOVE BACK TO PRIVATE WHEN DONE*/
    std::vector< glm::vec2 > positions; // mesh positions

    TerrainGeneration()
    {
        SetDefaults();
    };

    ~TerrainGeneration () {};

    //*********************************************
    //       Sets up the Shader and Textures
    //*********************************************
    void SetTextures()
    {
        std::cout << "Setting Textures on GPU...\n";

        if (!GPUTexSet)
        {
            // Setup Textures
            texture[0].Setup(tools::appendStrings("landscape/",TextureFiles[0]),"textures.lowlandMap");
            texture[1].Setup(tools::appendStrings("landscape/",TextureFiles[1]),"textures.mediumlandMap");
            texture[2].Setup(tools::appendStrings("landscape/",TextureFiles[2]),"textures.highlandMap");
            texture[3].Setup(tools::appendStrings("landscape/",TextureFiles[3]),"textures.cliffMap");

            // Load Textures to CPU and GPU
            for (auto&& i : texture)
            {
                i.LoadTextureDataToCPU();
                i.LoadTextureDataToGPU();
            }

            GPUTexSet=true;
        }
    };

    //*********************************************
    //       Sets up the Shader and Textures
    //*********************************************
    void SetShader()
    {
        std::cout << "Setting Shader on GPU...\n";

        if (!GPUShdrSet)
        {
            shader.ShaderSet(ShaderFiles);
            GPUShdrSet=true;
        }
    };

    //*********************************************
    //       Unset the Textures
    //*********************************************
    void UnsetTextures()
    {
        std::cout << "Unsetting Textures on GPU...\n";

        if (GPUTexSet)
        {
            for (auto&& i : texture)
                i.TextureCleanup();
        }

        GPUTexSet=false;
    };

    //*********************************************
    //       Unset the Shader
    //*********************************************
    void UnsetShader()
    {
        std::cout << "Unsetting Shader on GPU...\n";

        if (GPUShdrSet)
        {
            shader.Cleanup();
        }

        GPUShdrSet=false;
    };

    //*********************************************
    //         Generates Terrain on CPU
    //*********************************************
    void GenerateTerrain()
    {
        std::cout << "Generating Terrain Data...\n";
        GenerateTerrainData(this->terrainSize);

        std::cout << "Setting up Verts...\n";
        SetupVerts();
    };

    //*********************************************
    //         Move Terrain Data to GPU
    //*********************************************
    void SetInitalTerrainOnGPU()
    {
        if (GPUDataSet)
        {
            while (!buffers.empty())
            {
                buffers.back().ClearBuffers();
                buffers.pop_back();
            }

            setupMeshRegular();
        }
        else
        {
            setupMeshRegular();
        }

        GPUDataSet=true;
    };

    //*********************************************
    //             Draw the Terrain
    //*********************************************
    void DrawCall()
    {
        if (GPUDataSet)
        {
            Draw();
        }
    };

    //*********************************************
    //              Cleanup Class
    //*********************************************
    void Cleanup()
    {
        if (GPUDataSet)
        {
            while (!buffers.empty())
            {
                buffers.back().ClearBuffers();
                buffers.pop_back();
            }

            GPUDataSet=false;
        }

        UnsetTextures();
        UnsetShader();
    };

    //****************
    // Setup Defaults
    //****************
    void SetDefaults ()
    {
        GPUDataSet=false;
        GPUTexSet=false;
        GPUShdrSet=false;

        ShaderFiles="terrain";

        TextureFiles[0]="lowland.png";
        TextureFiles[1]="grass1.png";
        TextureFiles[2]="rocks.png";
        TextureFiles[3]="cliff1.png";

        sizeScale=10.0f;

        std::cout << "Setting up Materials...\n";
        glm::vec3 Ka(0.1f);
        glm::vec3 Kd(0.9f);
        glm::vec3 Ks(0.1f);
        float shininess=64.0;
        SetupMaterials(Ka,Kd,Ks,shininess);

        std::cout << "Setting up Parameters...\n";
        SetupTerrainCreationParameters(5,2,glm::ivec3(500,150,150));
        SetupTerrainModifyParameters(0.3f,glm::vec3(0.1f,0.8f,0.05f));

        subdiv=2;
    };

    //*****************
    // Setup Materials
    //*****************
    /*
    Ka = Ambient light vector (glm::vec3)
    Kd = Diffuse light vector (glm::vec3)
    Ks = Specular light vector (glm::vec3)
    shininess = Specular shine factor (float)
    */
    void SetupMaterials(glm::vec3 Ka,glm::vec3 Kd,glm::vec3 Ks,float shininess);

    //************************
    // Set Terrain Parameters
    //************************
    /*
    terrainSize: the width and height in number of verticies of the terrain (int)
    heightMult: The height multiplier of the terrain. (float)
    HeightVariation: (glm::ivec3)
            .x=number between 0 and 1000, corner starting heights.
            .y=Inital random drop of heights (shrinks by generation)
            .z=Inital random increases of heights (shrinks by generation)
    relheight: (glm::vec3)
            .x=switching height of lowland to midland textures
            .y=switching height of midland to highland textures
            .z=Percent of switching
    */
    void SetupTerrainCreationParameters(int terrainSize,int NSmooth,glm::ivec3 heightVariation);
    void SetupTerrainModifyParameters(float heightMult,glm::vec3 relheight);

    //**************************
    // Recalc From Height Data
    //**************************
    void RecalculateData()
    {
        //RecalculateVerticies();
        //RecalculateNormals();
        //ReloadMeshData();
        double ts=glfwGetTime();
        SetupVerts();
        SetInitalTerrainOnGPU();
        std::cout << "ReCalcTime: " << glfwGetTime()-ts << "s" << std::endl;
    };

    //**************************
    //  Get GPU Requirements
    //**************************
    int GetGPUMemoryReqs()
    {
        int rtnval=0;

        if (GPUDataSet)
        {
            rtnval += sizeof(Vertex) * this->meshVerts.back().size() * this->buffers.size();
            rtnval += sizeof(GLuint) * this->idxs.back().size() * this->buffers.size();
            rtnval /= (1024*1024);
        }

        return rtnval;
    };

    //**************************
    //     Number of Verts
    //**************************
    int GetNumberVerts()
    {
        int rtnval=0;

        if (GPUDataSet)
        {
            rtnval = this->meshVerts.back().size() * this->buffers.size();
        }

        return rtnval;
    };

    //------------------------------------
    //        TOOLBOX INTERFACING
    //------------------------------------
    //*********************
    // Return Creation Data
    //**********************
    TerrainCreationData GetCreationData()
    {
        TerrainCreationData data(terrainSize,NSmooth,subdiv,heightVariation);
        return data;
    };

    //*********************
    // Return Creation Data
    //**********************
    void SaveCreationData(TerrainCreationData &data)
    {
        int rtnSize;
        int rtnSmooth;
        glm::ivec3 rtnHV;
        data.ReturnData(rtnSize,rtnSmooth,subdiv,rtnHV);
        SetupTerrainCreationParameters(rtnSize,rtnSmooth,rtnHV);
    };

    //**************************
    // Return Modification Data
    //**************************
    TerrainModificationData GetModificationData()
    {
        glm::vec3 relHeight(relativeHeight.y,relativeHeight.z,relativeHeight.w);
        TerrainModificationData data(heightMult,relHeight);
        return data;
    };

    //**************************
    // Return Modification Data
    //**************************
    void SaveModificationData(TerrainModificationData &data)
    {
        float rtnSize;
        glm::vec3 rtnHV;
        data.ReturnData(rtnSize,rtnHV);
        std::cout << "rtnSize: " << rtnSize << " rtnHV: " << rtnHV.x << "," << rtnHV.y << "," << rtnHV.z << std::endl;
        SetupTerrainModifyParameters(rtnSize,rtnHV);
    };

    //**************************
    // Return Modification Data
    //**************************
    MaterialModificationData GetMaterialModificationData()
    {
        MaterialModificationData mmdata(materials.shine,materials.Ka,materials.Kd,materials.Ks);
        return mmdata;
    };

    //**************************
    // Return Modification Data
    //**************************
    void SaveMaterialModificationData(MaterialModificationData &data)
    {
        float shine;
        glm::vec3 Ka;
        glm::vec3 Kd;
        glm::vec3 Ks;
        data.ReturnData(shine,Ka,Kd,Ks);
        SetupMaterials(Ka,Kd,Ks,shine);
    };

    //**************************
    // Set land mesh buffer
    //**************************
    /*void SetLandMeshBuffer()
    {
        glm::vec2 mpos=positions[i];
        glm::vec3 landMeshPos=glm::vec3(mpos.x,0.0f,mpos.y);

        if (glm::length(landMeshPos-vPolePos)<100.0f)
        {

        }
    };*/

    void lowerTerrain(InputStruct &input,RTSCamera &camera)
    {
        modifyElevation(0,input,camera);
    };

    void raiseTerrain(InputStruct &input,RTSCamera &camera)
    {
        modifyElevation(1,input,camera);
    };

    void modifyElevation(int func,InputStruct &input,RTSCamera &camera);

private:
    //**************************
    //Terrain Building Functions
    //**************************
    void AllocateData (int size);
    void GenerateTerrainData(int size);
    int AverageHeights(int i,int j, std::vector<std::vector<int>> &data,int size);

    // Setup a Regular Mesh
    void setupMeshRegular();

    // Draws the Mesh (Singular)
    void Draw();

    // Calculate the max and low heights
    void RecalculateMaxMinHeights();

    // Recalculate Normals
    void RecalculateNormals();

    // Recalculate Normals
    void RecalculateVerticies();

    // Setup Verties
    void SetupVerts();

    // Set Materials
    void setMaterialUniform();

    // Set Relative Height Parameters
    void SetRelativeHeightUniform();
};
#endif
