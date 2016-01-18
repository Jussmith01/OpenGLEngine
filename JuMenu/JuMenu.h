#ifndef JUMENUAPI_H
#define JUMENUAPI_H

#include <string>

#include "src/boxcolored.h"
#include "src/checkbox.h"
#include "src/dropbox.h"
#include "src/insertionbox.h"
#include "src/menubutton.h"
#include "src/menuicon.h"
#include "src/menubar.h"
#include "src/dropdownmenu.h"

namespace JuMenu {

/* --------------------------------------
          Menu Resource Manager

Allows for dependency injection into the
primary Menu Class

-------------------------------------- */
class MenuResourceManager {

    /* Audio Engine Pointer */
    ISoundEngine* audioengine;

    /* Properties Pointer */
    Properties* props;

    /* Resources */
    std::vector<ScreenWriter> swrit; // Vector for different fonts
    std::vector<ImageDisplay> image; // Vector for images

    public:
    /*Constructor*/
    MenuResourceManager() {};

    void Init (Properties *props,ISoundEngine* audioengine) {
        this->audioengine = audioengine;
        this->props = props;
    }
};


/* --------------------------------------
                Menu Class

Class is in charge of loading saved menus
then displaying them using a GL window.

-------------------------------------- */
class Menu {

    MenuResourceManager mrm;

    std::vector<MenuButtons> mbtn; // Menu buttons

    /** Setup Functions Below **/
    /* Set the menu items based on the input file */
    void m_SetMenuItems( std::string msfile,Properties *props,ISoundEngine* audioengine );

public:
    /* Constructors */
    void Init (std::string msfile,Properties *props,ISoundEngine* audioengine) {
        mrm.Init(props,audioengine);

        m_SetMenuItems(msfile,props,audioengine);
    };

    void Update ();

    void Draw ();

    void Clear ();
};
}

#endif
