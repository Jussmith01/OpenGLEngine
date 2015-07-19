#ifndef TERRAINGENERATOR_C
#define TERRAINGENERATOR_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include "../../Handlers/TerrainHandler/terrainhandler.h"
#include "../worldbuildertools/randlib.h"
#include "../worldbuildertools/boxclass.h"
#include "../../Handlers/ModelHandler/base_classes.h"
#include "../../Tools/rtscamera.h"
#include "../../Tools/console.h"
#include "../../Tools/micro_timer.h"
#include "../../Tools/ToolBoxs/terraincreationtoolbox.h"
#include "../../Tools/ToolBoxs/terrainmodificationtoolbox.h"
#include "../../Tools/ToolBoxs/materialmodificationtoolbox.h"

//******************************************//
//         Terrain Generator Class          //
//******************************************//
/*
    The TerrainGenerator class holds all
    required functions and storage space
    for building terrains. This class
    inherits the TerrainHandler class for
    access to the drawing, gpu data loading,
    shader loading and texture loading
    functionality needed to draw the terrain
    to the screen.
*/
class TerrainGeneration : public TerrainHandler
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
    float heightMult; // Height multiplier

    glm::ivec3 heightVariation;

    //************
    // Built Data
    //************
    std::vector< Vertex > verts;

    //std::vector< glm::vec2 > positions; // mesh positions

    float lowShift;
    float highShift;

    float sizeScale; // Distance between verts

    //**********
    //   Timer
    //**********
    microTimer generalTimer;

public:
    /*MOVE BACK TO PRIVATE WHEN DONE*/
    double meshwidth;

    TerrainGeneration()
    {
        SetDefaults();
    };

    ~TerrainGeneration () {};

    //*********************************************
    //         Generates Terrain on CPU
    //*********************************************
    void GenerateTerrain()
    {
        Console::cPrint("Generating Terrain Data:");
        Console::cPrint(tools::appendStrings("Terain Size: ",this->terrainSize),true);
        GenerateTerrainData(this->terrainSize);

        Console::cPrint("Setting up Verts:");
        //std::cout << "Setting up Verts...\n";
        SetupVerts();
    };


    //****************
    // Setup Defaults
    //****************
    void SetDefaults ()
    {
        SetShaderFile("terrain"); //Inherited Function
        SetTexFiles("lowland.png","grass1.png","rocks.png","cliff1.png"); //Inherited Function

        sizeScale=10.0f;

        std::cout << "Setting up Materials...\n";
        glm::vec3 Ka(0.1f);
        glm::vec3 Kd(0.9f);
        glm::vec3 Ks(0.1f);
        float shininess=64.0;
        SetupMaterials(Ka,Kd,Ks,shininess); //Inherited Function

        std::cout << "Setting up Parameters...\n";
        SetupTerrainCreationParameters(5,2,glm::ivec3(500,150,150));
        SetupTerrainModifyParameters(0.3f,glm::vec3(0.1f,0.8f,0.05f));

        subdiv=2;
    };

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
        SetTerrainOnGPU();
        Console::cPrint(tools::appendStrings("ReCalcTime: " ,glfwGetTime()-ts,"s"));
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
        glm::vec3 relHeight(GetRelHeight().y,GetRelHeight().z,GetRelHeight().w);
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
        //std::cout << "rtnSize: " << rtnSize << " rtnHV: " << rtnHV.x << "," << rtnHV.y << "," << rtnHV.z << std::endl;
        SetupTerrainModifyParameters(rtnSize,rtnHV);
    };

    //**************************
    // Return Modification Data
    //**************************
    MaterialModificationData GetMaterialModificationData()
    {
        MaterialModificationData mmdata(GetMaterial().shine,GetMaterial().Ka,GetMaterial().Kd,GetMaterial().Ks);
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
        modifyElevation(0,1.0f,input,camera);
    };

    void raiseTerrain(InputStruct &input,RTSCamera &camera)
    {
        modifyElevation(1,1.0f,input,camera);
    };

    void levelTerrain(InputStruct &input,RTSCamera &camera)
    {
        modifyElevation(2,1.0f,input,camera);
    };

    void modifyElevation(int func,float eff,InputStruct &input,RTSCamera &camera);

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

    // Recalculate Verticies
    void RecalculateVerticies();

    // Setup Verties
    void SetupVerts();

    // Modify the height data
    void ModifyHeightData(glm::vec3 point,int updown,float eff);

    // Level the height data
    void LevelHeightData(glm::vec3 point,float eff);
};
#endif
