#include <iostream>
#include <fstream>

#include <regex>
#include <unordered_map>
#include "JuMenu.h"
#include "JuMenuErrs.h"

#include "menuitems/mfftools.h"

/** ****************************
         Menu Functions
***************************** **/
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

    vector< pair<string,string> > mItems;
    fileSplit(instr,mItems);

    regex pattern_menubutton("MenuButtons\\((.*)\\)",regex_constants::optimize);
    for (auto &&i : mItems) {
        smatch match;
        if (regex_search(i.first,match,pattern_menubutton)) {
            mbtn.emplace_back( match.str(1),i.second,&mrm );
        }
    }

    mItems.clear();
};

/* Update Menu Items */
void JuMenu::Menu::Update () {
    //for (auto&& m : mbtn) {

    //}
};

/* Draw Menu Items */
void JuMenu::Menu::Draw () {

};

/* Clear Menu Items */
void JuMenu::Menu::Clear () {

};
