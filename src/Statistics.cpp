
#include "Statistics.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_set>

std::size_t Statistics::countLines(
    const std::string& text
) {
    if (text.empty()) {
        return 0;
    }

    std::size_t count = std::count(
        text.begin(), text.end(), '\n'
    );

    if (text.back() != '\n') {
        ++count;
    }

    return count;
}

std::size_t Statistics::countParagraphs(
    const std::string& text
) {
    std::istringstream stream(text);
    std::string line;

    std::size_t paragraphs = 0;
    bool insideParagraph = false;

    while (std::getline(stream, line)) {
        bool hasContent = false;

        for (unsigned char ch : line) {
            if (!std::isspace(ch)) {
                hasContent = true;
                break;
            }
        }

        if (hasContent) {
            if (!insideParagraph) {
                ++paragraphs;
                insideParagraph = true;
            }
        } else {
            insideParagraph = false;
        }
    }

    return paragraphs;
}

TextStatistics Statistics::analyze(
    const std::string& text,
    const std::vector<std::string>& words,
    const std::vector<std::string>& sentences
) {
    TextStatistics result;

    result.characters = text.size();
    result.words = words.size();
    result.sentences = sentences.size();
    result.lines = countLines(text);
    result.paragraphs = countParagraphs(text);

    std::unordered_set<std::string> uniqueWords;

    std::size_t totalWordLength = 0;

    for (const auto& word : words) {
        uniqueWords.insert(word);

        totalWordLength += word.size();

        if (result.longestWord.empty() ||
            word.size() > result.longestWord.size()) {
            result.longestWord = word;
        }

        if (result.shortestWord.empty() ||
            word.size() < result.shortestWord.size()) {
            result.shortestWord = word;
        }
    }

    result.uniqueWords = uniqueWords.size();

    if (result.words > 0) {
        result.averageWordLength =
            static_cast<double>(totalWordLength)
            / result.words;

        result.lexicalDiversity =
            static_cast<double>(result.uniqueWords)
            / result.words;

        // Assume an average reading speed of 200 WPM.
        result.readingTimeMinutes =
            static_cast<double>(result.words) / 200.0;
    }

    if (result.sentences > 0) {
        result.averageSentenceLength =
            static_cast<double>(result.words)
            / result.sentences;
    }

    return result;
}