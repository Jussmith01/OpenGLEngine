#ifndef REQIFACE_H
#define REQIFACE_H

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"

#include "../../Tools/MenuItems.h"
#include "../../Tools/input_struct.h"

#include <irrKlang.h>
#include <ik_ISound.h>
using namespace irrklang;

//-------------------------------------------//
//               Properties Class            //
//-------------------------------------------//
struct RequestInterface
{
    RequestInterface () {};
    ~RequestInterface () {};

    //****************************
    // Graphics Settings Interface
    //****************************
    void Init(std::string content,std::string rtn0,std::string rtn1,
              GrafixEngine* game,ISoundEngine* audioengine);

    int Update(InputStruct &input);
    void Draw(void);
    void Cleaup(void);
    void SetTimeout(float totime,int selection);

private:
    GrafixEngine* game;
    ISoundEngine* audioengine;

    DisplayBox box;
    MenuButtons buttons;
    ScreenWriter text;

    // Variables
    std::string content;

    // Timeout Variables
    bool touse;
    bool toset;

    float stime;
    float ctime;
    float totime;
    int selection;

    // Private Timeout Functions
    int UpdateTimeout(void);
};

#endif
