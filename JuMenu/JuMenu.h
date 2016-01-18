#ifndef JUMENUAPI_H
#define JUMENUAPI_H

#include <string>

#include "menuitems/menubutton.h"

#include "JuMenuResourceManager.h"

namespace JuMenu {

/* --------------------------------------
                Menu Class

Class is in charge of loading saved menus
then displaying them using a GL window.

-------------------------------------- */
class Menu {
    /* Resource Manager */
    MenuResourceManager mrm;

    /* Menu Buttons */
    std::vector<MenuButtons> mbtn; // Menu buttons

    /** Setup Functions Below **/
    /* Set the menu items based on the input file */
    void m_SetMenuItems( std::string msfile );

public:
    /* Constructors */
    void Init (std::string msfile,Properties *props,ISoundEngine* audioengine) {
        swidth = props->WinWidth;
        sheight = props->WinHeight;

        mrm.Init(props,audioengine);

        m_SetMenuItems(msfile);
    };

    void Update ();

    void Draw ();

    void Clear ();
};
}

#endif
