#include "sentence_segmenter.h"
#include <vector>
#include <string>

// Groups tokens into sentences by identifying sentence-ending punctuation
std::vector<std::vector<Token>> SentenceSegmenter::segment(std::vector<Token> tokens){
    std::vector<std::vector<Token>> sentences;
    std::vector<Token> current_sentence;

    // Iterate through tokens, building sentences until a sentence end is found
    for (int i = 0; i < tokens.size(); i++){
        Token t = tokens[i];
        current_sentence.push_back(t);

        if (is_sentence_end(t)){
            sentences.push_back(current_sentence);
            current_sentence.clear();
        }
    }

    // Add any remaining tokens as the final sentence
    if (current_sentence.size() > 0){
        sentences.push_back(current_sentence);
    }
    return sentences;
}

// Checks if a token indicates the end of a sentence (period, exclamation, question mark)
bool SentenceSegmenter::is_sentence_end(Token t){
    if (t.type == TokenType::SENTENCE_END){
        return true;
    }

    if (t.type == TokenType::PUNCT){
        if (t.text == "." || t.text == "!" || t.text == "?"){
            return true;
        }
    }
    return false;
}

