
#include "ReadabilityAnalyzer.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

void testSyllables() {
    assert(ReadabilityAnalyzer::countSyllables("cat") == 1);
    assert(ReadabilityAnalyzer::countSyllables("hello") == 2);
    assert(ReadabilityAnalyzer::countSyllables("beautiful") == 3);
    assert(ReadabilityAnalyzer::countSyllables("") == 0);
}

void testReadability() {
    const std::vector<std::string> words = {
        "the", "cat", "is", "happy"
    };

    const auto result =
        ReadabilityAnalyzer::analyze(words, 1);

    assert(result.valid);
    assert(result.totalSyllables == 5);

    assert(
        std::abs(result.fleschReadingEase - 97.025)
        < 0.001
    );

    assert(
        std::abs(result.fleschKincaidGrade - 0.72)
        < 0.001
    );
}

void testEmptyText() {
    const auto result =
        ReadabilityAnalyzer::analyze({}, 0);

    assert(!result.valid);
    assert(result.totalSyllables == 0);
}

int main() {
    testSyllables();
    testReadability();
    testEmptyText();

    std::cout << "All readability tests passed!\n";
}