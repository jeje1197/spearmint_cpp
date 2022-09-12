#ifndef REGEXFILE_H_INCLUDED
#define REGEXFILE_H_INCLUDED

#include <regex>
using std::regex, std::smatch, std::string, std::regex_search;

void regexTest(string s) {
    regex myreg("[a-z][+][a-z]");
    smatch matches;

    regex_search(s, matches, myreg);

    std::cout << "Is there a match? " << matches.ready() << std::endl;
    std::cout << "Match: " << matches.str(0) << std::endl;
}


#endif // REGEXFILE_H_INCLUDED
