#include <iostream>
#include <fstream>

#include <regex>
#include <unordered_map>
#include "JuMenuDeps.h"
#include "JuMenuErrs.h"
#include "JuMenuResourceManager.h"

/** ****************************
      Menu Resource Manager
            Functions
***************************** **/

/*------------------------------------
  Function for Requesting Resources
-------------------------------------*/
std::shared_ptr<JuMenu::MenuResourceBase> JuMenu::MenuResourceManager::requestResource( std::string ident ) {
    unsigned n( ident.find_first_of(":") );

    std::string type = ident.substr(0,n);
    resource_umap::iterator it = umap.find(ident);

    if ( it == umap.end() ) {

        if        ( type == std::string("screenwriter") ) {
            //umap.emplace(ident,std::make_shared<ScreenWriter>());
        } else if ( type == std::string("imageloader") ) {
            std::cout << "LOAD IMAGE: " << ident << std::endl;
            umap.emplace(ident,std::make_shared<ImageLoader>());
        }

        add_resourcestack.push_back(ident);

        it = umap.find(ident);
        return (*it).second;
    }

    return (*it).second;
};

/* ----------------------------------------------
 This function loads all resources in the stack
 ------------------------------------------------*/
void JuMenu::MenuResourceManager::manageResources () {
    resource_umap::iterator it;

    /* Thread out loading jobs */
    while ( !add_resourcestack.empty() ) {
        it = umap.find(add_resourcestack.back());
        add_resourcestack.pop_back();
        (*it).second->Init( (*it).first.substr((*it).first.find_first_of(":")+1) );
        //std::thread (MenuResourceManager::thr_loader,(*it).second,(*it).first.substr((*it).first.find_first_of(":")+1)).detach();
    }

    for ( auto i(umap.begin()); i != umap.end(); ++i ) {
        //std::cout << i->first << ":" << i->second.use_count() << std::endl;
        if ( i->second.use_count() == 1 ) {
            i->second.reset();
            rmv_resourcestack.push_back(i->first);
        }
    }

    while ( !rmv_resourcestack.empty() ) {
        it = umap.find(rmv_resourcestack.back());
        rmv_resourcestack.pop_back();
        umap.erase(it);
    }
};
