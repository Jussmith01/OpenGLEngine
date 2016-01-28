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
    /* Initializer */
    void Init (std::string msfile,Properties *props,ISoundEngine* audioengine);

    /* Update */
    void Update ();

    /* Draw */
    void Draw ();

    /* Cleanup */
    void Cleanup ();
};
}

#endif
