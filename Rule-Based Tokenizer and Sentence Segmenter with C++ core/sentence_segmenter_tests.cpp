#include <iostream>
#include <vector>
#include <string>

#include "sentence_segmenter.h"
#include "token.h"

int tests_run = 0;
int tests_failed = 0;

// Helper to create a WORD token
Token create_word_token(std::string text, int start, int end) {
    return Token(text, TokenType::WORD, start, end);
}

// Helper to create a PUNCT token
Token create_punct_token(std::string text, int start, int end) {
    return Token(text, TokenType::PUNCT, start, end);
}

// Helper to create a SENTENCE_END token
Token create_sentence_end_token(std::string text, int start, int end) {
    return Token(text, TokenType::SENTENCE_END, start, end);
}

// Helper to create a NUMBER token
Token create_number_token(std::string text, int start, int end) {
    return Token(text, TokenType::NUMBER, start, end);
}

// Compares two strings and reports test pass/fail status
void assert_equal(std::string got, std::string expected, std::string test_name){
    tests_run++;
    if (got != expected){
        tests_failed++;
        std::cout << "[FAIL] " << test_name << "\n";
        std::cout << "Expected: " << expected << "\n";
        std::cout << "Got: " << got << "\n\n";
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
}

// Compares two integers and reports test pass/fail status
void assert_equal(int got, int expected, std::string test_name){
    tests_run++;
    if (got != expected){
        tests_failed++;
        std::cout << "[FAIL] " << test_name << "\n";
        std::cout << "Expected: " << expected << "\n";
        std::cout << "Got: " << got << "\n\n";
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
}

// Formats a sentence (vector of tokens) as a string for comparison
std::string sentence_to_string(const std::vector<Token>& sentence) {
    std::string result;
    for (size_t i = 0; i < sentence.size(); i++) {
        result += sentence[i].text;
        if (i + 1 < sentence.size()) {
            result += " ";
        }
    }
    return result;
}

// Formats all sentences for comparison
std::string sentences_to_string(const std::vector<std::vector<Token>>& sentences) {
    std::string result;
    for (size_t i = 0; i < sentences.size(); i++) {
        result += "[" + sentence_to_string(sentences[i]) + "]";
        if (i + 1 < sentences.size()) {
            result += " ";
        }
    }
    return result;
}

// Test basic sentence segmentation with period
void test_basic_sentence_period() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_word_token("Hello", 0, 4),
        create_word_token("world", 6, 10),
        create_punct_token(".", 11, 11)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "basic sentence period: sentence count");
    if (sentences.size() == 1) {
        assert_equal(sentence_to_string(sentences[0]), "Hello world .", 
                    "basic sentence period: content");
    }
}

// Test sentence segmentation with exclamation mark
void test_sentence_exclamation() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_word_token("Wow", 0, 2),
        create_punct_token("!", 3, 3)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "sentence exclamation: sentence count");
    assert_equal(sentence_to_string(sentences[0]), "Wow !", 
                "sentence exclamation: content");
}

// Test sentence segmentation with question mark
void test_sentence_question() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_word_token("How", 0, 2),
        create_word_token("are", 4, 6),
        create_word_token("you", 8, 10),
        create_punct_token("?", 11, 11)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "sentence question: sentence count");
    assert_equal(sentence_to_string(sentences[0]), "How are you ?", 
                "sentence question: content");
}

// Test multiple sentences
void test_multiple_sentences() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_word_token("This", 0, 3),
        create_word_token("is", 5, 6),
        create_word_token("sentence", 8, 15),
        create_word_token("one", 17, 19),
        create_punct_token(".", 20, 20),
        create_word_token("This", 22, 25),
        create_word_token("is", 27, 28),
        create_word_token("sentence", 30, 37),
        create_word_token("two", 39, 41),
        create_punct_token("!", 42, 42)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 2, "multiple sentences: sentence count");
    assert_equal(sentences_to_string(sentences), 
                "[This is sentence one .] [This is sentence two !]", 
                "multiple sentences: content");
}

