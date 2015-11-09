#include <iostream>
#include <fstream>

#include <regex>
#include <unordered_map>
#include "JuMenu.h"
#include "JuMenuErrs.h"

#include "src/mfftools.h"

/* Set the menu items based on the input file */
void JuMenu::Menu::m_SetMenuItems( std::string msfile ) {
    using namespace std;

    ifstream filebuffer(msfile.c_str(), ios::in | ios::binary);

    if (!filebuffer) {
        cout << "NO FILE!" << endl;
        JmenuThrow(string("Cannot open file ") + msfile);
    }

    // Load entire file into a string on memory
    string instr( (istreambuf_iterator<char>(filebuffer)), istreambuf_iterator<char>() );
    filebuffer.close();

    unordered_map<string,string> mItems;
    fileSplit(instr,mItems);

    for (auto &&i : mItems) {
        cout << "TEST: " << i.first << endl;
        if (i.first.compare("MenuButtons")==0) {
            cout << "Saving Menu Buttons!" << endl;

        }
    }

    cout << " MAP: " << mItems.size() << endl;
};

/* Update Menu Items */
void JuMenu::Menu::Update () {

};

/* Draw Menu Items */
void JuMenu::Menu::Draw () {

};

/* Clear Menu Items */
void JuMenu::Menu::Clear () {

};

/* Set MenuButtons */
void JuMenu::Menu::m_SetMenuButtons( std::string dstr ) {

};

/* Set MenuIcons */
void JuMenu::Menu::m_SetMenuIcons( std::string dstr ) {

};

/* Set MenuDropBox */
void JuMenu::Menu::m_SetMenuDropBox( std::string dstr ) {

};

/* Set DisplayBox */
void JuMenu::Menu::m_SetDisplayBox( std::string dstr ) {

};

/* Set MenuCheckBox */
void JuMenu::Menu::m_SetMenuCheckBox( std::string dstr ) {

};

/* Set DropDownMenu */
void JuMenu::Menu::m_SetDropDownMenu( std::string dstr ) {

};

/* Set InsertionBox */
void JuMenu::Menu::m_SetInsertionBox( std::string dstr ) {

};

/* Set MenuBar */
void JuMenu::Menu::m_SetMenuBar( std::string dstr ) {

};
