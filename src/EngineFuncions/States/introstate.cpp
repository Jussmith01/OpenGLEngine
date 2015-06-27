#include "../engine.h"
#include "../state.h"
#include "introstate.h"


IntroState IntroState::m_IntroState;
//IntroState *IntroState::event_handling_instance;
EventHandlingStateBase * EventHandlingStateBase::event_handling_instance;
//IntroEventStateBase * IntroEventStateBase::event_handling_instance;

void IntroState::Init(Engine *game)
{
    EntChk=false;

    //Disable Cursor
    glfwSetInputMode(game->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    SW.Setup("../Fonts/FreeSans.ttf",game->props.WinWidth,game->props.WinHeight,game->props.FontSize);
    image.Init("Splash/PLANSYS.png",1.0f,1.0f,game->props.WinWidth,game->props.WinHeight);

    //glfwSetKeyCallback(window, key_callback);
    setEventHandling();
    //glfwSetKeyCallback(game->window, EventStateBase::keycallback_dispatch);
    glfwSetKeyCallback(game->window, EventHandlingStateBase::keycallback_dispatch);
};

void IntroState::Cleanup()
{
    SW.Cleanup();
    image.Cleanup();
};

void IntroState::Pause()
{

};

void IntroState::Resume(Engine* game)
{

};

void IntroState::HandleEvents(Engine* game)
{
    glfwPollEvents();

    // Move to next state after 5 seconds
    if (glfwGetTime() > 5.0)
    {
        game->ChangeState( MainMenuState::Instance() );
    };

    // Move to next state if enter is pressed
    if (EntChk)
    {
        game->ChangeState( MainMenuState::Instance() );
    };
};

void IntroState::Update(Engine* game)
{

};

void IntroState::Draw(Engine* game)
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

    // Image Rendering
    image.DrawImage();

    // Text Rendering
    std::stringstream time;
    time << (int)glfwGetTime();
    SW.RenderTextCentered(time.str(),1,1.0,1,1.0,1,glm::vec3(1.0));
    SW.RenderTextCentered("Press Space to Continue",1,0.0,1,-0.85,1,glm::vec3(1.0));

    // Swap the screen buffers or NULL to remove the currently set callback.
    //glfwSwapInterval(1); //Vsync this state
    //glfwSwapBuffers(game->window);
};

//Function handles key callbacks
void IntroState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        EntChk=true;
};

//Function handles mouse button callbacks
void IntroState::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

};

//Function handles mouse movement callbacks
void IntroState::mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{

};

//Function handles mouse scroll callbacks
void IntroState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

};

//Function handles mouse enter and exit
void IntroState::cursor_enter_callback(GLFWwindow* window, int entered)
{

};
