#include "engine.h"
#include "state.h"

//***************************************
// GLFW Error Handling Callback Function
//***************************************
void glfw_error_callback(int error, const char* description)
{
    Console::cPrint("GLFW Error Detected: ");
    Console::cPrint(description);
    std::cerr << "GLFW Error Detected: " << std::endl;
    std::cerr << description << std::endl;
}

//**********************************
//Initializes the GL Engine via GLFW
//**********************************
/*
This function initializes,

init flaghandler
init glfw
init props
init glew
create audioengine

This function initializes the gl window
via the glfwCreateWindow call. All the
window properties are set after this
using the parameters set in the props
class or just using default parameters.
*/
void Engine::Init(std::string enginetitle,int argc,char *argv[])
{
    m_running=true;

    Console::cPrint("|-------------------------------------|");
    Console::cPrint(tools::appendStrings("Initializing Engine: ",enginetitle));
    std::cout << "Initializing Engine: " << enginetitle << std::endl;

    flaghandler.Init(argc,argv);

    // Init GLFW
    Console::cPrint("Initializing GLFW...");
    glfwInit();

    //Set error handling callback
    glfwSetErrorCallback(glfw_error_callback);

    //Initialize the Properties class. This loads all properties from the bin/config.cfg file.
    Console::cPrint("Loading Properties...");
    props.init();

    Console::cPrint(tools::appendStrings("Starting GLFW ",glfwGetVersionString()));
    Console::cPrint("Setting up OpenGL 3.3");
    std::cout << "Starting GLFW " << glfwGetVersionString() << " context, OpenGL 3.3" << std::endl;

    // Set all the required options for GLFW^M
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, props.MSAA);


    // Create a GLFWwindow object that we can use for GLFW's functions^M
    //window = glfwCreateWindow(props.WinWidth, props.WinHeight, "OpenGLEngine", nullptr, nullptr);

	//Initialize Window
	switch(props.FullScreen)
	{
        case 0:
        {window = glfwCreateWindow(props.WinWidth, props.WinHeight, "GameEngine Test", nullptr, nullptr);break;}
        case 1:
        {window = glfwCreateWindow(props.WinWidth, props.WinHeight, "GameEngine Test", glfwGetPrimaryMonitor(), nullptr);break;}
	}

	if( window == NULL )
	{
        Console::cPrint("Failed to open GLFW window.");
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions^M
    glewExperimental = GL_TRUE;

    // Initialize GLEW to setup the OpenGL Function pointers^M
    Console::cPrint("Initializing GLEW...");
    glewInit();

    // Define the viewport dimensions^M
    glViewport(0, 0, props.WinWidth, props.WinHeight);

    //Setup OpenGL Options
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Poly Fill Mode
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    //Initialize the console
    console.Init(&props);

    //Setup Audio Engine
    audioengine = createIrrKlangDevice();
    if (!audioengine)
	{
		Console::cPrint("Could not start audio engine...");
	} else {
        Console::cPrint("Irrklang audio engine started...");
	}
    Console::cPrint("|-------------------------------------|",true);
};

//**********************************
//  Final Cleanup of States and GL
//**********************************
/*
This function initializes,

init flaghandler
init glfw
init props
init glew
create audioengine

This function initializes the gl window
via the glfwCreateWindow call. All the
window properties are set after this
using the parameters set in the props
class or just using default parameters.
*/
void Engine::Cleanup()
{
    Console::cPrint(true,"Engine Cleanup...");
    std::cout << "Engine Cleanup..." << std::endl;

	// cleanup all states
	while ( !states.empty() ) {
        Console::cPrint(tools::appendStrings(" -Popping state: ",states.back()->stateID));
		states.back()->Cleanup();
		states.pop_back();
	}

    //Terminate Audio
    audioengine->drop();

    //Terminate Console
    Console::cPrint("Console Cleanup...");
    console.Clear();

    std::cout << "Exiting..." << std::endl;
    //Terminate GLFW
    glfwTerminate();
};

//****************************************
//Deletes current state and loads the next
//****************************************
void Engine::ChangeState(State* state)
{
	// Cleanup the current states
	while ( !states.empty() ) {
        Console::cPrint(tools::appendStrings(" -Popping state: ",states.back()->stateID));
		states.back()->Cleanup();
		states.pop_back();
	}

	// Store and Init the new state
	states.push_back(state);
	states.back()->Init(this);

    Console::cPrint(tools::appendStrings(" -Changing to state: ",states.back()->stateID));
};

//*****************************************
//Pause the current state and load the next
//*****************************************
void Engine::PushState(State* state)
{
	// pause current state
	if ( !states.empty() ) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);

    Console::cPrint(tools::appendStrings(" -Pushing to state: ",states.back()->stateID));
};

//******************************
//Pop back to the previous state
//******************************
void Engine::PopState()
{
	// cleanup the current state
	if ( !states.empty() ) {
        Console::cPrint(tools::appendStrings(" -Popping state: ",states.back()->stateID));
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
        Console::cPrint(tools::appendStrings(" -Resuming state: ",states.back()->stateID));
		states.back()->Resume(this);
	}
};

//******************************
//Handle current state events
//******************************
void Engine::HandleEvents()
{
	// let the state handle events
	states.back()->HandleEvents(this);
};

//******************************
//Update current states data
//******************************
void Engine::Update()
{
	// let the state update the game
	states.back()->Update(this);
};

//**********************************
//Draw current states data to window
//**********************************
void Engine::Draw()
{
	// let the state draw the screen
	states.back()->Draw(this);
	console.Draw();

    // Swap the screen buffers or NULL to remove the currently set callback.
    glfwSwapInterval(1); //Vsync this state
    glfwSwapBuffers(window);
};
