
#include "Tokenizer.h"
#include "WordFrequency.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

void testFrequency() {
    const auto words = Tokenizer::tokenizeWords(
        "Apple banana apple orange banana apple"
    );

    WordFrequency analyzer(words);

    assert(analyzer.getFrequency("apple") == 3);
    assert(analyzer.getFrequency("BANANA") == 2);
    assert(analyzer.getFrequency("orange") == 1);
    assert(analyzer.getFrequency("grape") == 0);
}

void testTopK() {
    const auto words = Tokenizer::tokenizeWords(
        "apple banana apple orange banana apple"
    );

    WordFrequency analyzer(words);

    const auto top = analyzer.getTopK(2);

    assert(top.size() == 2);
    assert(top[0].first == "apple");
    assert(top[0].second == 3);
    assert(top[1].first == "banana");
    assert(top[1].second == 2);
}

void testRelativeFrequency() {
    WordFrequency analyzer(
        {"apple", "banana", "apple", "orange"}
    );

    assert(
        std::abs(
            analyzer.getRelativeFrequency("apple") - 0.5
        ) < 0.0001
    );
}

void testHapaxLegomena() {
    WordFrequency analyzer(
        {"apple", "banana", "apple", "orange"}
    );

    const auto result = analyzer.getHapaxLegomena();

    assert(result.size() == 2);
    assert(result[0] == "banana");
    assert(result[1] == "orange");
}

void testStopWords() {
    WordFrequency analyzer(
        {"the", "cat", "is", "the", "cat", "happy"}
    );

    assert(analyzer.loadStopWords(
        "data/stopwords.txt"
    ));

    const auto top = analyzer.getTopK(3, true);

    assert(top.size() == 2);
    assert(top[0].first == "cat");
    assert(top[0].second == 2);
    assert(top[1].first == "happy");
}

void testEmptyInput() {
    WordFrequency analyzer({});

    assert(analyzer.getTopK(10).empty());
    assert(analyzer.getFrequency("apple") == 0);
    assert(analyzer.getRelativeFrequency("apple") == 0);
}

int main() {
    testFrequency();
    testTopK();
    testRelativeFrequency();
    testHapaxLegomena();
    testStopWords();
    testEmptyInput();

    std::cout
        << "All word frequency tests passed!\n";
}