#include "toolsmenustate.h"

ToolsMenuState ToolsMenuState::m_ToolsMenuState;
//EventHandlingStateBase * EventHandlingStateBase::event_handling_instance;

void ToolsMenuState::Init(Engine *game)
{
    swidth=game->props.WinWidth;
    sheight=game->props.WinHeight;

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

    //Save pointer to the audio engine
    audioengine = game->audioengine;

    // Setup Buttons
    buttons.Init("Buttons/testbutton","solid","../Fonts/FreeSans.ttf",1.0,game->props,audioengine);
    //buttons.SetFontColors(glm::vec3 normal,glm::vec3 hover,glm::vec3 press);

    //Define New Menu Buttons
    buttons.DefineNewButton("TerrainGenerator",-0.8f, 0.45f,true);
    buttons.DefineNewButton("WorldBuilder",-0.8f, 0.15f,false);
    buttons.DefineNewButton("Back",-0.8f,-0.15f,false);
};

void ToolsMenuState::Cleanup()
{
    cursor.Cleanup (); // Cleanup Cursor
    buttons.Cleanup(); // Cleanup buttons
    SW.Cleanup(); // Cleanup screen writer
};

void ToolsMenuState::Pause()
{

};

void ToolsMenuState::Resume(Engine* game)
{

};

void ToolsMenuState::HandleEvents(Engine* game)
{
    glfwPollEvents();

    // Update the button events
    int bID=buttons.UpdateButtonEvents(input);

    if (bID==0)
    {
        // Call world builder state
        game->ChangeState( TerrainGeneratorState:: Instance() );
    }

    if (bID==1)
    {
        // Call world builder state
        game->ChangeState( WorldBuilderState:: Instance() );
    }

    if (bID==2 || input.GetKey(GLFW_KEY_ESCAPE) || input.GetKey(GLFW_KEY_BACKSPACE))
    {
        game->PopState();
    }
};

void ToolsMenuState::Update(Engine* game)
{
    dt=glfwGetTime()-dt;
};

void ToolsMenuState::Draw(Engine* game)
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.33f, 0.33f, 0.33f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
    // Draw our first triangle

    SW.RenderTextCentered("Tools Menu",1,0.0,1,0.9,1.5,glm::vec3(1.0));
    SW.RenderTextCentered("Press Esc to go Back",1,0.0,1,-0.9,1,glm::vec3(1.0));

    buttons.DrawButtons();

    cursor.DrawImagePos(input.mx,input.my);

    // Swap the screen buffers
    glfwSwapBuffers(game->window);
    glFlush();
};

//Function handles key callbacks
void ToolsMenuState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
        input.SetKeyTrue(key);
    else if(action == GLFW_RELEASE)
        input.SetKeyFalse(key);
};

//Function handles mouse button callbacks
void ToolsMenuState::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        input.SetMouseKeyTrue(button);
    else if(action == GLFW_RELEASE)
        input.SetMouseKeyFalse(button);
};

//Function handles mouse movement callbacks
void ToolsMenuState::mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Save mouse position for rendering
    input.SetMousePosPDC(xpos,ypos); // Set mouse pos in input struct.
};

//Function handles mouse scroll callbacks
void ToolsMenuState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //std::cout << "SCROLL x: " << xoffset << " y: " << yoffset << std::endl;
    input.SetScrollPos(xoffset,yoffset); // Set scroll pos in input struct
};

//Function handles mouse enter and exit
void ToolsMenuState::cursor_enter_callback(GLFWwindow* window, int entered)
{

};
