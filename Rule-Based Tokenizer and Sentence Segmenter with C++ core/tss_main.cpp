#include <iostream>
#include <string>
#include <vector>

#include "tokenizer.h"
#include "sentence_segmenter.h"
#include "token.h"

// Main program: reads input, tokenizes it, segments into sentences, and prints the results
int main(){
    std::string input;
    char c;

    // Read all input from stdin
    while (std::cin.get(c)){
        input.push_back(c);
    }

    // Use default test input if no input provided
    if (input.size() == 0){
        input = "Dr. Meeden doesn't like state-of-the-art models. Does she?";
    }

    // Tokenize the input text
    Tokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.tokenize(input);

    // Segment tokens into sentences
    SentenceSegmenter segmenter;
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);

    // Print each sentence with its tokens and their types
    for (int i = 0; i < sentences.size(); i++){
        std::cout << "Sentence " << i + 1 << ":\n";

        for (int j = 0; j < sentences[i].size(); j++){
            Token t = sentences[i][j];
            std::cout << " [" << t.text << "] ";

            // Print the token type
            if (t.type == TokenType::WORD) {
                std::cout << "WORD";
            } else if (t.type == TokenType::NUMBER) {
                std::cout << "NUMBER";
            } else if (t.type == TokenType::PUNCT) {
                std::cout << "PUNCT";
            } else if (t.type == TokenType::SENTENCE_END) {
                std::cout << "SENTENCE_END";
            }

            std::cout << "\n";
        }

        std::cout << "\n";
    }
    return 0;
}
