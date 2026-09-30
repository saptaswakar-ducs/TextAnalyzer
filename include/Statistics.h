
#ifndef STATISTICS_H
#define STATISTICS_H

#include <cstddef>
#include <string>
#include <vector>

struct TextStatistics {
    std::size_t characters = 0;
    std::size_t words = 0;
    std::size_t sentences = 0;
    std::size_t lines = 0;
    std::size_t paragraphs = 0;
    std::size_t uniqueWords = 0;

    std::string longestWord;
    std::string shortestWord;

    double averageWordLength = 0.0;
    double averageSentenceLength = 0.0;
    double readingTimeMinutes = 0.0;
    double lexicalDiversity = 0.0;
};

class Statistics {
public:
    static TextStatistics analyze(
        const std::string& text,
        const std::vector<std::string>& words,
        const std::vector<std::string>& sentences
    );

private:
    static std::size_t countLines(
        const std::string& text
    );

    static std::size_t countParagraphs(
        const std::string& text
    );
};

#endif