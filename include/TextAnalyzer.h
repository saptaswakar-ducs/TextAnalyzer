
#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include "Statistics.h"

#include "WordFrequency.h"
#include "TextSearch.h"
#include "ReadabilityAnalyzer.h"
#include "ReportGenerator.h"

#include <cstddef>
#include <string>
#include <vector>

class TextAnalyzer {
private:
    std::string text;
    

public:
    explicit TextAnalyzer(std::string input);

    std::size_t countCharacters() const;
    std::size_t countWords() const;
    std::size_t countLines() const;
    std::size_t countSentences() const;

    std::vector<std::string> getWords() const;
    std::vector<std::string> getSentences() const;

    TextStatistics getStatistics() const;

    void displayStatistics() const;

    WordFrequency getWordFrequency() const;

    ReadabilityResult getReadability() const;

    void displayReadability() const;

    void displayWordFrequency(
        std::size_t k,
        bool excludeStopWords = false
    ) const;

    std::vector<SearchMatch> search(
        const std::string& pattern,
        bool caseSensitive = false,
        bool wholeWord = false,
        bool useKMP = false
    ) const;

    void displaySearchResults(
        const std::string& pattern,
        bool caseSensitive = false,
        bool wholeWord = false,
        bool useKMP = false
    ) const;

    AnalysisReport generateReport(
        std::size_t topK = 10
    ) const;

    void exportReport(
        const std::string& format,
        const std::string& path
    ) const;
};

#endif