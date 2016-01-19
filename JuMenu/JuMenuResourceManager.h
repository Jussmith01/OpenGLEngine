#ifndef JUMENURESMAN_H
#define JUMENURESMAN_H

#include <string>
#include <unordered_map>

#include "resources/imageresource.h"
#include "resources/writerresource.h"

namespace JuMenu {

/* --------------------------------------
          Menu Resource Manager

Allows for dependency injection into the
primary Menu Class.

-------------------------------------- */
class MenuResourceManager {

    /* Audio Engine Pointer */
    ISoundEngine* audioengine;

    /* Properties Pointer */
    Properties* props;

    /* Resources */
    std::unordered_map<std::string,ScreenWriter> swrit; // Vector for different fonts
    std::unordered_map<std::string,ImageDisplay> image; // Vector for images

    public:
    /*Constructor*/
    MenuResourceManager() {};

    void Init (Properties* props,ISoundEngine* audioengine) {
        this->audioengine = audioengine;
        this->props = props;
    }

    /* Class Access */
    const Properties*   getProps()  {return props;      };
    ISoundEngine* getAudEng() {return audioengine;};
};
};

#endif
