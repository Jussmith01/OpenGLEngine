#ifndef WORLDBUILDERSTATE_H
#define WORLDBUILDERSTATE_H

#include "../../engine.h"
#include "../../state.h"
#include "../../Loaders/shader.h"
#include "../../Tools/screenwriter.h"
#include "../../Tools/displayimage.h"
#include "../../Tools/MenuItems.h"
#include "../../../EventStateBase.h"
#include "../../Tools/tools.hpp"
#include "../../Tools/input_struct.h"
#include "../../Interfaces/MenuInterfaces/requestinterface.h"
#include "../introstate.h"
#include "../MenuStates/mainmenustate.h"
#include "../../DevTools/worldbuilderwrapper.h"

class WorldBuilderState : public State,public EventHandlingStateBase
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
    DisplayBox menubox;
    ISoundEngine* audioengine;
    RequestInterface reqiface;

    keyhandler kh;

    //*************************
    //   World Builder Wrapper
    //*************************
    WorldBuilderWrapper worldbuilder;

    //********************
    //   State Instance
    //********************
    static WorldBuilderState m_WorldBuilderState;

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

    static WorldBuilderState* Instance()
    {
        return &m_WorldBuilderState;
    }

protected:

    WorldBuilderState() {};

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
