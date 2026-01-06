#include "tokenizer.h"
#include "char_classes.h"
#include "token.h"
#include <string>
#include <vector>

// Initializes the tokenizer with starting state and empty buffer
Tokenizer::Tokenizer(){
    state_ = State::START;
    buffer_ = " ";
    token_start_ = 0;
}

// Resets the tokenizer to initial state, clearing the buffer
void Tokenizer::reset(){
    state_ = State::START;
    buffer_ = " ";
    token_start_ = 0;
}

// Main tokenization function: processes input character by character and returns all tokens
std::vector<Token> Tokenizer::tokenize(std::string input){
    std::vector<Token> tokens;

    // Process each character through the finite state machine
    for (int i=0; i<input.length(); i++){
        process_char(input[i], i, tokens);
    }

    // Emit any remaining token in the buffer after processing all characters
    if (buffer_ != " "){
        if (state_ == State::IN_ABBREVIATION){
            emit_token(tokens, input.length() - 1, TokenType::ABBREVIATION);
        } else if (state_ == State::IN_PUNCT){
            emit_token(tokens, input.length() - 1, TokenType::PUNCT);
        } else {
            emit_token(tokens, input.length() - 1, TokenType::WORD);
        }
    } 

    return tokens;
}

// Core FSM logic: processes each character based on current state and transitions appropriately
void Tokenizer::process_char(char c, int index, std::vector<Token>& output){
        CharClassifier classifier;
        CharClass cc = classifier.classify_char(c);

        // START state: beginning of a new token
        if (state_ == State::START){
            if(cc == CharClass::LETTER){
                buffer_ = c;
                token_start_ = index;
                state_ = State::IN_WORD;
            } else if (cc == CharClass::DIGIT){
                buffer_ = c;
                token_start_ = index;
                state_ = State::IN_NUMBER;
            } else if (cc == CharClass::PUNCT || cc == CharClass::PERIOD){
                buffer_ = c;
                token_start_ = index;
                state_ = State::IN_PUNCT;
            }
        }

        // IN_WORD state: accumulating letters into a word token
        else if (state_ == State::IN_WORD){
            if (cc == CharClass::LETTER){
                buffer_ = buffer_ + c;
            } else if (cc == CharClass::PERIOD){
                // Check if this looks like an abbreviation (short word <= 4 chars including period)
                std::string test_abbrev = buffer_ + c;
                if (is_abbreviation_candidate(test_abbrev)){
                    buffer_ = buffer_ + c;
                    state_ = State::IN_ABBREVIATION;
                } else {
                    // Not an abbreviation, emit word and process period separately
                    emit_token(output, index - 1, TokenType::WORD);
                    buffer_ = " ";
                    state_ = State::START;
                    process_char(c, index, output);
                }
            } else if (cc == CharClass::APOSTROPHE){
                buffer_ = buffer_ + c;
                state_ = State::IN_CONTRACTION;
            } else if (cc == CharClass::HYPHEN){
                buffer_ = buffer_ + c;
                state_ = State::IN_HYPHENATED;
            } else {
                emit_token(output, index - 1, TokenType::WORD);
                buffer_ = " ";
                state_ = State::START;
                process_char(c, index, output);
            }
        }

        // IN_NUMBER state: accumulating digits into a number token
        else if (state_ == State::IN_NUMBER){
            if (cc == CharClass::DIGIT){
                buffer_ = buffer_ + c;
            } else {
                emit_token(output, index - 1, TokenType::NUMBER);
                buffer_ = " ";
                state_ = State::START;
                process_char(c, index, output);
            }
        }

        // IN_PUNCT state: accumulating punctuation characters
        else if (state_ == State::IN_PUNCT){
            if (cc == CharClass::PUNCT || cc == CharClass::PERIOD){
                buffer_ = buffer_ + c;
            } else {
                emit_token(output, index - 1, TokenType::PUNCT);
                buffer_ = " ";
                state_ = State::START;
                process_char(c, index, output);
            }
        }

        // IN_CONTRACTION state: processing apostrophe in contractions like "don't"
        else if (state_ == State::IN_CONTRACTION){
            if (cc == CharClass::LETTER){
                buffer_ = buffer_ + c;
                state_ = State::IN_WORD;
            } else {
                emit_token(output, index - 1, TokenType::CONTRACTION);
                buffer_ = " ";
                state_ = State::START;
                process_char(c, index, output);
            }
        }

        // IN_ABBREVIATION state: handling abbreviations like "Dr." or "U.S."
        else if (state_ == State::IN_ABBREVIATION){
            if (cc == CharClass::LETTER){
                buffer_ = buffer_ + c;
                state_ = State::IN_WORD;
            } else if (cc == CharClass::PERIOD){
                // For abbreviations like "U.S.", allow letter-period-letter-period pattern
                buffer_ = buffer_ + c;
                // Stay in IN_ABBREVIATION for multiple periods like "U.S."
            } else if (cc == CharClass::WHITESPACE){
                // Whitespace after abbreviation - emit it
                emit_token(output, index - 1, TokenType::ABBREVIATION);
                buffer_ = " ";
                state_ = State::START;
                process_char(c, index, output);
            } else {
                emit_token(output, index - 1, TokenType::ABBREVIATION);
                buffer_ = " ";
                state_ = State::START;
                process_char(c, index, output);
            }
        }

        // IN_HYPHENATED state: processing hyphenated words like "state-of-the-art"
        else if (state_ == State::IN_HYPHENATED){
            if (cc == CharClass::LETTER){
                buffer_ = buffer_ + c;
                state_ = State::IN_WORD;
            } else {
                emit_token(output, index - 1, TokenType::HYPHENATED);
                buffer_ = " ";
                state_ = State::START;
                process_char(c, index, output);
            }
        }
    }

// Creates a token from the current buffer and adds it to the output, then resets the buffer
void Tokenizer::emit_token(std::vector<Token>& output, int end_index, TokenType type){
        Token t(buffer_, type, token_start_, end_index);
        output.push_back(t);
        buffer_ = " ";
        state_ = State::START;
    }

// Transitions the FSM to a new state
void Tokenizer::transition_to(State next){
        state_ = next;
    }

// Checks if a string is a candidate abbreviation: ends with period and is 4 characters or less
bool Tokenizer::is_abbreviation_candidate(std::string s){
        if (s.length() > 0 && s[s.length() - 1] == '.' && s.length() <= 4){
            return true;
        } 
        return false;
    }

// Determines if an apostrophe is between two letters (valid contraction boundary)
bool Tokenizer::is_contraction_boundary(char prev, char curr){
       if (((prev >= 'a' && prev <= 'z') || (prev >= 'A' && prev <= 'Z')) &&
        ((curr >= 'a' && curr <= 'z') || (curr >= 'A' && curr <= 'Z'))) {
        return true;
    }
    return false;
}