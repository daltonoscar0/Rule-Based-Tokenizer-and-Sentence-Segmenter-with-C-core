# RULE-BASED TOKENIZER AND SENTNECE SEGMENTER WITH C++ CORE

## Overview 
This is a C++ implementation of a text tokenizer and sentnece segmenter that processes natural language text into tokens and groups them into sentnences using a Finite State Machine (FSM) approach.

This project provides a pipeline for text processing:

1. Tokenizer: converts raw text into structured tokens (words, numbers, punctuation, abbreviations, contractions, etc.)
2. Sentence Segmenter: Groups tokens into sentneces based on sentence-ending punctuation

This system handles complex language phenomena like abbreviations, contractions, hyphenated words, and ellipses. 

### Features 
#### Tokenizer Features 
- Identifies words, numbers, and punctuation
- Handles contractions (e.g. "don't" and "I'll")
- Processes hyphenated words (e.g. "state-of-the-art")
- Recognized abbreviations (e.g. "Dr." and "US")
- Detects ellipses ("...")
- Uses a finite state machine for robust character-by-character processing
- Preserves token positino information in original text

#### Sentence Segmenter Features 
- Identifies sentence boundaries using punctuation (., !, ?)
- Handles SENTENCE_END token types
- Properly handles abbreviations that end with periods
- Groups tokens into sentence vectors

### Project Structure 
|-- tokenizer.h/.cpp            # Main tokenier class with FSM implementation
|-- char_classes.h/.cpp         # Character classification utilities
|-- token.h                     # Token data structure and types
|-- sentence_segmenter.h/.cpp   # Sentence segmentation logic
|-- utils.h                     # String utility declarations
|-- tss_main.cpp                # Main program entry point
|-- tokenizer_tests.cpp         # Tokenizer test suite
|-- sentence_segmenter_tests.cpp # Sentence segmenter test suite

### Token Types 
WORD                            # Standard words
NUMBER                          # Numeric sequences
PUNCT                           # Punctuation Marks
ABBREVIATION                    # Abbreviations
CONTRACTION                     # Contractions with apostrophes
HYPHENATED                      # Hyphenated compound words
SENTENCE_END                    # Explicit sentence-ending markers

## Building the Project 
Prerequisities -- C++ compiler with C++11 support (g++ 4.8+ or clang++ 3.3+)

### Compiling Statement
g++ -std=c++11 -o tss \
    tss_main.cpp \
    tokenizer.cpp \
    char_classes.cpp \
    sentence_segmenter.cpp

Then, type "./tss" an press enter. After doing so, input your message, press enter, and then Ctrl + D to see the program work.

### Test Outline
#### Build tokenizer tests
g++ -std=c++11 -o tokenizer_tests \
    tokenizer_tests.cpp \
    tokenizer.cpp \
    char_classes.cpp \
    -I.

#### Run tokenizer tests
./tokenizer_tests

#### Expected output
[PASS] simple sentence
[PASS] contractions
[PASS] hyphenation
[PASS] abbreviations
[PASS] ellipsis

Tests run: 5
Tests failed: 0

#### Build sentence segmenter tests
g++ -std=c++11 -o segmenter_tests \
    sentence_segmenter_tests.cpp \
    sentence_segmenter.cpp \
    -I.

#### Run sentence segmenter tests
./sentence_segmenter_tests

#### Expected output
Running Sentence Segmenter Tests
===============================

[PASS] basic sentence period: sentence count
[PASS] basic sentence period: content
[PASS] sentence exclamation: sentence count
... (more test results)

Tests run: 11
Tests failed: 0

### Example walkthrough
Step 1: Run the program
./tss

Step 2: Type input
The quick brown fox jumps over 3 lazy dogs. Amazing!

Step 3: Press Ctrl + D, you'll see:
Sentence 1:
 [The] WORD
 [quick] WORD
 [brown] WORD
 [fox] WORD
 [jumps] WORD
 [over] WORD
 [3] NUMBER
 [lazy] WORD
 [dogs] WORD
 [.] PUNCT

Sentence 2:
 [Amazing] WORD
 [!] PUNCT

 ## How it works 
 Take the text: "Dr. Jones can't visit on Jan. 31st."
 The program uses step-by-step processing to find the states of each expression, it implicitly looks like something like this:

    D → r → . = [Dr.] ABBREVIATION
    J → o → n → e → s = [Jones] WORD
    c → a → n → ' → t = [can't] CONTRACTION
    v → i → s → i → t = [visit] WORD
    o → n = [on] WORD
    J → a → n → . = [Jan.] ABBREVIATION
    3 → 1 → s → t = [31st] WORD

So the final output looks like this:

Sentence 1:
 [Dr.] ABBREVIATION
 [Jones] WORD
 [can't] CONTRACTION
 [visit] WORD
 [on] WORD
 [Jan.] ABBREVIATION
 [31st] WORD
 [.] PUNCT