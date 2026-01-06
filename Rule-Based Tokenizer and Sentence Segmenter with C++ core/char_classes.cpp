#include "char_classes.h"
#include <cctype>

// Classifies a character by checking its type and returning the appropriate CharClass enum value
CharClass CharClassifier::classify_char(char c){
    if (std::isalpha(c)){
        return CharClass::LETTER;
    } else if (std::isdigit(c)){
        return CharClass::DIGIT;
    } else if (std::isspace(c)){
        return CharClass::WHITESPACE;
    } else if (c == '.'){
        return CharClass::PERIOD;
    } else if (c == '\''){
        return CharClass::APOSTROPHE;
    } else if (c == '-'){
        return CharClass::HYPHEN;
    } else if (std::ispunct(c)){
        return CharClass::PUNCT;
    } else {
        return CharClass::OTHER;
    }
}

