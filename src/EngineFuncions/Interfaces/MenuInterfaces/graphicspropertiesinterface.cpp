#include "graphicspropertiesinterface.h"

//-------------------------------------------//
//    Properties Interface Class Functions   //
//-------------------------------------------//

// The following functions are for the graphics settings graphical interface
void GraphicsPropertiesInterface::InitGPI(Engine* game,ISoundEngine* audioengine)
{
    this->game=game;
    this->audioengine=audioengine;
    this->bID=-1;

    text.Setup("../Fonts/FreeSans.ttf",game->props.WinWidth,game->props.WinHeight,game->props.FontSize);
    box.Init(0.7,0.03,0.005,game->props);
    box.SetColors(glm::vec4(0.5,0.5,0.0,0.5),glm::vec4(0.4,0.6,0.0,1.0));

    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",0.5,game->props,audioengine);
    buttons.DefineNewButton("Apply",-0.15f, -0.8f,true);
    buttons.DefineNewButton("Cancel",0.15f, -0.8f,false);

    // Full Screen Mode Checkbox
    chkbox[0].Init("CheckBoxes/chkbox1","solid",0.018,game->props,audioengine);

    // Auto resolution Checkbox
    chkbox[1].Init("CheckBoxes/chkbox1","solid",0.018,game->props,audioengine);

    // Resolution Dropbox
    int currentres;
    std::vector<std::string> datares;
    GetResolutions(datares,currentres);
    drpbox[0].Init("DropBoxes/dropbox1","solid",0.57,0.5,0.25,0.02,1.0,game->props,audioengine,datares);
    drpbox[0].SetSelection(currentres);

    // MSAA Menu Dropbox
    std::vector<std::string> datamsaa;
    datamsaa.push_back("Off");
    datamsaa.push_back("x2");
    datamsaa.push_back("x4");
    datamsaa.push_back("x8");
    drpbox[1].Init("DropBoxes/dropbox1","solid",0.57,0.4,0.25,0.02,1.0,game->props,audioengine,datamsaa);

    // Apply Interface
    appInterface.Init("Restart Required.","Quit","Cancel",game,audioengine);
    appInterface.SetTimeout(15.0,0);

    // Load Set Properties from props class.
    PropertiesLoad();

    datamsaa.clear();
    datares.clear();
};

int GraphicsPropertiesInterface::UpdateGPI(InputStruct &input)
{
    int running=0;
    if (bID==-1)
    {
        bID = buttons.UpdateButtonEvents(input);
        chkbox[0].UpdateCheckBoxEvents(input);
        chkbox[1].UpdateCheckBoxEvents(input);
        drpbox[0].UpdateDropBoxEvents(input);
        drpbox[1].UpdateDropBoxEvents(input);
    }

    if (bID==0)
    {
        int aID=appInterface.Update(input);

        if (aID==0)
        {
            bID=-1;
            PropertiesSave();
            running=2;
        }

        if (aID==1)
        {
            buttons.SetActiveButton(0);
            bID=-1;
        }
    }

    if (bID==1)
    {
        running=1;
        bID=-1;
    }

    return running;
};

void GraphicsPropertiesInterface::PropertiesSave()
{
    std::stringstream *ss;

    ss = new std::stringstream [5];

    ss[0] << chkbox[0].CheckedState(); // Fullscreen
    game->props.FullScreen=chkbox[0].CheckedState(); // Fullscreen

    ss[1] << chkbox[1].CheckedState(); // Auto-resolution
    game->props.ResAuto=chkbox[1].CheckedState();

    ss[2] << rwidth[drpbox[0].SelectionState()]; // Auto-resolution
    game->props.WinWidth=rwidth[drpbox[0].SelectionState()];

    ss[3] << rheight[drpbox[0].SelectionState()]; // Auto-resolution
    game->props.WinHeight=rheight[drpbox[0].SelectionState()];

    ss[4] << MSAAMap(drpbox[1].SelectionState(),true); // Auto-resolution
    game->props.MSAA=MSAAMap(drpbox[1].SelectionState(),true);

    game->props.save_gcf(ss);

    if (game->props.ResAuto==0)
    {
        glfwSetWindowSize(game->window,game->props.WinWidth,game->props.WinHeight);
        glViewport(0, 0, game->props.WinWidth, game->props.WinHeight);
    }

    glfwWindowHint(GLFW_SAMPLES, game->props.MSAA);
    glEnable(GL_MULTISAMPLE);

    delete [] ss;
};

void GraphicsPropertiesInterface::PropertiesLoad()
{
    chkbox[0].Set((bool)game->props.FullScreen);
    chkbox[1].Set((bool)game->props.ResAuto);

    drpbox[1].SetSelection(MSAAMap(game->props.MSAA,false));
};

