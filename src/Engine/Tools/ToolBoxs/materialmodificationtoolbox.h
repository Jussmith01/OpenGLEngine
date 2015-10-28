#ifndef MATERIALMODIFICATIONTOOLBOX_C
#define MATERIALMODIFICATIONTOOLBOX_C

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
//	   Material Modification Data   	   //
//*****************************************//
struct MaterialModificationData
{
    std::string shine;
    std::string Ka[3];
    std::string Kd[3];
    std::string Ks[3];

    MaterialModificationData() {};

    MaterialModificationData(float shine,glm::vec3 Ka,glm::vec3 Kd,glm::vec3 Ks)
    {
        std::stringstream ss;

        ss << shine;
        this->shine=ss.str();

        for (int i=0;i<3;++i)
        {
            std::stringstream ss[3];

            ss[0] << Ka[i];
            this->Ka[i]=ss[0].str();

            ss[1] << Kd[i];
            this->Kd[i]=ss[1].str();

            ss[2] << Ks[i];
            this->Ks[i]=ss[2].str();
        }
    };

    //Class Assignment
    MaterialModificationData& operator=(const MaterialModificationData& instance)
    {
        this->shine = instance.shine;

        for (int i=0;i<3;++i)
        {
            this->Ka[i] = instance.Ka[i];
            this->Kd[i] = instance.Kd[i];
            this->Ks[i] = instance.Ks[i];
        }
        return *this;
    };

    void ReturnData(float &shine,glm::vec3 &Ka,glm::vec3 &Kd,glm::vec3 &Ks)
    {
        shine=atof(this->shine.c_str());

        for (int i=0;i<3;++i)
        {
            Ka[i]=atof(this->Ka[i].c_str());
            Kd[i]=atof(this->Kd[i].c_str());
            Ks[i]=atof(this->Ks[i].c_str());
        }
    };
};

//*****************************************//
//	     Terrain Creation Toolbox	       //
//*****************************************//
class MaterialModificationToolbox
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
    InsertionBox insertbox[10];
    MenuButtons buttons;

    // Check if mouse is over button
    bool CheckMouseOver(float x,float y);

public:
    // Initialize Button Class
    void Init(float posx, float posy,Properties* props,ISoundEngine* audioengine,MaterialModificationData &mmdata);

    // Cleanup function for destruction
    void Cleanup();

    // Returns ButtonID if button was Pressed and Released while over a Button
    int UpdateEvents(InputStruct &input);

    // Draw all buttons to the screen
    void DrawToolBox();

    // Draw all buttons to the screen
    MaterialModificationData FetchMaterialModificationData();
};

#endif
