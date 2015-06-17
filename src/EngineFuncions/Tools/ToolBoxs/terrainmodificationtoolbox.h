#ifndef TERRAINMODIFICATIONTOOLBOX_C
#define TERRAINMODIFICATIONTOOLBOX_C

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
struct TerrainModificationData
{
    std::string heightMult;
    std::string relheight[3];

    TerrainModificationData() {};

    TerrainModificationData(float heightMult,glm::vec3 relheight)
    {
        std::stringstream ss[4];

        ss[0] << heightMult;
        this->heightMult=ss[0].str();

        ss[1] << relheight.x;
        this->relheight[0]=ss[1].str();

        ss[2] << relheight.y;
        this->relheight[1]=ss[2].str();

        ss[3] << relheight.z;
        this->relheight[2]=ss[3].str();
    };

    //Class Assignment
    TerrainModificationData& operator=(const TerrainModificationData& instance)
    {
        this->heightMult = instance.heightMult;
        this->relheight[0] = instance.relheight[0];
        this->relheight[1] = instance.relheight[1];
        this->relheight[2] = instance.relheight[2];
        return *this;
    };

    void ReturnData(float &heightMult,glm::vec3 &relheight)
    {
        heightMult=atof(this->heightMult.c_str());
        relheight.x=atof(this->relheight[0].c_str());
        relheight.y=atof(this->relheight[1].c_str());
        relheight.z=atof(this->relheight[2].c_str());
    };
};

//*****************************************//
//	     Terrain Creation Toolbox	       //
//*****************************************//
class TerrainModificationToolbox
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
    InsertionBox insertbox[4];
    MenuButtons buttons;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(float posx, float posy,Properties* props,ISoundEngine* audioengine,TerrainModificationData &tmdata);

    // Cleanup function for destruction
    void Cleanup();

    // Returns ButtonID if button was Pressed and Released while over a Button
    int UpdateEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawToolBox();

    // Draw all buttons to the screen
    TerrainModificationData FetchTerrainModificationData();
};


#endif
