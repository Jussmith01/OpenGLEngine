#include "terraingeneratorwrapper.h"

//******************************************//
//   World Builder Wrapper Initialization   //
//******************************************//
/*
Builder Initializer
*/
void TerrainGeneratorWrapper::Init (Engine *game)
{
    this->swidth=game->props.WinWidth;
    this->sheight=game->props.WinHeight;
    this->wireframe=false;
    this->numbermesh=true;

    // Initialize Camera Class
    camera.Init(20.0,50.0,0.0,0.0,game->props.WinWidth,game->props.WinHeight);
    camera.SetCameraSpeeds(0.5f,1.0f,5.0f,20.0f);

    // Initialize the overhead "sky" lighting
    skylight.InitStatic(glm::vec3(0.0f,-0.5f,-0.5f),glm::vec3(1.0f),1.0f);

    // Initialize screen writing class
    text.Setup("../Fonts/FreeSans.ttf",game->props.WinWidth,game->props.WinHeight,game->props.FontSize);

    //***********************
    //Setup Creation Toolbox
    //***********************
    // Initialize the Terrain CModification Toolbox
    TerrainCreationData tctoolboxdata=terrainGen.GetCreationData();
    tctoolbox.Init(0.0f,0.0f,&game->props,game->audioengine,tctoolboxdata);

    //***********************
    //Setup Modify Toolbox
    //***********************
    // Initialize the Terrain Creation Toolbox
    TerrainModificationData tmtoolboxdata=terrainGen.GetModificationData();
    tmtoolbox.Init(0.0f,0.0f,&game->props,game->audioengine,tmtoolboxdata);

    //***********************
    //Setup Material Toolbox
    //***********************
    // Initialize the Terrain Creation Toolbox
    MaterialModificationData mmtoolboxdata=terrainGen.GetMaterialModificationData();
    mmtoolbox.Init(0.0f,0.0f,&game->props,game->audioengine,mmtoolboxdata);

    //***********************
    //  Save Terrain Toolbox
    //***********************
    // Initialize the Terrain Creation Toolbox
    sttoolbox.Init(0.0f,0.0f,&game->props,game->audioengine);

    //********************
    // Setup the menu bar
    //********************
    std::vector< SingleBarMenuItem > smbi;
    smbi.resize(4);

    smbi[0].title="File";
    smbi[0].options.push_back("Load");
    smbi[0].options.push_back("Save");
    smbi[0].options.push_back("Export");

    smbi[1].title="View";
    smbi[1].options.push_back("Wireframe Mode");
    smbi[1].options.push_back("Number Mesh");

    smbi[2].title="Generate";
    smbi[2].options.push_back("Terrain");
    smbi[2].options.push_back("Set Textures");
    smbi[2].options.push_back("Unset Textures");
    smbi[2].options.push_back("Set Shader");
    smbi[2].options.push_back("Unset Shader");
    smbi[2].options.push_back("Reset Shader");

    smbi[3].title="Modify";
    smbi[3].options.push_back("Terrain");
    smbi[3].options.push_back("Material");

    mbar.Init(smbi,game->props);
    smbi.clear();

    //*******************
    // Initialize Timing
    //*******************
    dt=glfwGetTime();
    timeChk=glfwGetTime();
    framecnt=0;
};

