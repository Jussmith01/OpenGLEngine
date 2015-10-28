#ifndef SETTINGMENUSTATE_H
#define SETTINGMENUSTATE_H

#include "../../engine.h"
#include "../../state.h"
#include "../../Loaders/shader.h"
#include "../../Tools/screenwriter.h"
#include "../../Tools/displayimage.h"
#include "../../Tools/MenuItems/menubutton.h"
#include "../introstate.h"
#include "../../../EventStateBase.h"
#include "../../Tools/tools.hpp"
#include "../../Tools/input_struct.h"
#include "mainmenustate.h"
#include "settingsmenustate.h"

// Graphics Properties Interface
#include "../../Interfaces/MenuInterfaces/graphicspropertiesinterface.h"

class SettingsMenuState : public State,public EventHandlingStateBase {
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

    bool GPIactive;

    //********************
    //    State Tools
    //********************
    ScreenWriter SW;
    ImageDisplay cursor;
    MenuButtons buttons;
    ISoundEngine* audioengine;
    keyhandler kh;
    GraphicsPropertiesInterface GPI;

    //********************
    //   State Instance
    //********************
    static SettingsMenuState m_SettingsMenuState;

public:
    //***********************
    //    State Functions
    //***********************
    void Init(GrafixEngine* game);
    void Cleanup();

    void Pause();
    void Resume(GrafixEngine* game);

    void HandleEvents(GrafixEngine* game);
    void Update(GrafixEngine* game);
    void Draw(GrafixEngine* game);

    static SettingsMenuState* Instance()
    {
        return &m_SettingsMenuState;
    }

protected:

    SettingsMenuState() {};

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
