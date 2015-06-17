#ifndef TERRAINCREATIONTOOLBOX_C
#define TERRAINCREATIONTOOLBOX_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"

#include "../MenuItems.h"
#include "../displayimage.h"
#include "../screenwriter.h"
#include "../input_struct.h"

#include <irrKlang.h>
#include <ik_ISound.h>
using namespace irrklang;
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

//*****************************************//
//	      Terrain Creation Data   	       //
//*****************************************//
struct TerrainCreationData
{
    std::string terrainSize;
    std::string NSmooth;
    std::string SubDivision;
    std::string heightVariation[3];

    TerrainCreationData() {};

    TerrainCreationData(int terrainSize,int NSmooth,int SubDivision,glm::ivec3 heightVariation)
    {
        std::stringstream ss[6];

        ss[0] << terrainSize;
        this->terrainSize=ss[0].str();

        ss[1] << heightVariation.x;
        this->heightVariation[0]=ss[1].str();

        ss[2] << heightVariation.y;
        this->heightVariation[1]=ss[2].str();

        ss[3] << heightVariation.z;
        this->heightVariation[2]=ss[3].str();

        ss[4] << NSmooth;
        this->NSmooth=ss[4].str();

        ss[5] << SubDivision;
        this->SubDivision=ss[5].str();
    };

    //Class Assignment
    TerrainCreationData& operator=(const TerrainCreationData& instance)
    {
        this->terrainSize = instance.terrainSize;
        this->NSmooth = instance.NSmooth;
        this->SubDivision = instance.SubDivision;
        this->heightVariation[0] = instance.heightVariation[0];
        this->heightVariation[1] = instance.heightVariation[1];
        this->heightVariation[2] = instance.heightVariation[2];
        return *this;
    };

    void ReturnData(int &terrainSize,int &NSmooth,int &SubDivision,glm::ivec3 &heightVariation)
    {
        terrainSize=atoi(this->terrainSize.c_str());
        NSmooth=atoi(this->NSmooth.c_str());
        SubDivision=atoi(this->SubDivision.c_str());
        heightVariation.x=atoi(this->heightVariation[0].c_str());
        heightVariation.y=atoi(this->heightVariation[1].c_str());
        heightVariation.z=atoi(this->heightVariation[2].c_str());
    };
};

//*****************************************//
//	     Terrain Creation Toolbox	       //
//*****************************************//
class TerrainCreationToolbox
{
    // Variables
    std::string sound;
    float swidth,sheight;
    float x,y;
    float lh,lw;

    double dt;//For press timing

    // Tools
    Properties *props;
    ISoundEngine* audioengine;

    DisplayBox frame;
    ScreenWriter text;

    // Testing!
    InsertionBox insertbox[6];
    MenuButtons buttons;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(float posx, float posy,Properties* props,ISoundEngine* audioengine,TerrainCreationData &tcdata);

    // Cleanup function for destruction
    void Cleanup();

    // Returns ButtonID if button was Pressed and Released while over a Button
    int UpdateEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawToolBox();

    // Draw all buttons to the screen
    TerrainCreationData FetchTerrainCreationData();
};


#endif
