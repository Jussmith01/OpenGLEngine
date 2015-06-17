#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "../state.h"
#include "../Loaders/shader.h"
#include "../Tools/screenwriter.h"
#include "../Tools/displayimage.h"
#include "MenuStates/mainmenustate.h"
#include "../../EventStateBase.h"

class IntroState : public State,public EventHandlingStateBase
{
private:
    //*****************
    // State Variables
    //*****************
    Shader S1;
    ImageDisplay image;

    GLuint VBO, VAO, EBO;
    bool EntChk;

    //*************
    // State Tools
    //*************
    ScreenWriter SW;

    //********************
    // Call Back Handling
    //********************

    //Functions
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    //*********************
    // IntroState Instance
    //*********************
    static IntroState m_IntroState;

public:
    //*****************
    // State Functions
    //*****************
    /*
    The Engine virtual functions inherit
    these functions for every state.
    */
    void Init(Engine* game);
    void Cleanup();

    void Pause();
    void Resume(Engine* game);

    void HandleEvents(Engine* game);
    void Update(Engine* game);
    void Draw(Engine* game);

    static IntroState* Instance()
    {
        return &m_IntroState;
    }

protected:

    IntroState() {};
};

#endif
