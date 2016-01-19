#include "menubutton.h"

#include <regex>

//************************************
// Menu Button Initilization Function
//************************************
/*
Initialize the menu button class.

PARAMETERS:

1) The first parameter is the naming scheme for the
button image in string. For instance, this parameter
would be "button/imgfilename" if your buttons
image files were stored in bin/Data/Images/button/
and the file names were,

normal: imgfilename_normal.png
hover/active: imgfilename_hover.png
press and hold: imgfilename_press.png

2) The second is the same as before but for sounds
These are stored in in bin/Data/Sounds/

Change active state(hover): soundfilename_hover.png
Press: soundfilename_press.png

If NULL, no sound is produced.

3) Button font to use for text. If NULL no text is used.

4) Scale of the button. 10.0 is the image stretched over the screen.

5 and 6) the screen width and height.
*/
void JuMenu::MenuButtons::Init(std::string buttonimage,std::string buttonsound,std::string font,
               float scale,float x, float y,float spread,std::string orient) {
    // Define needed variables
    this->swidth=pmrm->getProps()->WinWidth;
    this->sheight=pmrm->getProps()->WinHeight;

    activebutton=-1;
    hoverbutton=-1;

    this->orient = orient;
    this->spread = spread;

    this->x = x;
    this->y = y;

    this->scale = scale;

    std::stringstream fn1,fn2,fn3; //File names for images
    fn1 << "imageloader:" << buttonimage << "_normal.png";
    fn2 << "imageloader:" << buttonimage << "_hover.png";
    fn3 << "imageloader:" << buttonimage << "_press.png";

    image1 = pmrm->requestResource(fn1.str());
    image2 = pmrm->requestResource(fn2.str());
    image3 = pmrm->requestResource(fn3.str());

    std::stringstream fnt; //File names for images
    fnt << "writerresource:" << buttonimage;

    //swrite = mrm->requestResource(fnt.str());

    // Setup button images on GPU
    //float is=scale*0.1;

    //image[0].Init(fn1.str(),scale*is,scale*is,swidth,sheight);
    //image[1].Init(fn2.str(),scale*is,scale*is,swidth,sheight);
    //image[2].Init(fn3.str(),scale*is,scale*is,swidth,sheight);

    //image[0].Init(fn1.str(),is,is,swidth,sheight);
    //image[1].Init(fn2.str(),is,is,swidth,sheight);
    //image[2].Init(fn3.str(),is,is,swidth,sheight);

    //Obtain Image Border
    //lw=image[0].GetImageHalfLength();
    //lh=image[0].GetImageHalfHeight();

    //std::cout << "Image Width: " << lw << std::endl;

    // Setup test writer on GPU
    //text.Setup(font,swidth,sheight,pow(props.FontSize,3.0)*5.0*scale);
    //text.Setup(font,swidth,sheight,props.FontSize);

    // Set Default Colors
    colors[0]=glm::vec3(1.0f);
    colors[1]=glm::vec3(1.0f);
    colors[2]=glm::vec3(1.0f);

    std::stringstream s1,s2;
    s1 << "../Data/Sounds/" << buttonsound << "_hover.wav";
    s2 << "../Data/Sounds/" << buttonsound << "_press.wav";
    soundactive = s1.str();
    soundpress = s2.str();
};

//************************************
//      Set Text Colors Function
//************************************
/*
Change the color of the text under certain conditions.
The parameters are self explainatory.
*/
void JuMenu::MenuButtons::SetFontColors(glm::vec3 normal,glm::vec3 hover,glm::vec3 press) {
    colors[0]=normal;
    colors[1]=hover;
    colors[2]=press;
};

//************************************
//     Draw the buttons Function
//************************************
/*
This function carries out all draw calls to draw
the buttons to the screen.
*/
void JuMenu::MenuButtons::draw() {
    for (int i=0; i<(int)button.size(); ++i) {
        // Variables
        //float x=button[i].x;
        //float y=button[i].y;
        int bs=button[i].state;
        std::string cap=button[i].caption;

        // Draw
        //std::cout << "BUTTON " << i<< "x: " << x << " y: " << y << std::endl;
        switch (bs) {
            case 0: {
                image1.get()->draw();
                //text.RenderTextCentered(cap,true,x,true,y,1.0,colors[bs]);
                break;
            }
            case 1: {
                image1.get()->draw();
                //text.RenderTextCentered(cap,true,x,true,y,1.0,colors[bs]);
                break;
            }
            case 2: {
                image1.get()->draw();
                //text.RenderTextCentered(cap,true,x,true,y,1.0,colors[bs]);
                break;
            }
        }

    }
};

