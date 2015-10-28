#include "menubar.h"

//************************************
// Menu Bar Initilization Function
//************************************
/*
Initialize the menu check box class.

PARAMETERS:

*/
void MenuBar::Init(std::vector<SingleBarMenuItem> &baritems,Properties &props)
{
    this->NM=(int)baritems.size();
    this->ddmenu.resize(this->NM);

    for(int i=0;i<this->NM;++i)
    {
        NO.push_back((int)baritems[i].options.size());
        this->ddmenu[i].Init(baritems[i].title,-0.85+i*0.15,0.975,0.3,0.05,0.5,props,baritems[i].options);
    }
};

//************************************
//          Update the Menu Bar
//************************************
/*

*/
BarSelection MenuBar::UpdateBarEvents(InputStruct &input)
{
    BarSelection select;

    for(int i=0;i<this->NM;++i)
    {
        this->ddmenu[i].UpdateEvents(input);
        int sID=this->ddmenu[i].SelectionState();
        if (sID>=0)
        {
            select.isset=true;
            select.menu=i;
            select.option=sID;
            break;
        }
    }

    return select;
};

//************************************
//          Draw the bar
//************************************
/*
This function carries out all draw calls to draw
the bar to the screen.
*/
void MenuBar::DrawBar()
{
    for(int i=0;i<this->NM;++i)
    {
        this->ddmenu[i].Draw();
    }
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the class
*/
void MenuBar::Cleanup()
{
    for(int i=0;i<this->NM;++i)
    {
        this->ddmenu[i].Cleanup();
    }
};
