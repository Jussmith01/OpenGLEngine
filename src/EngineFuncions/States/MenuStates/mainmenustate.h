#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "../../engine.h"
#include "../../state.h"
#include "../../Loaders/shader.h"
#include "../../Tools/screenwriter.h"
#include "../../Tools/displayimage.h"
#include "../../Tools/MenuItems/menubutton.h"
#include "../../Tools/MenuItems/boxcolored.h"
#include "../../Tools/MenuItems/checkbox.h"
#include "../../Tools/MenuItems/dropbox.h"
#include "../introstate.h"
#include "../../../EventStateBase.h"
#include "../../Tools/tools.hpp"
#include "../../Tools/input_struct.h"
#include "settingsmenustate.h"
#include "aboutmenustate.h"
#include "toolsmenustate.h"
#include "../../Interfaces/MenuInterfaces/requestinterface.h"

class MainMenuState : public State,public EventHandlingStateBase
{
private:

    //********************
    //  Global Variables
    //********************
    int swidth;
    int sheight;

    //********************
    //  State Variables
    //********************
    InputStruct input;
    double dt;
    int bID;

    //********************
    //    State Tools
    //********************
    ScreenWriter SW;
    ImageDisplay cursor;
    MenuButtons buttons;
    RequestInterface exitInterface;

    ISoundEngine* audioengine;

    //********************
    //   State Instance
    //********************
    static MainMenuState m_MainMenuState;

public:
    //***********************
    //    State Functions
    //***********************
    void Init(Engine* game);
    void Cleanup();

    void Pause();
    void Resume(Engine* game);

    void HandleEvents(Engine* game);
    void Update(Engine* game);
    void Draw(Engine* game);

    static MainMenuState* Instance()
    {
        return &m_MainMenuState;
    }

protected:

    MainMenuState() {};

private:
    //******************************
    //    Event Call Back Handling
    //******************************

    //Functions
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void cursor_enter_callback(GLFWwindow* window, int entered);
};

#endif
