#include "helper.h"

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
 
bool helper_minesweeper::pair_compare::operator()(const std::pair<std::string,std::string>& lhs,
                                                  const std::pair<std::string,std::string>& rhs)
{
    if (rhs.second == "-") {return true;}
    else if (lhs.second == "-") {return false;}
    else {
        return stol(lhs.second) <= stol(rhs.second);
    }
}

void helper_minesweeper::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

bool helper_minesweeper::file_exists(const std::string& name)
{
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

std::vector<std::string> helper_minesweeper::split_string(const std::string& input, 
                                                          const char delimiter) 
{
    std::stringstream ss {input};
    std::vector<std::string> result;
    // result.reserve(count(begin(input), end(input), delimiter));
    
    for (std::string buffer; 
         getline(ss, buffer, delimiter);) 
            {result.push_back(std::move(buffer));}
    
    return result;
}

std::string helper_minesweeper::xor_crypt(const std::string& input)
{
    static const char key = 'K';
    
    std::string output {input};
    
    for (char& c: output) {
        c ^= key;
    }
    
    return output;
}
