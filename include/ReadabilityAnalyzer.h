
#ifndef READABILITY_ANALYZER_H
#define READABILITY_ANALYZER_H

#include <cstddef>
#include <string>
#include <vector>

struct ReadabilityResult {
    std::size_t totalSyllables = 0;

    double fleschReadingEase = 0.0;
    double fleschKincaidGrade = 0.0;

    bool valid = false;

    std::string difficulty;
};

class ReadabilityAnalyzer {
public:
    static ReadabilityResult analyze(
        const std::vector<std::string>& words,
        std::size_t sentenceCount
    );

    static std::size_t countSyllables(
        const std::string& word
    );

private:
    static std::string classifyDifficulty(
        double score
    );
};

#endif