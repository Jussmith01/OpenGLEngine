#ifndef PROPSIFACE_H
#define PROPSIFACE_H

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"

#include "../../Tools/MenuItems.h"
#include "../../Tools/input_struct.h"

#include "requestinterface.h"

#include <irrKlang.h>
#include <ik_ISound.h>
using namespace irrklang;

//-------------------------------------------//
//               Properties Class            //
//-------------------------------------------//
struct GraphicsPropertiesInterface
{
    GraphicsPropertiesInterface () {};
    ~GraphicsPropertiesInterface () {};

    //****************************
    // Graphics Settings Interface
    //****************************
    void InitGPI(GrafixEngine* game,ISoundEngine* audioengine);
    int UpdateGPI(InputStruct &input);
    void PropertiesSave(void);
    void PropertiesLoad(void);
    void DrawGPI(void);
    void CleaupGPI(void);

private:
    GrafixEngine* game;
    ISoundEngine* audioengine;

    DisplayBox box;
    MenuButtons buttons;
    ScreenWriter text;
    MenuCheckBox chkbox[2];
    MenuDropBox drpbox[2];
    RequestInterface appInterface;

    // Variables
    std::vector<int> rwidth;
    std::vector<int> rheight;
    int bID;

    // GPI Tools
    int MSAAMap(int idx,bool inv);
    void GetResolutions(std::vector<std::string> &res,int &current);
    std::string AspectString(void);
};

#endif
