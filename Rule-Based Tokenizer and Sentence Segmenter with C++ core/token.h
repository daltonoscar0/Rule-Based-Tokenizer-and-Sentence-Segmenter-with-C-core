#pragma once
#include <string>

// Types of tokens that can be identified by the tokenizer
enum class TokenType{
    WORD,
    NUMBER,
    PUNCT,
    ABBREVIATION,
    CONTRACTION,
    HYPHENATED,
    SENTENCE_END,
};

// Represents a single token with its text content, type, and position in the original input
class Token {
    public:
    // Constructor that initializes a token with text, type, and position indices
    Token(std::string t, TokenType ty, int start, int end){
        text = t;
        type = ty;
        start_index = start;
        end_index = end;
    }
    
    // Default constructor creates an empty token
    Token(){
        text = " ";
        type = TokenType::WORD;
        start_index = 0;
        end_index = 0;
    }

    std::string text;
    TokenType type;
    int start_index;
    int end_index;
};