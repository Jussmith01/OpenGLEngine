#ifndef TERRAINGENERATORSTATE_H
#define TERRAINGENERATORSTATE_H

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
#include "../../DevTools/terraingeneratorwrapper.h"

class TerrainGeneratorState : public State,public EventHandlingStateBase {
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
    TerrainGeneratorWrapper terraingenerator;

    //********************
    //   State Instance
    //********************
    static TerrainGeneratorState m_TerrainGeneratorState;

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

    static TerrainGeneratorState* Instance()
    {
        return &m_TerrainGeneratorState;
    }

protected:

    TerrainGeneratorState() {};

private:

    //******************************
    //    Event Call Back Handling
    //******************************
    //Virtual Functions from State class
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void cursor_enter_callback(GLFWwindow* window, int entered);
};

#endif