//************************************
//    Cleanup When Destroying
//************************************
/*
Cleanup after finished using the buttons
*/
void JuMenu::MenuButtons::cleanup() {
    //image[0].Cleanup();
    //image[1].Cleanup();
    //image[2].Cleanup();
    //text.Cleanup();
    button.clear();
};

//************************************
//   Adds a new button to the Class
//************************************
/*
This function adds a new button into the class
at screen position x,y in normalized device
coordinates with a caption that reads "caption".
The active parameter set to true sets this button
as the default active button when these buttons are
printed to the screen.
*/
void JuMenu::MenuButtons::DefineNewButton(std::string ident,std::string caption,std::string action,std::string active) {
    unsigned n(button.size());

    float xpos,ypos;

    if        (orient.compare("up")) {
        xpos=x;
        ypos=y+n*spread;
    } else if (orient.compare("down")) {
        xpos=x;
        ypos=y-n*spread;
    } else if (orient.compare("left")) {
        xpos=x-n*spread;
        ypos=y;
    } else if (orient.compare("right")) {
        xpos=x+n*spread;
        ypos=y;
    } else {
        std::cout << "Error: bad orientation selection!" << std::endl;
    }

    //tools::pixel_device_coordinate(xpos,ypos,swidth, sheight);

    std::cout << "SAVING BUTTON XPOS: " << xpos << " YPOS: " << ypos << std::endl;

    Button Btemp(ident,caption,action,xpos,ypos);
    button.push_back(Btemp);
    if ( (active.compare("yes")==0) ? true : false ) {
        int ID=button.size()-1;
        SetActiveButton(ID);
    }
};

//************************************
//      Check Button States
//************************************
/*

*/
//int JuMenu::MenuButtons::update(InputStruct &input) {
void JuMenu::MenuButtons::update() {
    //std::cout << "epress: " <<  epress << " erelease: " <<  erelease << " activebutton: " << activebutton << std::endl;
    //double x,y;
    //input.ReturnMousePos(x,y);

    //bool aup=input.GetKey(GLFW_KEY_UP);
    //bool adown=input.GetKey(GLFW_KEY_DOWN);

    //aup=kh[0].CheckKeyState(aup,GLFW_KEY_UP);
    //adown=kh[1].CheckKeyState(adown,GLFW_KEY_DOWN);

    // Find out which button the mouse is over, -1 if none
    //m_checkmouseover(x,y);
    //ArrowChangeActive(aup,adown);

    //bool mpress=input.GetMouseKey(GLFW_MOUSE_BUTTON_LEFT);
    //bool epress=input.GetKey(GLFW_KEY_ENTER);

    //Check if press and hold occures and change for a button
    //m_checkpress(mpress,hoverbutton);
    //m_checkpress(epress,activebutton);

    //mpress=kh[2].CheckKeyState(mpress,GLFW_MOUSE_BUTTON_LEFT);
    //epress=kh[3].CheckKeyState(epress,GLFW_KEY_ENTER);

    //int ButtonID=-1;

    //std::cout << "ab: " << activebutton << " epress: " << epress << std::endl;
    //Tigestd::cout << "hb: " << hoverbutton << " mpress: " << mpress << std::endl;
    /*if((activebutton>=0 && epress) || (hoverbutton>=0 && mpress)) {
        lastuse=glfwGetTime();
        ISound* snd=pmrm->getAudEng()->play2D(soundpress.c_str(),false,false,true);
        while(!snd->isFinished()) {};
        ButtonID=activebutton;
    }*/

    //return ButtonID;
};

//*******************************************
//Determine if any button is in pressed state
//*******************************************
/*
This function checks for arrow up and arrow down calls to change
the active state of the buttons accordingly. Arrow down will
cycle from first to last defined and so on for arrow up.
*/
void JuMenu::MenuButtons::ArrowChangeActive(bool aup,bool adown) {
    if (aup && activebutton>0) {
        alastuse=glfwGetTime();
        int ID=activebutton-1;
        SetActiveButton(ID);
    }

    if (adown && activebutton<(int)(button.size()-1)) {
        alastuse=glfwGetTime();
        int ID=activebutton+1;
        SetActiveButton(ID);
    }
};

//*******************************************
//          Reset Button States
//*******************************************
/*
This is a tool function that resets all active states to zero.
Its typically used before setting a new active state within
this class.
*/
void JuMenu::MenuButtons::ResetButtonStates() {
    for (int j=0; j<(int)button.size(); ++j) {
        button[j].state=0;// Reset States to Default
    }
};

//*******************************************
//             Set Active Button
//*******************************************
/*
This function sets the new buttonID give as the current active button.
This is used many times throughout the class to change button active
states under certain conditions.
*/
void JuMenu::MenuButtons::SetActiveButton(int ButtonID) {
    //std::cout << soundactive.c_str() << std::endl;
    pmrm->getAudEng()->play2D(soundactive.c_str());
    ResetButtonStates();
    activebutton=ButtonID;
    button[ButtonID].state=1;
};

