#include "settingsmenustate.h"

SettingsMenuState SettingsMenuState::m_SettingsMenuState;

void SettingsMenuState::Init(Engine *game)
{
    SettingsMenuState::stateID = "Settings Menu";

    swidth=game->props.WinWidth;
    sheight=game->props.WinHeight;
    bID=-1;

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
    //Disable Cursor
    //glfwSetInputMode(game->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //Setup State Audio Engine
    //Save point to the audio engine
    audioengine = game->audioengine;

    // Setup Buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",1.0,game->props,audioengine);
    //buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",1.0,swidth,sheight);
    //buttons.SetFontColors(glm::vec3 normal,glm::vec3 hover,glm::vec3 press);

    //Define New Menu Buttons
    buttons.DefineNewButton("Graphics",-0.8f, 0.15f,true);
    buttons.DefineNewButton("Back",-0.8f, -0.15f,false);

    // Setup GPI
    GPI.InitGPI(game,audioengine);
};

void SettingsMenuState::Cleanup()
{
    cursor.Cleanup (); // Cleanup Cursor
    buttons.Cleanup(); // Cleanup buttons
    SW.Cleanup(); // Cleanup screen writer

    GPI.CleaupGPI();// Cleanup the Graphics Properties Interface
};

void SettingsMenuState::Pause()
{

};

void SettingsMenuState::Resume(Engine* game)
{

};

void SettingsMenuState::HandleEvents(Engine* game)
{
    glfwPollEvents();

    Console::Update(kh.CheckKeyState(input.GetKey(GLFW_KEY_F12),GLFW_KEY_F12));

    // Update the button events
    if (bID==-1)
    {
        bID=buttons.UpdateButtonEvents(input);
    }

    if (bID==0)
    {
        int stay=GPI.UpdateGPI(input);
        // 1 is returned if exit is requested without reloading states
        if (stay==1 || input.GetKey(GLFW_KEY_ESCAPE) || input.GetKey(GLFW_KEY_BACKSPACE))
        {
            bID=-1;
            buttons.SetActiveButton(0);
            input.SetKeyFalse(GLFW_KEY_ESCAPE);
            input.SetKeyFalse(GLFW_KEY_BACKSPACE);
        }

        // 2 is returned if exit with reloading is requested
        if (stay==2)
        {
            game->ChangeState( MainMenuState::Instance() );
        }
    }

    if (bID==1 || input.GetKey(GLFW_KEY_ESCAPE) || input.GetKey(GLFW_KEY_BACKSPACE))
    {
        game->PopState();
    }
};

void SettingsMenuState::Update(Engine* game)
{
    dt=glfwGetTime()-dt;
};

void SettingsMenuState::Draw(Engine* game)
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
    // Draw our first triangle

    SW.RenderTextCentered("Settings Menu",1,0.0,1,0.9,1.5,glm::vec3(1.0));
    SW.RenderTextCentered("OpenGLEngine Vpre-A0.001",1,0.85,1,-0.9,0.6,glm::vec3(1.0));
    SW.RenderTextCentered("Press Esc to Exit",1,0.0,1,-0.9,1,glm::vec3(1.0));

    buttons.DrawButtons();

    if (bID==0)
    {
        GPI.DrawGPI();
    }

    cursor.DrawImagePos(input.mx,input.my);

    // Swap the screen buffers
    //glfwSwapBuffers(game->window);
    //glFlush();
};

//Function handles key callbacks
void SettingsMenuState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
        input.SetKeyTrue(key);
    else if(action == GLFW_RELEASE)
        input.SetKeyFalse(key);
};

//Function handles mouse button callbacks
void SettingsMenuState::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        input.SetMouseKeyTrue(button);
    else if(action == GLFW_RELEASE)
        input.SetMouseKeyFalse(button);
};

//Function handles mouse movement callbacks
void SettingsMenuState::mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Save mouse position for rendering
    input.SetMousePosPDC(xpos,ypos); // Set mouse pos in input struct.
};

//Function handles mouse scroll callbacks
void SettingsMenuState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //std::cout << "SCROLL x: " << xoffset << " y: " << yoffset << std::endl;
    input.SetScrollPos(xoffset,yoffset); // Set scroll pos in input struct
};

//Function handles mouse enter and exit
void SettingsMenuState::cursor_enter_callback(GLFWwindow* window, int entered)
{

};
