
#ifndef WORD_FREQUENCY_H
#define WORD_FREQUENCY_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

class WordFrequency {
public:
    using FrequencyEntry =
        std::pair<std::string, std::size_t>;

    explicit WordFrequency(
        const std::vector<std::string>& words
    );

    std::size_t getFrequency(
        const std::string& word
    ) const;

    std::vector<FrequencyEntry> getTopK(
        std::size_t k,
        bool excludeStopWords = false
    ) const;

    std::vector<std::string> getHapaxLegomena() const;

    double getRelativeFrequency(
        const std::string& word
    ) const;

    bool loadStopWords(
        const std::string& filePath
    );

    void displayTopK(
        std::size_t k,
        bool excludeStopWords = false
    ) const;

private:
    std::unordered_map<
        std::string, std::size_t
    > frequencies;

    std::unordered_set<std::string> stopWords;

    std::size_t totalWords = 0;
};

#endif