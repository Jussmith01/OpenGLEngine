#include "Engine/engine.h"
#include "Engine/States/introstate.h"
#include "Engine/States/MenuStates/mainmenustate.h"

// The MAIN function, from here we start the application and run the game loop2
int main(int argc, char *argv[])
{
    GrafixEngine engine;
    engine.Init("Engine Testing",argc,argv);

    // Set Intro State
    engine.ChangeState( IntroState::Instance() );

    if(engine.flaghandler.nosplash) {
        engine.ChangeState( MainMenuState::Instance() );
    }

    while ( engine.Running() ) {
        engine.HandleEvents();
        engine.Update();
        engine.Draw();
    }

    engine.Cleanup();

    return 0;
}