//************************************
//  Check if Mouse is Over a Button
//************************************
/*
This function checks if the mouse, at x,y coords,
is over the function, then sets that button
active if it is.
*/
void JuMenu::MenuButtons::m_checkmouseover(float x, float y) {
    hoverbutton=-1;

    float xmp=x;
    float ymp=y;

    //tools::normal_device_coordinate(xmp,ymp,swidth, sheight);

    bool hover=false;
    for (int i=0; i<(int)button.size(); ++i) {

        hover=button[i].CheckIfOver(xmp,ymp,lw,lh);// Determine mouse over

        if (hover) {
            //std::cout << "HOVERING: " << i << std::endl;
            hoverbutton=i;

            if (activebutton!=i) {
                SetActiveButton(i);
            }
        }
    }
};

//*******************************************
//Determine if any button is in pressed state
//*******************************************
/*

*/
void JuMenu::MenuButtons::m_checkpress(bool press,int ButtonID) {
    if(ButtonID>=0 && press) {
        button[ButtonID].state=2;// Set State to button Pressed
    }
};

//*******************************************
//Determine if any button is in pressed state
//*******************************************
/*

*/
void JuMenu::MenuButtons::m_setmenubuttons( std::string &dstr ) {
    using namespace std;

    regex pattern_object("\\s*(.+):.*\\[\\s*([^\\]]+)\\s\\]",regex_constants::optimize);

    if (regex_search(dstr,pattern_object)) {
        sregex_iterator items(dstr.begin(),dstr.end(),pattern_object);
        sregex_iterator end;
        for (; items != end; ++items)
            if (items->str(1).compare("parameters")==0) {
                m_setparameters(items->str(2));
            } else if (items->str(1).compare("tbutton")==0) {
                m_addbutton(items->str(2));
            }
    } else {
        cout << "No matching pattern found in menu script file!" << endl;
    }
};

//*******************************************
//     Set Button Parameters from String
//*******************************************
/*

*/
void JuMenu::MenuButtons::m_setparameters( std::string pstr ) {
    using namespace std;
    //cout << "SET PARAMETERS: " << pstr << endl;

    std::string imgfile,sndfile,fnt,scl,x,y,sprd,ort;

    regex pattern_object("\\s*(.+)\\s*=\\s*(.+)\\s*",regex_constants::optimize);

    if (regex_search(pstr,pattern_object)) {
        sregex_iterator items(pstr.begin(),pstr.end(),pattern_object);
        sregex_iterator end;
        for (; items != end; ++items) {
            if        (items->str(1).compare("imagefile")==0) {
                imgfile = items->str(2);
            } else if (items->str(1).compare("soundfile")==0) {
                sndfile = items->str(2);
            } else if (items->str(1).compare("font")==0) {
                fnt = items->str(2);
            } else if (items->str(1).compare("scale")==0) {
                scl = items->str(2);
            } else if (items->str(1).compare("x")==0) {
                x  = items->str(2);
            } else if (items->str(1).compare("y")==0) {
                y = items->str(2);
            } else if (items->str(1).compare("spread")==0) {
                sprd = items->str(2);
            } else if (items->str(1).compare("orient")==0) {
                ort  = items->str(2);
            }
        }

        Init(imgfile,sndfile,fnt,atof(scl.c_str()),atof(x.c_str()),atof(y.c_str()),atof(sprd.c_str()),ort);

    } else {
        cout << "No matching pattern found in menu script file!" << endl;
    }
};

//*******************************************
//     Set Button Parameters from String
//*******************************************
/*

*/
void JuMenu::MenuButtons::m_addbutton( std::string bstr ) {
    using namespace std;
    //cout << "SET BUTTON: " << bstr << endl;

    regex pattern_object("\\s*(.+)\\s*=\\s*(.+)\\s*",regex_constants::optimize);

    string ind,cap,atv,act;

    if (regex_search(bstr,pattern_object)) {
        sregex_iterator items(bstr.begin(),bstr.end(),pattern_object);
        sregex_iterator end;
        for (; items != end; ++items) {
            if        (items->str(1).compare("Ident")==0) {
                ind = items->str(2);
            } else if (items->str(1).compare("Caption")==0) {
                cap  = items->str(2);
            } else if (items->str(1).compare("Active")==0) {
                atv  = items->str(2);
            } else if (items->str(1).compare("Action")==0) {
                act  = items->str(2);
            }
        }

        DefineNewButton(ind,cap,atv,act);

    } else {
        cout << "No matching pattern found in menu script file!" << endl;
    }
};
