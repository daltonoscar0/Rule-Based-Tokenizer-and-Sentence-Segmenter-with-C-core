#pragma once
#include <string>
#include <vector>
#include "token.h"

// Segments a sequence of tokens into sentences based on sentence-ending punctuation
class SentenceSegmenter{
    public:
    // Takes tokens and groups them into sentences, returning a vector of sentence vectors
    std::vector<std::vector<Token>> segment(std::vector<Token> tokens);

    private:
    // Determines if a token marks the end of a sentence
    bool is_sentence_end(Token t);
    std::vector<std::string> abbreviations;
};