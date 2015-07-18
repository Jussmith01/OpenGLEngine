#ifndef TERRAINGENERATORWRAPPER_C
#define TERRAINGENERATORWRAPPER_C

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "../Tools/rtscamera.h"
#include "../engine.h"
#include "testingbox.h"
#include "../Handlers/ModelHandler/model.h"
#include "../Handlers/LightHandler/staticskylight.h"
#include "../Loaders/texture.h"
#include "../Tools/screenwriter.h"
#include "../Tools/ToolBoxs/terraincreationtoolbox.h"
#include "../Tools/ToolBoxs/terrainsculptingtoolbox.h"
#include "../Tools/ToolBoxs/savetoolbox.h"
#include "TerrainGenerator/terrainGenerator.h"

//******************************************//
//    Terrain Generator Wrapper Class       //
//******************************************//
/*
Bundles all terrain generator functionality into
one class. This acts to deploy the different
terrain generation functionality and toolboxes
on the the screen for rendering and interaction
with the scene.
*/
class TerrainGeneratorWrapper
{
    // Class Variables
    double dt;
    double timeChk;
    double frametime;

    int framecnt;
    int swidth;
    int sheight;

    bool wireframe;
    bool numbermesh;

    // Program Testing
    std::vector<Model> models;
    Shader shader;
    Texture texture;
    StaticSkyLighting skylight;

    // Terrain Creation Toolbox
    TerrainCreationToolbox tctoolbox;

    // Terrain Modification Toolbox
    TerrainModificationToolbox tmtoolbox;

    // Material Modification Toolbox
    MaterialModificationToolbox mmtoolbox;

    // Terrain Scupting Toolbox
    TerrainSculptingToolbox tstoolbox;

    // Save Terrain Toolbox
    SaveToolbox sttoolbox;

    // Export Terrain Toolbox
    SaveToolbox ettoolbox;

    // Terrain Generator Class
    TerrainGeneration terrainGen;

    // Class Wrappers
    RTSCamera camera;

    // Class programs
    ScreenWriter text;

    // Menu Bar
    BarSelection selID;
    MenuBar mbar;

public:
    TerrainGeneratorWrapper() {};
    ~TerrainGeneratorWrapper () {};

    //**************************
    //  World Builder Functions
    //**************************
    void Init (Engine *game);
    void UpdateEvents(InputStruct &input);
    void UpdateData(void);
    void ToolBoxDraw(void);
    void Draw(void);
    void Cleanup(void);
};
#endif