//******************************************//
//              Update Events               //
//******************************************//
/*
Updates all events for the wrapper class
*/
void TerrainGeneratorWrapper::UpdateEvents(InputStruct &input)
{
    //*******************
    // Default Bar State
    //*******************
    if (!selID.isset)
    {
        selID=mbar.UpdateBarEvents(input);
    }

    //******************************
    //         File Menu
    //******************************
    if (selID.isset && selID.menu==0)
    {
        //*******************************
        //        Load Terrain
        //*******************************
        if (selID.option==0)
        {
            selID.reset();
        }

        //*******************************
        //        Save Terrain
        //*******************************
        if (selID.option==1)
        {
            int sttbCall=sttoolbox.UpdateEvents(input);
            if(sttbCall==0)
            {
                //*****************************
                //    SAVE TERRAIN FUNCTIONS
                //*****************************
                std::string fn=sttoolbox.GetFileName();
            }

            if(sttbCall==1)
            {
                selID.reset();
            }
        }

        //*******************************
        //        Export Terrain
        //*******************************
        if (selID.option==2)
        {
            selID.reset();
        }
    }

    //******************************
    //         View Menu
    //******************************
    if (selID.isset && selID.menu==1)
    {
        //*******************************
        //     Set Wireframe Terrain
        //*******************************
        if (selID.option==0)
        {
            if (!wireframe)
            {
                wireframe=true;
            }
            else
            {
                wireframe=false;
            }

            selID.reset();
        }

        //*******************************
        //     Set Numbered Meshes
        //*******************************
        if (selID.option==1)
        {
            if (!numbermesh)
            {
                numbermesh=true;
            }
            else
            {
                numbermesh=false;
            }

            selID.reset();
        }
    }

    //******************************
    //      Build Menu
    //******************************
    if (selID.isset && selID.menu==2)
    {
        //******************************
        //   Terrain Creation Toolbox
        //******************************
        if (selID.option==0)
        {
            int tctbCall=tctoolbox.UpdateEvents(input);
            if(tctbCall==0)
            {
                //*****************************
                // TERRAIN CREATION FUNCTIONS
                //*****************************
                TerrainCreationData tcdata=tctoolbox.FetchTerrainCreationData();
                terrainGen.SaveCreationData(tcdata);
                terrainGen.GenerateTerrain();
                terrainGen.SetInitalTerrainOnGPU();
            }

            if(tctbCall==1)
            {
                selID.reset();
            }
        }

        //******************************
        //  Set the Textures and Shader
        //******************************
        if (selID.option==1)
        {
            terrainGen.SetTextures();
            selID.reset();
        }

        //******************************
        // Unset the Textures and Shader
        //******************************
        if (selID.option==2)
        {
            terrainGen.UnsetTextures();
            selID.reset();
        }

        //******************************
        //       Set the Shader
        //******************************
        if (selID.option==3)
        {
            terrainGen.SetShader();

            //*****************
            // Register Shader
            //*****************
            camera.RegisterShaderWithCameraDataUBO(terrainGen.shader);
            skylight.RegisterShader(terrainGen.shader);

            selID.reset();
        }

        //******************************
        //      Unset the Shader
        //******************************
        if (selID.option==4)
        {
            terrainGen.UnsetShader();
            selID.reset();
        }

        //******************************
        //      Reset the Shader
        //******************************
        if (selID.option==5)
        {
            terrainGen.UnsetShader();

            terrainGen.SetShader();

            //*****************
            // Register Shader
            //*****************
            camera.RegisterShaderWithCameraDataUBO(terrainGen.shader);
            skylight.RegisterShader(terrainGen.shader);

            selID.reset();
        }
    }

    //******************************
    //     Modification Menu
    //******************************
    if (selID.isset && selID.menu==3)
    {
        //******************************
        // Terrain Modification Toolbox
        //******************************
        if (selID.option==0)
        {
            int tmtbCall=tmtoolbox.UpdateEvents(input);
            if(tmtbCall==0)
            {
                //*****************************
                //  TERRAIN MODIFY FUNCTIONS
                //*****************************
                TerrainModificationData tmdata=tmtoolbox.FetchTerrainModificationData();
                terrainGen.SaveModificationData(tmdata);
                terrainGen.RecalculateData();
            }

            if (tmtbCall==1)
            {
                selID.reset();
            }
        }

        //*******************************
        // Material Modification Toolbox
        //******************************
        if (selID.option==1)
        {
            int mmtbCall=mmtoolbox.UpdateEvents(input);
            if(mmtbCall==0)
            {
                //*****************************
                //  TERRAIN MODIFY FUNCTIONS
                //*****************************
                MaterialModificationData mmdata=mmtoolbox.FetchMaterialModificationData();
                terrainGen.SaveMaterialModificationData(mmdata);
            }

            if (mmtbCall==1)
            {
                selID.reset();
            }
        }
    }

    camera.UpdateCamera(input);
};

