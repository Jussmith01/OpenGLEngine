#ifndef ENGINE_M
#define ENGINE_M

#include "../Headers/headerscpp.h"
#include "../Headers/headersogl.h"
#include "Loaders/properties.h"
#include "Tools/flaghandler.h"
#include "Tools/console.h"
#include "Tools/tools.hpp"

#include <irrKlang.h>
#include <ik_ISound.h>
using namespace irrklang;

class State;

//*************************************************
//                 Engine Class
//*************************************************
class GrafixEngine {
public:
    void Init(std::string enginetitle,int argc,char *argv[]);
    void Cleanup();

    void ChangeState(State* state);
    void PushState(State* state);
    void PopState();

    void HandleEvents();
    void Update();
    void Draw();

    bool Running() {
        return m_running;
    };

    void Quit() {
        m_running = false;
    };

    GLFWwindow* window;
    GLFWwindow* resource_context;

    ISoundEngine* audioengine;
    Properties props;
    FlagHandler flaghandler;
    Console console;

private:
    // the stack of states
    std::vector<State*> states;

    bool m_running;
    bool m_fullscreen;
};

#endif
