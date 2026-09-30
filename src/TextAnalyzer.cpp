
#include "TextAnalyzer.h"

#include "Tokenizer.h"
#include "Statistics.h"
#include "WordFrequency.h"
#include "TextSearch.h"
#include "ReadabilityAnalyzer.h"
#include "ReportGenerator.h"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// ==========================================
// Constructor
// ==========================================

TextAnalyzer::TextAnalyzer(std::string input)
    : text(std::move(input)) {
}

// ==========================================
// Basic text statistics
// ==========================================

std::size_t TextAnalyzer::countCharacters() const {
    return text.size();
}

std::size_t TextAnalyzer::countWords() const {
    return getWords().size();
}

std::size_t TextAnalyzer::countLines() const {
    return getStatistics().lines;
}

std::size_t TextAnalyzer::countSentences() const {
    return getSentences().size();
}

// ==========================================
// Tokenization
// ==========================================

std::vector<std::string>
TextAnalyzer::getWords() const {
    return Tokenizer::tokenizeWords(text);
}

std::vector<std::string>
TextAnalyzer::getSentences() const {
    return Tokenizer::tokenizeSentences(text);
}

// ==========================================
// Advanced statistics — Phase 3
// ==========================================

TextStatistics TextAnalyzer::getStatistics() const {
    return Statistics::analyze(
        text,
        getWords(),
        getSentences()
    );
}

void TextAnalyzer::displayStatistics() const {
    const TextStatistics stats = getStatistics();

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "         TEXT STATISTICS\n";
    std::cout << "====================================\n";

    std::cout << "Characters: "
              << stats.characters << '\n';

    std::cout << "Words: "
              << stats.words << '\n';

    std::cout << "Sentences: "
              << stats.sentences << '\n';

    std::cout << "Lines: "
              << stats.lines << '\n';

    std::cout << "Paragraphs: "
              << stats.paragraphs << '\n';

    std::cout << "Unique words: "
              << stats.uniqueWords << '\n';

    std::cout << "Longest word: "
              << stats.longestWord << '\n';

    std::cout << "Shortest word: "
              << stats.shortestWord << '\n';

    std::cout << std::fixed
              << std::setprecision(2);

    std::cout << "Average word length: "
              << stats.averageWordLength << '\n';

    std::cout << "Average sentence length: "
              << stats.averageSentenceLength << '\n';

    std::cout << "Estimated reading time: "
              << stats.readingTimeMinutes
              << " minutes\n";

    std::cout << "Lexical diversity: "
              << stats.lexicalDiversity * 100.0
              << "%\n";

    std::cout << "====================================\n";
}

// ==========================================
// Word frequency analysis — Phase 4
// ==========================================

WordFrequency TextAnalyzer::getWordFrequency() const {
    return WordFrequency(getWords());
}

void TextAnalyzer::displayWordFrequency(
    std::size_t k,
    bool excludeStopWords
) const {
    WordFrequency analyzer = getWordFrequency();

    if (excludeStopWords) {
        if (!analyzer.loadStopWords(
                "data/stopwords.txt")) {
            std::cerr
                << "Warning: Could not load "
                << "data/stopwords.txt\n";

            return;
        }
    }

    analyzer.displayTopK(
        k,
        excludeStopWords
    );
}

// ==========================================
// Text searching — Phase 5
// ==========================================

std::vector<SearchMatch> TextAnalyzer::search(
    const std::string& pattern,
    bool caseSensitive,
    bool wholeWord,
    bool useKMP
) const {
    TextSearch engine(text);

    if (useKMP) {
        return engine.findAllKMP(
            pattern,
            caseSensitive,
            wholeWord
        );
    }

    return engine.findAll(
        pattern,
        caseSensitive,
        wholeWord
    );
}

void TextAnalyzer::displaySearchResults(
    const std::string& pattern,
    bool caseSensitive,
    bool wholeWord,
    bool useKMP
) const {
    const auto matches = search(
        pattern,
        caseSensitive,
        wholeWord,
        useKMP
    );

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "          SEARCH RESULTS\n";
    std::cout << "====================================\n";

    std::cout << "Pattern: "
              << pattern << '\n';

    std::cout << "Algorithm: "
              << (useKMP ? "KMP" : "Standard")
              << '\n';

    std::cout << "Case-sensitive: "
              << (caseSensitive ? "Yes" : "No")
              << '\n';

    std::cout << "Whole-word matching: "
              << (wholeWord ? "Yes" : "No")
              << '\n';

    std::cout << "Total matches: "
              << matches.size() << '\n';

    std::cout << "------------------------------------\n";

    if (matches.empty()) {
        std::cout << "No matches found.\n";
    } else {
        for (std::size_t i = 0;
             i < matches.size();
             ++i) {

            const auto& match = matches[i];

            std::cout
                << "Match " << i + 1
                << " | Offset: " << match.position
                << " | Line: " << match.line
                << " | Column: " << match.column
                << '\n';
        }
    }

    std::cout << "====================================\n";
}

// ==========================================
// Readability analysis — Phase 6
// ==========================================

ReadabilityResult
TextAnalyzer::getReadability() const {
    return ReadabilityAnalyzer::analyze(
        getWords(),
        countSentences()
    );
}

void TextAnalyzer::displayReadability() const {
    const ReadabilityResult result =
        getReadability();

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "       READABILITY ANALYSIS\n";
    std::cout << "====================================\n";

    if (!result.valid) {
        std::cout
            << "Insufficient text for "
            << "readability analysis.\n";

        return;
    }

    std::cout << std::fixed
              << std::setprecision(2);

    std::cout << "Estimated syllables: "
              << result.totalSyllables << '\n';

    std::cout << "Flesch Reading Ease: "
              << result.fleschReadingEase << '\n';

    std::cout << "Flesch-Kincaid Grade Level: "
              << result.fleschKincaidGrade << '\n';

    std::cout << "Difficulty: "
              << result.difficulty << '\n';

    std::cout << "\nNote: Readability scores "
              << "are estimates for English text.\n";

    std::cout << "====================================\n";
}

// ==========================================
// Report generation — Phase 6
// ==========================================

AnalysisReport TextAnalyzer::generateReport(
    std::size_t topK
) const {
    AnalysisReport report;

    const auto words = getWords();
    const auto sentences = getSentences();

    report.statistics = Statistics::analyze(
        text,
        words,
        sentences
    );

    report.readability =
        ReadabilityAnalyzer::analyze(
            words,
            sentences.size()
        );

    WordFrequency frequency(words);

    report.topWords = frequency.getTopK(topK);

    return report;
}

void TextAnalyzer::exportReport(
    const std::string& format,
    const std::string& path
) const {
    const AnalysisReport report =
        generateReport();

    if (format == "txt") {
        ReportGenerator::exportTXT(
            report,
            path
        );
    }
    else if (format == "csv") {
        ReportGenerator::exportCSV(
            report,
            path
        );
    }
    else if (format == "json") {
        ReportGenerator::exportJSON(
            report,
            path
        );
    }
    else {
        throw std::invalid_argument(
            "Unsupported report format: " + format
        );
    }
}