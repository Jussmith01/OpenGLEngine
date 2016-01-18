#ifndef JUMENUMFFTOOLS_H
#define JUMENUMFFTOOLS_H

#include <unordered_map>
#include "../JuMenuDeps.h"
#include "../JuMenuErrs.h"

namespace JuMenu {

/* file Split */
void fileSplit(const std::string &dataIn,std::vector< std::pair<std::string,std::string> > &dataOut);

/* Equals delimit */
//void fileSplit(const std::string &dataIn,std::unordered_map<std::string,std::string> &dataOut);


};


#endif
