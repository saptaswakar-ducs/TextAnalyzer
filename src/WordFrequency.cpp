
#include "WordFrequency.h"
#include "Tokenizer.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

WordFrequency::WordFrequency(
    const std::vector<std::string>& words
) : totalWords(words.size()) {

    for (const auto& word : words) {
        ++frequencies[word];
    }
}

std::size_t WordFrequency::getFrequency(
    const std::string& word
) const {
    const auto tokens =
        Tokenizer::tokenizeWords(word);

    if (tokens.size() != 1) {
        return 0;
    }

    const auto it = frequencies.find(tokens[0]);

    if (it == frequencies.end()) {
        return 0;
    }

    return it->second;
}

std::vector<WordFrequency::FrequencyEntry>
WordFrequency::getTopK(
    std::size_t k,
    bool excludeStopWords
) const {
    std::vector<FrequencyEntry> entries;

    for (const auto& entry : frequencies) {
        if (excludeStopWords &&
            stopWords.count(entry.first) > 0) {
            continue;
        }

        entries.push_back(entry);
    }

    std::sort(
        entries.begin(),
        entries.end(),
        [](const auto& a, const auto& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }

            return a.first < b.first;
        }
    );

    if (entries.size() > k) {
        entries.resize(k);
    }

    return entries;
}

std::vector<std::string>
WordFrequency::getHapaxLegomena() const {
    std::vector<std::string> result;

    for (const auto& entry : frequencies) {
        if (entry.second == 1) {
            result.push_back(entry.first);
        }
    }

    std::sort(result.begin(), result.end());

    return result;
}

double WordFrequency::getRelativeFrequency(
    const std::string& word
) const {
    if (totalWords == 0) {
        return 0.0;
    }

    return static_cast<double>(
        getFrequency(word)
    ) / totalWords;
}

bool WordFrequency::loadStopWords(
    const std::string& filePath
) {
    std::ifstream file(filePath);

    if (!file) {
        return false;
    }

    std::unordered_set<std::string> loaded;
    std::string line;

    while (std::getline(file, line)) {
        const auto words =
            Tokenizer::tokenizeWords(line);

        for (const auto& word : words) {
            loaded.insert(word);
        }
    }

    if (file.bad()) {
        return false;
    }

    stopWords = std::move(loaded);
    return true;
}

void WordFrequency::displayTopK(
    std::size_t k,
    bool excludeStopWords
) const {
    const auto entries =
        getTopK(k, excludeStopWords);

    std::cout << "\n===== TOP WORDS =====\n";

    std::cout << std::left
              << std::setw(20) << "Word"
              << std::setw(12) << "Count"
              << "Frequency\n";

    std::cout << std::string(45, '-') << '\n';

    for (const auto& [word, count] : entries) {
        std::cout << std::left
                  << std::setw(20) << word
                  << std::setw(12) << count
                  << std::fixed
                  << std::setprecision(2)
                  << getRelativeFrequency(word) * 100
                  << "%\n";
    }
}