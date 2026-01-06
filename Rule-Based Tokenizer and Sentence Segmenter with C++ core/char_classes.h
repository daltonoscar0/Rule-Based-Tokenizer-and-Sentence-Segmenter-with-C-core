#pragma once
// Enumeration of character classes used for tokenization
enum class CharClass{
    LETTER,
    DIGIT,
    WHITESPACE,
    PERIOD,
    APOSTROPHE,
    HYPHEN, 
    PUNCT,
    OTHER
};

// Classifies characters into categories for the tokenizer's finite state machine
class CharClassifier{
    public:
    // Returns the character class for a given character
    CharClass classify_char(char c);
};