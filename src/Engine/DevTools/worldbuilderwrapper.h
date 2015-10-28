#ifndef WORLDBUILDERWRAPPER_C
#define WORLDBUILDERWRAPPER_C

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "../Tools/rtscamera.h"
#include "../engine.h"
#include "testingbox.h"
#include "../Handlers/ModelHandler/model.h"
#include "../Handlers/LightHandler/staticskylight.h"
#include "../Loaders/texture.h"
#include "../Tools/screenwriter.h"
#include "TerrainGenerator/terrainGenerator.h"

//******************************************//
//      World Builder Wrapper Class         //
//******************************************//
/*
Bundles all world builder functionality into
one class.
*/
class WorldBuilderWrapper
{
    // Class Variables
    double dt;
    double frametime;

    // Program Testing
    std::vector<Model> models;
    Shader shader;
    Texture texture;
    StaticSkyLighting skylight;

    // Class Wrappers
    RTSCamera camera;

    // Class programs
    ScreenWriter text;

public:
    WorldBuilderWrapper() {};
    ~WorldBuilderWrapper () {};

    //**************************
    //  World Builder Functions
    //**************************
    void Init (GrafixEngine *game);
    void UpdateEvents(InputStruct &input);
    void UpdateData(void);
    void Draw(void);
    void Cleanup(void);
};
#endif
