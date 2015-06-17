#include "worldbuilderstate.h"

WorldBuilderState WorldBuilderState::m_WorldBuilderState;
//EventHandlingStateBase * EventHandlingStateBase::event_handling_instance;

void WorldBuilderState::Init(Engine *game)
{
    swidth=game->props.WinWidth;
    sheight=game->props.WinHeight;
    bID=0;

    input.Init(game);
    input.SetMouseGLFW();

    // Setup Screen Writer
    SW.Setup("../Fonts/FreeSans.ttf",game->props.WinWidth,game->props.WinHeight,game->props.FontSize);

    // Setup Cursor
    cursor.Init("Cursors/testcursor.png",0.05f,0.05f,game->props.WinWidth,game->props.WinHeight);

    // glfwSetKeyCallback(window, key_callback);
    setEventHandling();

    glfwSetKeyCallback(game->window, EventHandlingStateBase::keycallback_dispatch);
    glfwSetMouseButtonCallback(game->window, EventHandlingStateBase::mousebuttoncallback_dispatch);
    glfwSetCursorPosCallback(game->window, EventHandlingStateBase::mouseposcallback_dispatch);
    glfwSetScrollCallback(game->window, EventHandlingStateBase::scrollcallback_dispatch);
    glfwSetCursorEnterCallback(game->window, EventHandlingStateBase::mouseentercallback_dispatch);

    dt=glfwGetTime();

    // Disable Cursor
    // glfwSetInputMode(game->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Save pointer to the audio engine
    audioengine = game->audioengine;

    // Setup Buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",1.0,game->props,audioengine);
    //buttons.SetFontColors(glm::vec3 normal,glm::vec3 hover,glm::vec3 press);

    // Define New Menu Buttons
    buttons.DefineNewButton("Continue",-0.8f, 0.45f,true);
    buttons.DefineNewButton("Main Menu",-0.8f,0.15f,false);
    buttons.DefineNewButton("Exit",-0.8f,-0.15f,false);

    // Setup Menu Box
    menubox.Init(0.12,0.41,0.005,game->props);
    menubox.SetColors(glm::vec4(0.5,0.5,0.0,0.5),glm::vec4(0.4,0.6,0.0,1.0));

    // Setup Request interface class
    reqiface.Init("Are You Sure?","Yes","No",game,audioengine);
    reqiface.SetTimeout(5.0,0);

    // Setup the World Builder Wrapper class.
    worldbuilder.Init(game);
};

void WorldBuilderState::Cleanup()
{
    cursor.Cleanup (); // Cleanup Cursor
    buttons.Cleanup(); // Cleanup buttons
    menubox.Cleanup(); // Cleanup the menubox
    SW.Cleanup(); // Cleanup screen writer
    worldbuilder.Cleanup(); // Cleanup World Builder
};

void WorldBuilderState::Pause()
{

};

void WorldBuilderState::Resume(Engine* game)
{

};

void WorldBuilderState::HandleEvents(Engine* game)
{
    glfwPollEvents();

    // Update the button events
    if (bID==-1)
    {
        bID=buttons.UpdateButtonEvents(input);
    }

    if (bID==0)
    {
        //THIS WILL BE THE WORLD BUILDER EVENT UPDATE FUNCTION
        worldbuilder.UpdateEvents(input);
    }

    if (bID==1)
    {
        //THIS IS RETURN TO MAIN MENU
        int reqID=reqiface.Update(input);

        if(reqID==0)
        {
            game->ChangeState( MainMenuState::Instance() );
        }

        if(reqID==1 || kh.CheckKeyState(input.GetKey(GLFW_KEY_ESCAPE),GLFW_KEY_ESCAPE))
        {
            buttons.SetActiveButton(1);
            bID=-1;
        }
    }

    if (bID==2)
    {
        //THIS IS EXIT PROGRAM
        int reqID=reqiface.Update(input);

        if(reqID==0)
        {
            game->Quit();
        }

        if(reqID==1 || kh.CheckKeyState(input.GetKey(GLFW_KEY_ESCAPE),GLFW_KEY_ESCAPE))
        {
            buttons.SetActiveButton(2);
            bID=-1;
        }
    }

    if (kh.CheckKeyState(input.GetKey(GLFW_KEY_ESCAPE),GLFW_KEY_ESCAPE))
    {
        if (bID==-1)
        {
            bID=0;
        }
        else
        {
            bID=-1;
        }
    }
};

void WorldBuilderState::Update(Engine* game)
{
    dt=glfwGetTime()-dt;

    // Update the world builder
    if (bID==0)
    {
        worldbuilder.UpdateData();
    }


};

void WorldBuilderState::Draw(Engine* game)
{
    // Render
    // Clear the color buffer and depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

    // WorldBuilder Draw Call
    worldbuilder.Draw();

    // Draw Menus
    SW.RenderTextCentered("WorldBuilder",1,0.0,1,0.92,1.5,glm::vec3(1.0));

    if (bID==-1)
    {
        menubox.DrawBoxPos(-0.8,0.15);
        buttons.DrawButtons();
    }

    if (bID==1 || bID==2)
    {
        reqiface.Draw();
    }

    //Draw Cursor
    cursor.DrawImagePos(input.mx,input.my);

    // Swap the screen buffers and flush
    glfwSwapBuffers(game->window);
    glFlush();
};

//Function handles key callbacks
void WorldBuilderState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
        input.SetKeyTrue(key);
    else if(action == GLFW_RELEASE)
        input.SetKeyFalse(key);
};

//Function handles mouse button callbacks
void WorldBuilderState::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        input.SetMouseKeyTrue(button);
    else if(action == GLFW_RELEASE)
        input.SetMouseKeyFalse(button);
};

//Function handles mouse movement callbacks
void WorldBuilderState::mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Save mouse position for rendering
    input.SetMousePosPDC(xpos,ypos); // Set mouse pos in input struct.
};

//Function handles mouse scroll callbacks
void WorldBuilderState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //std::cout << "SCROLL x: " << xoffset << " y: " << yoffset << std::endl;
    input.SetScrollPos(xoffset,yoffset); // Set scroll pos in input struct
};

//Function handles mouse enter and exit
void WorldBuilderState::cursor_enter_callback(GLFWwindow* window, int entered)
{

};
