#ifndef HELPER_MINESWEEPER_FUNCTIONS
#define HELPER_MINESWEEPER_FUNCTIONS

#include <vector>
#include <string>

// helper functions without a class

namespace helper_minesweeper {

struct pair_compare {
    bool operator()(const std::pair<std::string,std::string>& lhs,
                    const std::pair<std::string,std::string>& rhs);
    
};

void rtrim(std::string &s);
bool file_exists(const std::string& name);
std::vector<std::string> split_string(const std::string& input, 
                                      const char delimiter);
    
} // namespace helper_minesweeper

#endif // HELPER_MINESWEEPER_FUNCTIONS