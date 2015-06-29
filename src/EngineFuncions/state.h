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

    virtual void Init(Engine* game) = 0;
    virtual void Cleanup() = 0;

    virtual void Pause() = 0;
    virtual void Resume(Engine* game) = 0;

    virtual void HandleEvents(Engine* game) = 0;
    virtual void Update(Engine* game) = 0;
    virtual void Draw(Engine* game) = 0;

    void ChangeState(Engine* game, State* state)
    {
        game->ChangeState(state);
    }

protected:
    State() { }
};

#endif

