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

/*             Menu Class
Class is in charge of loading saved menus
then displaying them using a GL window.
*/
class Menu {

    std::vector<MenuButtons> menubuttons; // Setup!

    std::vector<MenuIcons> menuicons;
    std::vector<MenuDropBox> menudropboxes;
    std::vector<DisplayBox> displayboxes;
    std::vector<MenuCheckBox> menucheckboxes;
    std::vector<DropDownMenu> dropdownmenus;
    std::vector<InsertionBox> insertionboxes;
    std::vector<MenuBar> menubars;

    std::vector<ImageDisplay> images;
    std::vector<ScreenWriter> texts;

    /** Setup Functions Below **/
    /* Set the menu items based on the input file */
    void m_SetMenuItems( std::string msfile );

    /** Set Menu Item Functions Below **/
    /* Set MenuDropBox */
    void m_SetMenuDropBox( std::string dstr );

    /* Set DisplayBox */
    void m_SetDisplayBox( std::string dstr );

    /* Set MenuCheckBox */
    void m_SetMenuCheckBox( std::string dstr );

    /* Set DropDownMenu */
    void m_SetDropDownMenu( std::string dstr );

    /* Set InsertionBox */
    void m_SetInsertionBox( std::string dstr );

    /* Set MenuBar */
    void m_SetMenuBar( std::string dstr );

    /* Set MenuButtons */
    void m_SetMenuButtons( std::string dstr );

    /* Set MenuIcons */
    void m_SetMenuIcons( std::string dstr );

public:
    /* Constructors */
    Menu () {};

    Menu (std::string msfile) {
        m_SetMenuItems(msfile);
    };

    void SetupMenus (std::string msfile) {
        m_SetMenuItems(msfile);
    };

    void Update ();

    void Draw ();

    void Clear ();
};
}

#endif
