#include "aboutmenustate.h"

AboutMenuState AboutMenuState::m_AboutMenuState;
//EventHandlingStateBase * EventHandlingStateBase::event_handling_instance;

void AboutMenuState::Init(Engine *game)
{
    AboutMenuState::stateID = "About Menu";

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

    std::vector<std::string> files=tools::GetFilesInDirectory("../Shaders");

    for (int i=0;i<(int)files.size();++i)
    {
            std::cout << i << ") " << files[i] <<std::endl;
    }

    std::vector< SingleBarMenuItem > smbi;
    smbi.resize(3);

    smbi[0].title="File";
    smbi[0].options.push_back("File Test 1");
    smbi[0].options.push_back("File Test 2");
    smbi[0].options.push_back("File Test 3");

    smbi[1].title="Edit";
    smbi[1].options.push_back("Edit Test 1");
    smbi[1].options.push_back("Edit Test 2");

    smbi[2].title="View";
    smbi[2].options.push_back("View Test 1");
    smbi[2].options.push_back("View Test 2");
    smbi[2].options.push_back("View Test 3");

    mbar.Init(smbi,game->props);
    smbi.clear();


    //Define New Menu Buttons
    buttons.DefineNewButton("Back",-0.8f, 0.0f,true);
};

void AboutMenuState::Cleanup()
{
    cursor.Cleanup (); // Cleanup Cursor
    buttons.Cleanup(); // Cleanup button
    SW.Cleanup(); // Cleanup screen writer

    mbar.Cleanup();
};

void AboutMenuState::Pause()
{

};

void AboutMenuState::Resume(Engine* game)
{

};

void AboutMenuState::HandleEvents(Engine* game)
{
    glfwPollEvents();

    // Update the button events
    int bID=buttons.UpdateButtonEvents(input);
    mbar.UpdateBarEvents(input);

    if (bID==0 || input.GetKey(GLFW_KEY_ESCAPE) || input.GetKey(GLFW_KEY_BACKSPACE))
    {
        game->PopState();
    }
};

void AboutMenuState::Update(Engine* game)
{
    dt=glfwGetTime()-dt;
};

void AboutMenuState::Draw(Engine* game)
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
    // Draw our first triangle

    SW.RenderTextCentered("About Menu",1,0.0,1,0.9,1.5,glm::vec3(1.0));
    SW.RenderTextCentered("OpenGLEngine Vpre-A0.001",1,0.85,1,-0.9,0.6,glm::vec3(1.0));
    SW.RenderTextCentered("Press Esc to Exit",1,0.0,1,-0.9,1,glm::vec3(1.0));

    buttons.DrawButtons();

    mbar.DrawBar();

    cursor.DrawImagePos(input.mx,input.my);

    // Swap the screen buffers
    //glfwSwapBuffers(game->window);
    //glFlush();
};

//Function handles key callbacks
void AboutMenuState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
        input.SetKeyTrue(key);
    else if(action == GLFW_RELEASE)
        input.SetKeyFalse(key);
};

//Function handles mouse button callbacks
void AboutMenuState::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        input.SetMouseKeyTrue(button);
    else if(action == GLFW_RELEASE)
        input.SetMouseKeyFalse(button);
};

//Function handles mouse movement callbacks
void AboutMenuState::mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Save mouse position for rendering
    input.SetMousePosPDC(xpos,ypos); // Set mouse pos in input struct.
};

//Function handles mouse scroll callbacks
void AboutMenuState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //std::cout << "SCROLL x: " << xoffset << " y: " << yoffset << std::endl;
    input.SetScrollPos(xoffset,yoffset); // Set scroll pos in input struct
};

//Function handles mouse enter and exit
void AboutMenuState::cursor_enter_callback(GLFWwindow* window, int entered)
{

};
