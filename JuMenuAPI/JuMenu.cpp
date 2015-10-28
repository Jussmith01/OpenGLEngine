#include <iostream>
#include <fstream>

#include "JuMenu.h"
#include "JuMenuErrs.h"

/* Set the menu items based on the input file */
void JuMenu::Menu::m_SetMenuItems(std::string msfile) {
    std::ifstream filebuffer(msfile.c_str(), std::ios::in | std::ios::binary);

    if (!filebuffer) {
        JmenuThrow(std::string("Cannot open file ") + msfile);
    }

    // Load entire file into a string on memory
    std::string instr((std::istreambuf_iterator<char>(filebuffer)), std::istreambuf_iterator<char>());
    filebuffer.close();

    std::vector<std::string> sections;

    m_ParseScriptSection(sections,instr);

    std::cout << instr << std::endl;
};

/* Script Section Parser */
void JuMenu::Menu::m_ParseScriptSection(std::vector<std::string> &sections,std::string &instr) {

};

/* Set MenuDropBox */
void JuMenu::Menu::m_SetMenuDropBox(std::string dstr) {

};

/* Set DisplayBox */
void JuMenu::Menu::m_SetDisplayBox(std::string dstr) {

};

/* Set MenuCheckBox */
void JuMenu::Menu::m_SetMenuCheckBox(std::string dstr) {

};

/* Set DropDownMenu */
void JuMenu::Menu::m_SetDropDownMenu(std::string dstr) {

};

/* Set InsertionBox */
void JuMenu::Menu::m_SetInsertionBox(std::string dstr) {

};

/* Set MenuBar */
void JuMenu::Menu::m_SetMenuBar(std::string dstr) {

};

/* Set MenuButtons */
void JuMenu::Menu::m_SetMenuButtons(std::string dstr) {

};

/* Set MenuIcons */
void JuMenu::Menu::m_SetMenuIcons(std::string dstr) {

};
