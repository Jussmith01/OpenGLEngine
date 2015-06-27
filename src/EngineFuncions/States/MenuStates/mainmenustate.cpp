#include "mainmenustate.h"

MainMenuState MainMenuState::m_MainMenuState;
//EventHandlingStateBase * EventHandlingStateBase::event_handling_instance;

void MainMenuState::Init(Engine *game)
{
    swidth=game->props.WinWidth;
    sheight=game->props.WinHeight;
    bID=-1;

    input.Init(game);

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
    //Disable Cursor
    //glfwSetInputMode(game->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //Save point to the audio engine
    audioengine=game->audioengine;

    // Setup Buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",1.0,game->props,audioengine);

    // Define New Menu Buttons
    buttons.DefineNewButton("Tools",-0.8f, 0.45f,true);
    buttons.DefineNewButton("Settings",-0.8f, 0.15f,false);
    buttons.DefineNewButton("About",-0.8f, -0.15f,false);
    buttons.DefineNewButton("Exit",-0.8f, -0.45f,false);

    // Setup the exit interface
    exitInterface.Init("Exit?","Yes","No",game,audioengine);
    exitInterface.SetTimeout(5.0,0);
};

void MainMenuState::Cleanup()
{
    cursor.Cleanup (); // Cleanup Cursor
    buttons.Cleanup(); // Cleanup buttons
    SW.Cleanup(); // Cleanup screen writer
    exitInterface.Cleaup(); // Cleanup the exit Interface
};

void MainMenuState::Pause()
{

};

void MainMenuState::Resume(Engine* game)
{
    setEventHandling();

    glfwSetKeyCallback(game->window, EventHandlingStateBase::keycallback_dispatch);
    glfwSetMouseButtonCallback(game->window, EventHandlingStateBase::mousebuttoncallback_dispatch);
    glfwSetCursorPosCallback(game->window, EventHandlingStateBase::mouseposcallback_dispatch);
    glfwSetScrollCallback(game->window, EventHandlingStateBase::scrollcallback_dispatch);
    glfwSetCursorEnterCallback(game->window, EventHandlingStateBase::mouseentercallback_dispatch);

    input.SetMouseGLFW();
    buttons.SetActiveButton(0);
    bID=-1;
};

void MainMenuState::HandleEvents(Engine* game)
{
    glfwPollEvents();

    // Update the button events
    if (bID==-1)
    {
        bID=buttons.UpdateButtonEvents(input);
    }

    if (bID==0)
    {
        game->PushState( ToolsMenuState::Instance() );
    }

    if (bID==1)
    {
        game->PushState( SettingsMenuState::Instance() );
    }

    if (bID==2)
    {
        game->PushState( AboutMenuState::Instance() );
    }

    if (bID==3)
    {
        bID=3;
        int reqID=exitInterface.Update(input);

        if(reqID==0)
        {
            game->Quit();
        }

        if(reqID==1)
        {
            buttons.SetActiveButton(3);
            bID=-1;
        }
    }
};

void MainMenuState::Update(Engine* game)
{
    dt=glfwGetTime()-dt;
};

void MainMenuState::Draw(Engine* game)
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.33f, 0.33f, 0.33f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
    // Draw our first triangle

    std::string text = "This is a pre-alpha build of a Linux and Windows multiplatform OpenGL engine.";
    SW.RenderTextBlocked(text,-0.6,0.6,1.0,game->props.LineSpace,0.5,glm::vec3(1.0));

    SW.RenderTextCentered("TEST MENU",1,0.0,1,0.9,1.5,glm::vec3(1.0));
    SW.RenderTextCentered("OpenGLEngine Vpre-A0.001",1,0.85,1,-0.9,0.6,glm::vec3(1.0));
    SW.RenderTextCentered("Press Esc to Exit",1,0.0,1,-0.9,1,glm::vec3(1.0));

    // Draw Buttons
    buttons.DrawButtons();

    // Draw Exit Menu
    if(bID==3)
    {
        exitInterface.Draw();
    }

    // Draw Mouse Cursor
    cursor.DrawImagePos(input.mx,input.my);

    // Swap the screen buffers
    //glfwSwapBuffers(game->window);
    //glFlush();
    //glFinish();
};

//Function handles key callbacks
void MainMenuState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
        input.SetKeyTrue(key);
    else if(action == GLFW_RELEASE)
        input.SetKeyFalse(key);
};

//Function handles mouse button callbacks
void MainMenuState::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        input.SetMouseKeyTrue(button);
    else if(action == GLFW_RELEASE)
        input.SetMouseKeyFalse(button);
};

//Function handles mouse movement callbacks
void MainMenuState::mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Save mouse position for rendering
    //std::cout << "MOUSE x: " << xpos << " y: " << ypos << std::endl;
    input.SetMousePosPDC(xpos,ypos); // Set mouse pos in input struct.
};

//Function handles mouse scroll callbacks
void MainMenuState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //std::cout << "SCROLL x: " << xoffset << " y: " << yoffset << std::endl;
    input.SetScrollPos(xoffset,yoffset); // Set scroll pos in input struct
};

//Function handles mouse enter and exit
void MainMenuState::cursor_enter_callback(GLFWwindow* window, int entered)
{

};
