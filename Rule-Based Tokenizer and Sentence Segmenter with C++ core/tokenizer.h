#pragma once
#include <string>
#include <vector>
#include "token.h"
#include "char_classes.h"

// Tokenizes text using a finite state machine to identify words, numbers, abbreviations, contractions, etc.
class Tokenizer{
    public:
    Tokenizer();

    // Main entry point: converts input string into a vector of tokens
    std::vector<Token> tokenize(std::string input);
    // Resets the tokenizer state to initial conditions
    void reset();

    private:
    // States for the finite state machine that processes characters
    enum class State{
        START,
        IN_WORD,
        IN_NUMBER,
        IN_ABBREVIATION,
        IN_CONTRACTION,
        IN_HYPHENATED,
        IN_PUNCT,
        POSSIBLE_SENTENCE_END,
    };

    State state_;
    std::string buffer_;
    int token_start_;

    // Processes a single character and updates the FSM state, emitting tokens when appropriate
    void process_char(char c, int index, std::vector<Token>& output);
    // Creates and adds a token to the output vector using the current buffer contents
    void emit_token(std::vector<Token>& output, int end_index, TokenType type);
    // Transitions the FSM to a new state
    void transition_to(State next);

    // Checks if a string matches the pattern of an abbreviation (short word ending with period)
    bool is_abbreviation_candidate(std::string s);
    // Determines if an apostrophe is at a valid contraction boundary between two letters
    bool is_contraction_boundary(char prev, char curr);
    // Checks if a hyphen should be treated as internal to a word rather than a separator
    bool is_hyphen_internal(char prev, char next);
};