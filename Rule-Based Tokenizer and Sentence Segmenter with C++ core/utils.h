#pragma once
#include <string>
#include <vector> 

// Utility functions for string manipulation (declarations only - implementations not provided)
class Utils {
    public:
    std::string to_lower(std::string s);
    std::string trim(std::string s);
    bool is_number(std::string s);
    bool is_punct(std::string s);
};