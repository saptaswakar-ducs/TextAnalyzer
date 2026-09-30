
#include "ReportGenerator.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace {

std::ofstream openReport(const std::string& path) {
    const std::filesystem::path output(path);

    if (output.has_parent_path()) {
        std::filesystem::create_directories(
            output.parent_path()
        );
    }

    std::ofstream file(path);

    if (!file) {
        throw std::runtime_error(
            "Cannot create report: " + path
        );
    }

    return file;
}

void checkOutput(
    const std::ofstream& file
) {
    if (!file) {
        throw std::runtime_error(
            "Error while writing report"
        );
    }
}

} // namespace

std::string ReportGenerator::escapeCSV(
    const std::string& value
) {
    std::string result = "\"";

    for (char ch : value) {
        if (ch == '"') {
            result += "\"\"";
        } else {
            result += ch;
        }
    }

    return result + "\"";
}

std::string ReportGenerator::escapeJSON(
    const std::string& value
) {
    std::ostringstream result;

    for (unsigned char ch : value) {
        switch (ch) {
            case '"': result << "\\\""; break;
            case '\\': result << "\\\\"; break;
            case '\n': result << "\\n"; break;
            case '\r': result << "\\r"; break;
            case '\t': result << "\\t"; break;
            default:
                if (ch < 0x20) {
                    result << "\\u"
                           << std::hex
                           << std::setw(4)
                           << std::setfill('0')
                           << static_cast<int>(ch)
                           << std::dec;
                } else {
                    result << static_cast<char>(ch);
                }
        }
    }

    return result.str();
}


void ReportGenerator::exportTXT(
    const AnalysisReport& report,
    const std::string& path
) {
    auto file = openReport(path);

    const auto& s = report.statistics;
    const auto& r = report.readability;

    file << std::fixed << std::setprecision(2);

    file << "TEXT ANALYSIS REPORT\n";
    file << "====================\n\n";

    file << "TEXT STATISTICS\n";
    file << "Characters: " << s.characters << '\n';
    file << "Words: " << s.words << '\n';
    file << "Sentences: " << s.sentences << '\n';
    file << "Lines: " << s.lines << '\n';
    file << "Paragraphs: " << s.paragraphs << '\n';
    file << "Unique words: " << s.uniqueWords << '\n';
    file << "Longest word: " << s.longestWord << '\n';
    file << "Shortest word: " << s.shortestWord << '\n';

    file << "Average word length: "
         << s.averageWordLength << '\n';

    file << "Average sentence length: "
         << s.averageSentenceLength << '\n';

    file << "Reading time (minutes): "
         << s.readingTimeMinutes << '\n';

    file << "Lexical diversity: "
         << s.lexicalDiversity * 100.0 << "%\n";

    file << "\nREADABILITY\n";

    if (r.valid) {
        file << "Syllables: " << r.totalSyllables << '\n';
        file << "Flesch Reading Ease: "
             << r.fleschReadingEase << '\n';
        file << "Flesch-Kincaid Grade: "
             << r.fleschKincaidGrade << '\n';
        file << "Difficulty: " << r.difficulty << '\n';
    } else {
        file << "Insufficient text for scoring.\n";
    }

    file << "\nTOP WORDS\n";

    for (const auto& [word, count] : report.topWords) {
        file << word << ": " << count << '\n';
    }

    file << "\nNote: Syllable counts and readability "
            "scores are estimates.\n";

    file.flush();
    checkOutput(file);
}


void ReportGenerator::exportCSV(
    const AnalysisReport& report,
    const std::string& path
) {
    auto file = openReport(path);

    const auto& s = report.statistics;
    const auto& r = report.readability;

    file << std::fixed << std::setprecision(4);
    file << "section,metric,value\n";

    auto row = [&](const std::string& section,
                   const std::string& metric,
                   const auto& value) {
        std::ostringstream formatted;
        formatted << value;

        file << escapeCSV(section) << ','
             << escapeCSV(metric) << ','
             << escapeCSV(formatted.str()) << '\n';
    };

    row("statistics", "characters", s.characters);
    row("statistics", "words", s.words);
    row("statistics", "sentences", s.sentences);
    row("statistics", "lines", s.lines);
    row("statistics", "paragraphs", s.paragraphs);
    row("statistics", "unique_words", s.uniqueWords);
    row("statistics", "longest_word", s.longestWord);
    row("statistics", "shortest_word", s.shortestWord);
    row("statistics", "average_word_length",
        s.averageWordLength);
    row("statistics", "average_sentence_length",
        s.averageSentenceLength);
    row("statistics", "reading_time_minutes",
        s.readingTimeMinutes);
    row("statistics", "lexical_diversity",
        s.lexicalDiversity);

    row("readability", "valid", r.valid);

    if (r.valid) {
        row("readability", "syllables", r.totalSyllables);
        row("readability", "flesch_reading_ease",
            r.fleschReadingEase);
        row("readability", "flesch_kincaid_grade",
            r.fleschKincaidGrade);
        row("readability", "difficulty", r.difficulty);
    }

    for (const auto& [word, count] : report.topWords) {
        row("word_frequency", word, count);
    }

    file.flush();
    checkOutput(file);
}


void ReportGenerator::exportJSON(
    const AnalysisReport& report,
    const std::string& path
) {
    auto file = openReport(path);

    const auto& s = report.statistics;
    const auto& r = report.readability;

    file << std::fixed << std::setprecision(4);

    file << "{\n";
    file << "  \"statistics\": {\n";
    file << "    \"characters\": " << s.characters << ",\n";
    file << "    \"words\": " << s.words << ",\n";
    file << "    \"sentences\": " << s.sentences << ",\n";
    file << "    \"lines\": " << s.lines << ",\n";
    file << "    \"paragraphs\": " << s.paragraphs << ",\n";
    file << "    \"unique_words\": " << s.uniqueWords << ",\n";

    file << "    \"longest_word\": \""
         << escapeJSON(s.longestWord) << "\",\n";

    file << "    \"shortest_word\": \""
         << escapeJSON(s.shortestWord) << "\",\n";

    file << "    \"average_word_length\": "
         << s.averageWordLength << ",\n";

    file << "    \"average_sentence_length\": "
         << s.averageSentenceLength << ",\n";

    file << "    \"reading_time_minutes\": "
         << s.readingTimeMinutes << ",\n";

    file << "    \"lexical_diversity\": "
         << s.lexicalDiversity << "\n";
    file << "  },\n";

    file << "  \"readability\": {\n";
    file << "    \"valid\": "
         << (r.valid ? "true" : "false");

    if (r.valid) {
        file << ",\n    \"total_syllables\": "
             << r.totalSyllables;

        file << ",\n    \"flesch_reading_ease\": "
             << r.fleschReadingEase;

        file << ",\n    \"flesch_kincaid_grade\": "
             << r.fleschKincaidGrade;

        file << ",\n    \"difficulty\": \""
             << escapeJSON(r.difficulty) << "\"\n";
    } else {
        file << '\n';
    }

    file << "  },\n";

    file << "  \"top_words\": [\n";

    for (std::size_t i = 0;
         i < report.topWords.size(); ++i) {
        const auto& [word, count] = report.topWords[i];

        file << "    {\"word\": \""
             << escapeJSON(word)
             << "\", \"count\": " << count << "}";

        if (i + 1 < report.topWords.size()) {
            file << ',';
        }

        file << '\n';
    }

    file << "  ]\n";
    file << "}\n";

    file.flush();
    checkOutput(file);
}