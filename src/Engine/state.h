#ifndef STATE_M
#define STATE_M

#include "engine.h"
#include "../Headers/headerscpp.h"
#include "../Headers/headersogl.h"

//*************************************************
//                 State Class
//*************************************************
class State
{
public:
    std::string stateID;

    virtual void Init(GrafixEngine* game) = 0;
    virtual void Cleanup() = 0;

    virtual void Pause() = 0;
    virtual void Resume(GrafixEngine* game) = 0;

    virtual void HandleEvents(GrafixEngine* game) = 0;
    virtual void Update(GrafixEngine* game) = 0;
    virtual void Draw(GrafixEngine* game) = 0;

    void ChangeState(GrafixEngine* game, State* state) {
        game->ChangeState(state);
    };

protected:
    State() { };
};

#endif

