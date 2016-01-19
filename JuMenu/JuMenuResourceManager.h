#ifndef JUMENURESMANTEST_H
#define JUMENURESMANTEST_H

#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <unordered_map>

#include "resources/imageresource.h"
#include "resources/writerresource.h"

#include "JuMenuDeps.h"

namespace JuMenu {

/* --------------------------------
          thr() Function
---------------------------------- */
void thr_loader(std::shared_ptr<MenuResourceBase> p,std::string filename) {
    // shared use_count is incremented
    {
        //static std::mutex load_mutex;
        //std::lock_guard<std::mutex> lk(load_mutex);
        p.get()->Init(filename);
    }
};

/* --------------------------------
             Aliases
---------------------------------- */
typedef std::unordered_map< std::string,std::shared_ptr<MenuResourceBase> > resource_umap;

/* ------------------------------------
         Menu Resource Manager

Allows for dependency injection into the
primary Menu Class.
-------------------------------------- */
class MenuResourceManager {

    /* Audio Engine Pointer */
    ISoundEngine* audioengine;

    /* Properties Pointer */
    Properties* props;

    /* Maps and Stacks */
    resource_umap umap;
    std::vector<std::string> add_resourcestack;
    std::vector<std::string> rmv_resourcestack;

public:
    /* Constructor */
    MenuResourceManager () {};

    /* Destructor */
    ~MenuResourceManager () {};

    void Init(Properties* props,ISoundEngine* audioengine) {
        this->props = props;
        this->audioengine = audioengine;
    }

    /*------------------------------------
      Function for Requesting Resources
    -------------------------------------*/
    std::shared_ptr<MenuResourceBase> requestResource( std::string ident );

    /* ----------------------------------------------
     This function loads all resources in the stack
     ------------------------------------------------*/
    void manageResources( void );

    /*------------------------------------
             Print Loaded Stuff
    -------------------------------------*/
    void printLoad() {

        for (auto&& i : umap) {
            if (i.second.get()->is_loaded()) {
                std::cout << i.first << " LOADED!" << std::endl;
            } else {
                std::cout << i.first << " WAITING!" << std::endl;
            };
        }

        std::cout << "nresources: " << umap.size() << std::endl;
    };

    /* Class Access */
    const Properties*   getProps()  {return props;      };
    ISoundEngine*       getAudEng() {return audioengine;};
};
};

#endif