//******************************************//
//            Toolbox Drawing               //
//******************************************//
/*

*/
void TerrainGeneratorWrapper::ToolBoxDraw()
{
    mbar.DrawBar();

    if (selID.isset)
    {
        //******
        // File
        //******
        if(selID.menu==0)
        {
            // Load Menu
            if(selID.option==0)
            {

            }

            // Save Menu
            if(selID.option==1)
            {
                sttoolbox.DrawToolBox();
            }

            // Save Menu
            if(selID.option==2)
            {

            }
        }

        //************
        // Build Menu
        //************
        if(selID.menu==2)
        {
            // Create Terrain
            if(selID.option==0)
            {
                tctoolbox.DrawToolBox();
            }
        }

        //*************
        // Modify Menu
        //*************
        if(selID.menu==3)
        {
            // Terrain Modify Menu
            if(selID.option==0)
            {
                tmtoolbox.DrawToolBox();
            }

            // Material Modify Menu
            if(selID.option==1)
            {
                mmtoolbox.DrawToolBox();
            }
        }
    }
};

//******************************************//
//             Update the Data              //
//******************************************//
/*
Updates all data for the wrapper class
*/
void TerrainGeneratorWrapper::UpdateData()
{
    ++framecnt;
    if (glfwGetTime()-timeChk >= 1.0)
    {
        frametime=1000.0/(float)framecnt;
        framecnt=0;
        timeChk=glfwGetTime();
    }

    dt=glfwGetTime();
};

//******************************************//
//      Draw everything for the Wrapper     //
//******************************************//
/*
Draw everything for the wrapper class
*/
void TerrainGeneratorWrapper::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Draw Terrain Object
    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    terrainGen.DrawCall();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDisable(GL_DEPTH_TEST);

    // Draw mesh numbers
    if(numbermesh)
    {
        for (int i=0; i<(int)terrainGen.positions.size(); ++i)
        {
            glm::vec2 mpos=terrainGen.positions[i];
            float dp=glm::dot(glm::normalize(glm::vec3(mpos.x,0.0f,mpos.y)-camera.cameraPos),glm::normalize(camera.cameraDir));

            if (dp > 0)
            {
                glm::vec3 screenpos=glm::project(glm::vec3(mpos.x,0.0f,mpos.y),camera.VM,camera.PM,glm::vec4(0,0,swidth,sheight));
                tools::normal_device_coordinate(screenpos.x,screenpos.y,swidth,sheight);
                std::stringstream ssn;
                ssn << i;
                text.RenderTextCentered(ssn.str(),1,screenpos.x,1,-screenpos.y,0.9f,glm::vec3(1.0f));
            }
        };
    }

    // Draw any active toolboxes
    ToolBoxDraw();

    // Draw the data stuff
    std::stringstream ss;
    ss << "Frame Time: " << frametime << "ms/f";
    text.RenderTextRightJustified(ss.str(),0.98,0.95,0.9f,glm::vec3(1.0f));

    std::stringstream ss2;
    ss2 << "GPU Mem Usage: " << terrainGen.GetGPUMemoryReqs() << "MB";
    text.RenderTextRightJustified(ss2.str(),0.98,0.9,0.9f,glm::vec3(1.0f));

    std::stringstream ss3;
    ss3 << "Number of Verts: " << terrainGen.GetNumberVerts();
    text.RenderTextRightJustified(ss3.str(),0.98,0.85,0.9f,glm::vec3(1.0f));
};

//******************************************//
//      World Builder Wrapper Cleanup       //
//******************************************//
/*
Cleaup the class
*/
void TerrainGeneratorWrapper::Cleanup()
{
    camera.Cleanup();
    skylight.Cleanup();
    text.Cleanup();
    terrainGen.Cleanup();

    // Cleanup toolboxes
    tctoolbox.Cleanup();
    tmtoolbox.Cleanup();
    mmtoolbox.Cleanup();
    sttoolbox.Cleanup();

    //Cleanup bars
    mbar.Cleanup();
};
