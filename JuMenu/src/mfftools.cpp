#include <regex>
#include "mfftools.h"

/* Bracket Split
Bracket split class
*/
void JuMenu::fileSplit(const std::string &dataIn,std::vector< std::pair<std::string,std::string> > &dataOut) {
    using namespace std;

    regex pattern_object("(.+):.*\\{\\s*([^\\}]+)\\s\\};",regex_constants::optimize);

    if (regex_search(dataIn,pattern_object)) {
        sregex_iterator items(dataIn.begin(),dataIn.end(),pattern_object);
        sregex_iterator end;
        for (; items != end; ++items)
            dataOut.push_back(make_pair<string,string>(items->str(1),items->str(2)));
    } else {
        JmenuThrow(string("No matching pattern found in menu script file!"));
    }
};

/* Bracket Split
Delimit Equals
*/
/*void JuMenu::fileSplit(const std::string &dataIn,std::unordered_map<std::string,std::string> &dataOut) {
    using namespace std;

    regex pattern_object("(.+):.*\\{\\s*([^\\}]+)\\s\\};",regex_constants::optimize);

    if (regex_search(dataIn,pattern_object)) {
        sregex_iterator items(dataIn.begin(),dataIn.end(),pattern_object);
        sregex_iterator end;
        for (; items != end; ++items)
            dataOut.insert(make_pair<string,string>(items->str(1),items->str(2)));

    } else {
        JmenuThrow(std::string("No matching pattern found in menu script file!"));
    }
};*/
