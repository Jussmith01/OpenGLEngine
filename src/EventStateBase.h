#ifndef EHSB_H
#define EHSB_H


//*****************************************//
//		Event Handling State Base		   //
//*****************************************//
//This state base allows for member        //
//functions to be used as glfw callback    //
//functions.				               //
//*****************************************//
class EventHandlingStateBase
{
    //Functions
    virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)=0;
    virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)=0;
    virtual void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)=0;
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)=0;
    virtual void cursor_enter_callback(GLFWwindow* window, int entered)=0;

    static EventHandlingStateBase *event_handling_instance;
    // technically setEventHandling should be finalized so that it doesn't
    // get overwritten by a descendant class.

public:
    virtual void setEventHandling()
    {
        event_handling_instance = this;
    };

    //Key Callback Function
    static void keycallback_dispatch(GLFWwindow *window,int key,int scancode,int action,int mods)
    {
        if(event_handling_instance)
            event_handling_instance->key_callback(window,key,scancode,action,mods);
    };

    //Mouse Button Callback Function
    static void mousebuttoncallback_dispatch(GLFWwindow* window, int button, int action, int mods)
    {
        if(event_handling_instance)
            event_handling_instance->mouse_button_callback(window,button,action,mods);
    };

    //Mouse Position Callback Function
    static void mouseposcallback_dispatch(GLFWwindow* window, double xpos, double ypos)
    {
        if(event_handling_instance)
            event_handling_instance->mouse_pos_callback(window,xpos,ypos);
    };

    //Scroll Callback Function
    static void scrollcallback_dispatch(GLFWwindow* window, double xoffset, double yoffset)
    {
        if(event_handling_instance)
            event_handling_instance->scroll_callback(window,xoffset,yoffset);
    };

    //Mouse enter/exit Callback Function
    static void mouseentercallback_dispatch(GLFWwindow* window, int entered)
    {
        if(event_handling_instance)
            event_handling_instance->cursor_enter_callback(window,entered);
    };
};

#endif