void GraphicsPropertiesInterface::DrawGPI()
{
    // Render GPI Title
    text.RenderTextCentered("Graphics Settings",1,0.0,1,0.82,1.2,glm::vec3(1.0));

    // Draw the buttons to screen
    buttons.DrawButtons();

    // Full screen mode Checkbox
    box.DrawBoxPos(0.15,0.7);
    text.RenderTextLeftJustified("Fullscreen Mode: ",-0.54,0.7,1.0f,glm::vec3(1.0f));
    chkbox[0].DrawCheckBox(0.82,0.7);

    // Full screen mode Checkbox
    box.DrawBoxPos(0.15,0.6);
    text.RenderTextLeftJustified("Automatic Resolution Mode: ",-0.54,0.6,1.0f,glm::vec3(1.0f));
    chkbox[1].DrawCheckBox(0.82,0.6);

    // MSAA
    box.DrawBoxPos(0.15,0.4);
    text.RenderTextLeftJustified("MSAA (Anti-aliasing): ",-0.54,0.415,1.0f,glm::vec3(1.0f));
    drpbox[1].DrawDropBox();

    // Resolution
    box.DrawBoxPos(0.15,0.5);
    std::stringstream ss;
    ss << "Resolution (" << AspectString() << "): ";
    text.RenderTextLeftJustified(ss.str(),-0.54,0.51,1.0f,glm::vec3(1.0f));
    drpbox[0].DrawDropBox();

    if(bID==0)
    {
        appInterface.Draw();
    }
};

void GraphicsPropertiesInterface::CleaupGPI()
{
    chkbox[0].Cleanup();
    chkbox[1].Cleanup();

    buttons.Cleanup();

    drpbox[0].Cleanup();
    drpbox[1].Cleanup();

    rwidth.clear();
    rheight.clear();

    box.Cleanup();
    text.Cleanup();

    appInterface.Cleaup();
};

int GraphicsPropertiesInterface::MSAAMap(int idx,bool inv)
{
    int val=0;

    if(!inv)
    {
        switch(idx)
        {
        case 0: {val=0;break;}
        case 2: {val=1;break;}
        case 4: {val=2;break;}
        case 8: {val=3;break;}
        }
    }
    else
    {
        switch(idx)
        {
        case 0: {val=0;break;}
        case 1: {val=2;break;}
        case 2: {val=4;break;}
        case 3: {val=8;break;}
        }
    }

    return val;
};

class vidchk
{
    std::vector<int> vw;
    std::vector<int> vh;

public:
    vidchk() {};

    bool CheckUsed(int w, int h)
    {
        bool rtn=true;

        int K=vw.size();
        for (int i=0; i<K; ++i)
        {
            if(vw[i]==w && vh[i]==h)
            {
                rtn=false;
            }
        }

        if (rtn)
        {
            vw.push_back(w);
            vh.push_back(h);
        }

        return rtn;
    };

};

void GraphicsPropertiesInterface::GetResolutions(std::vector<std::string> &res,int &current)
//void GraphicsPropertiesInterface::GetResolutions()
{
    vidchk vc;

    GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
    if (Monitor)
    {
        int K;
        const GLFWvidmode* Mode = glfwGetVideoModes(Monitor,&K);
        std::cout << "Number of Detected Video Modes: " << K << std::endl;
        for (int i=0; i<K; ++i)
        {
            int w=Mode[i].width;
            int h=Mode[i].height;

            if (w>800 && h>600)
            {
                //std::cout << "w: " << w << " h: " << h << " aspdiff: " << fabs((w/(float)h)-game->props.aspectratio) << std::endl;
                if(vc.CheckUsed(w,h) && fabs((w/(float)h)-game->props.aspectratio)<1.0E-1)
                {
                    rwidth.push_back(w);
                    rheight.push_back(h);
                }
            }
            //std::cout << " " << i << ") " << Mode[i].width << "x" << Mode[i].height << " @ " << Mode[i].refreshRate << "Hz" << std::endl;
        }
    }
    else
    {
        std::cout << "Error Obtaining Primary Monitor.\n";
    };

    current=0;
    for (int i=0; i<(int)rwidth.size(); ++i)
    {
        std::stringstream ss;
        ss << rwidth[i] << " x " << rheight[i];
        std::cout << " " << i << ") " << rwidth[i] << "x" << rheight[i] << " RATIO: " << rwidth[i]/(float)rheight[i] << std::endl;
        res.push_back(ss.str());
        if (rwidth[i]==game->props.WinWidth && rheight[i]==game->props.WinHeight)
        {
            current=i;
        }
        //std::cout << " " << i << ") " << rwidth[i] << "x" << rheight[i] << " ASP: " << rwidth[i]/(float)rheight[i] << std::endl;
    }
};

std::string GraphicsPropertiesInterface::AspectString()
{
    std::string rtnstr;

    if ((game->props.aspectratio-1.6)<1.0E-1)
    {
        rtnstr="16:10";
    }
    else if ((game->props.aspectratio-1.7777)<1.0E-1)
    {
        rtnstr="16:9";
    }
    else if ((game->props.aspectratio-1.25)<1.0E-1)
    {
        rtnstr="5:4";
    }
    else if ((game->props.aspectratio-1.3333)<1.0E-1)
    {
        rtnstr="4:3";
    }
    else
    {
        rtnstr=" Unknown Aspect Ratio";
    }

    return rtnstr;
}

