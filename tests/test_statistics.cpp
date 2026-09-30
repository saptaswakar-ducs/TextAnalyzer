
#include "Statistics.h"
#include "Tokenizer.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

void testEmptyText() {
    const std::string text;

    const auto stats = Statistics::analyze(
        text,
        Tokenizer::tokenizeWords(text),
        Tokenizer::tokenizeSentences(text)
    );

    assert(stats.words == 0);
    assert(stats.paragraphs == 0);
    assert(stats.uniqueWords == 0);
    assert(stats.averageWordLength == 0.0);
}

void testBasicStatistics() {
    const std::string text =
        "Hello world. Hello C++!";

    const auto stats = Statistics::analyze(
        text,
        Tokenizer::tokenizeWords(text),
        Tokenizer::tokenizeSentences(text)
    );

    assert(stats.words == 4);
    assert(stats.uniqueWords == 3);
    assert(stats.sentences == 2);
    assert(stats.paragraphs == 1);
    assert(stats.longestWord == "hello");
    assert(stats.shortestWord == "c");

    assert(
        std::abs(stats.averageWordLength - 4.0)
        < 0.0001
    );

    assert(
        std::abs(stats.lexicalDiversity - 0.75)
        < 0.0001
    );
}

void testParagraphs() {
    const std::string text =
        "First paragraph.\n"
        "\n"
        "Second paragraph.\n";

    const auto stats = Statistics::analyze(
        text,
        Tokenizer::tokenizeWords(text),
        Tokenizer::tokenizeSentences(text)
    );

    assert(stats.paragraphs == 2);
    assert(stats.lines == 3);
}

int main() {
    testEmptyText();
    testBasicStatistics();
    testParagraphs();

    std::cout << "All statistics tests passed!\n";
    return 0;
}