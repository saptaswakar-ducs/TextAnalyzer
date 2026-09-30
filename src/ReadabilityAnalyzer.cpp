
#include "ReadabilityAnalyzer.h"

#include <algorithm>
#include <cctype>
#include <unordered_map>

std::size_t ReadabilityAnalyzer::countSyllables(
    const std::string& word
) {
    if (word.empty()) {
        return 0;
    }

    std::string lower;
    lower.reserve(word.size());

    for (unsigned char ch : word) {
        lower += static_cast<char>(
            std::tolower(ch)
        );
    }

    // Common exceptions to spelling-based rules.
    static const std::unordered_map<
        std::string, std::size_t
    > exceptions = {
        {"the", 1},
        {"every", 2},
        {"family", 3},
        {"business", 2},
        {"people", 2},
        {"beautiful", 3},
        {"queue", 1},
        {"science", 2}
    };

    const auto exception = exceptions.find(lower);

    if (exception != exceptions.end()) {
        return exception->second;
    }

    auto isVowel = [](char ch) {
        return ch == 'a' || ch == 'e' ||
               ch == 'i' || ch == 'o' ||
               ch == 'u' || ch == 'y';
    };

    std::size_t syllables = 0;
    bool previousVowel = false;

    for (char ch : lower) {
        const bool currentVowel = isVowel(ch);

        if (currentVowel && !previousVowel) {
            ++syllables;
        }

        previousVowel = currentVowel;
    }

    // Subtract a commonly silent final e.
    if (lower.size() > 2 &&
        lower.back() == 'e' &&
        lower[lower.size() - 2] != 'l' &&
        syllables > 1) {
        --syllables;
    }

    return std::max<std::size_t>(1, syllables);
}


std::string ReadabilityAnalyzer::classifyDifficulty(
    double score
) {
    if (score >= 90.0) return "Very easy";
    if (score >= 80.0) return "Easy";
    if (score >= 70.0) return "Fairly easy";
    if (score >= 60.0) return "Standard";
    if (score >= 50.0) return "Fairly difficult";
    if (score >= 30.0) return "Difficult";

    return "Very difficult";
}

ReadabilityResult ReadabilityAnalyzer::analyze(
    const std::vector<std::string>& words,
    std::size_t sentenceCount
) {
    ReadabilityResult result;

    if (words.empty() || sentenceCount == 0) {
        return result;
    }

    for (const auto& word : words) {
        result.totalSyllables += countSyllables(word);
    }

    const double wordCount =
        static_cast<double>(words.size());

    const double sentences =
        static_cast<double>(sentenceCount);

    const double syllables =
        static_cast<double>(result.totalSyllables);

    result.fleschReadingEase =
        206.835
        - 1.015 * (wordCount / sentences)
        - 84.6 * (syllables / wordCount);

    result.fleschKincaidGrade =
        0.39 * (wordCount / sentences)
        + 11.8 * (syllables / wordCount)
        - 15.59;

    result.difficulty = classifyDifficulty(
        result.fleschReadingEase
    );

    result.valid = true;

    return result;
}