#ifndef JUMENUDEPS_H
#define JUMENUDEPS_H

#include "../src/Headers/headerscpp.h"
#include "../src/Headers/headersogl.h"

#include "../src/Engine/Tools/tools.hpp"
#include "../src/Engine/Tools/input_struct.h"
#include "../src/Engine/Loaders/properties.h"

#include <irrKlang.h>
#include <ik_ISound.h>

namespace JuMenu {
static float swidth;
static float sheight;

/* --------------------------------
          MenuResourceBase
---------------------------------- */
class MenuResourceBase {
    public:
    MenuResourceBase() {};

    virtual void Init( std::string filename ) = 0;
    virtual bool is_loaded() = 0;

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void cleanup() = 0;

    // Note: non-virtual destructor is OK here
    ~MenuResourceBase() {};
};
}

#endif