// Test that SENTENCE_END token type works
void test_sentence_end_token_type() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_word_token("The", 0, 2),
        create_word_token("end", 4, 6),
        create_sentence_end_token(".", 7, 7)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "sentence_end token type: sentence count");
    assert_equal(sentence_to_string(sentences[0]), "The end .", 
                "sentence_end token type: content");
}

// Test sentence without ending punctuation (should be treated as one sentence)
void test_no_sentence_end() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_word_token("No", 0, 1),
        create_word_token("ending", 3, 8),
        create_word_token("punctuation", 10, 20)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "no sentence end: sentence count");
    assert_equal(sentence_to_string(sentences[0]), "No ending punctuation", 
                "no sentence end: content");
}

// Test mixed content with numbers and punctuation
void test_mixed_content() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_number_token("42", 0, 1),
        create_word_token("is", 3, 4),
        create_word_token("the", 6, 8),
        create_word_token("answer", 10, 15),
        create_punct_token(".", 16, 16),
        create_word_token("True", 18, 21),
        create_word_token("or", 23, 24),
        create_word_token("false", 26, 30),
        create_punct_token("?", 31, 31)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 2, "mixed content: sentence count");
    assert_equal(sentences_to_string(sentences), 
                "[42 is the answer .] [True or false ?]", 
                "mixed content: content");
}

// Test punctuation that is NOT sentence-ending
void test_non_sentence_ending_punct() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_word_token("Hello", 0, 4),
        create_punct_token(",", 5, 5),
        create_word_token("world", 7, 11),
        create_punct_token(".", 12, 12)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "non-sentence ending punct: sentence count");
    assert_equal(sentence_to_string(sentences[0]), "Hello , world .", 
                "non-sentence ending punct: content");
}

// Test empty token list
void test_empty_tokens() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens;
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 0, "empty tokens: sentence count");
}

// Test single token that is sentence-ending
void test_single_sentence_ending_token() {
    SentenceSegmenter segmenter;
    std::vector<Token> tokens = {
        create_punct_token("!", 0, 0)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "single sentence ending token: sentence count");
    assert_equal(sentence_to_string(sentences[0]), "!", 
                "single sentence ending token: content");
}

// Test that abbreviations with periods don't break sentences (if tokenized as ABBREVIATION type)
// Note: This depends on how tokens are created - if "Dr." is tokenized as ABBREVIATION type,
// it won't trigger sentence end in current implementation
void test_abbreviation_in_sentence() {
    SentenceSegmenter segmenter;
    // Assuming "Dr." would be tokenized as ABBREVIATION type, not PUNCT
    std::vector<Token> tokens = {
        Token("Dr.", TokenType::ABBREVIATION, 0, 2),
        create_word_token("Smith", 4, 8),
        create_word_token("arrived", 10, 16),
        create_punct_token(".", 17, 17)
    };
    
    std::vector<std::vector<Token>> sentences = segmenter.segment(tokens);
    
    assert_equal(sentences.size(), 1, "abbreviation in sentence: sentence count");
    std::string result = sentences_to_string(sentences);
    // Should keep "Dr." with the sentence, not break after it
    assert_equal(result.find("Dr. Smith"), 1, "abbreviation in sentence: Dr. kept in sentence");
}

// Runs all sentence segmenter test cases
int main(){
    std::cout << "Running Sentence Segmenter Tests\n";
    std::cout << "===============================\n\n";
    
    test_basic_sentence_period();
    test_sentence_exclamation();
    test_sentence_question();
    test_multiple_sentences();
    test_sentence_end_token_type();
    test_no_sentence_end();
    test_mixed_content();
    test_non_sentence_ending_punct();
    test_empty_tokens();
    test_single_sentence_ending_token();
    test_abbreviation_in_sentence();

    std::cout << "\n===============================\n";
    std::cout << "Tests run: " << tests_run << "\n";
    std::cout << "Tests failed: " << tests_failed << "\n";

    if (tests_failed > 0){
        return 1;
    }
    return 0;
}