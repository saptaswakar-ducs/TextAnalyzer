
#include "ReportGenerator.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

std::string readFile(const std::string& path) {
    std::ifstream file(path);

    assert(file.is_open());

    return std::string(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

int main() {
    AnalysisReport report;

    report.statistics.words = 4;
    report.statistics.uniqueWords = 3;
    report.statistics.sentences = 1;

    report.readability.valid = true;
    report.readability.totalSyllables = 5;
    report.readability.fleschReadingEase = 97.025;
    report.readability.fleschKincaidGrade = 0.72;
    report.readability.difficulty = "Very easy";

    report.topWords = {
        {"cat", 2},
        {"happy", 1},
        {"the", 1}
    };

    const std::string directory =
        "reports/test_output";

    ReportGenerator::exportTXT(
        report, directory + "/test.txt"
    );

    ReportGenerator::exportCSV(
        report, directory + "/test.csv"
    );

    ReportGenerator::exportJSON(
        report, directory + "/test.json"
    );

    const auto txt =
        readFile(directory + "/test.txt");

    const auto csv =
        readFile(directory + "/test.csv");

    const auto json =
        readFile(directory + "/test.json");

    assert(txt.find("TEXT ANALYSIS REPORT") !=
           std::string::npos);

    assert(csv.find("word_frequency") !=
           std::string::npos);

    assert(json.find("\"top_words\"") !=
           std::string::npos);

    assert(json.find("\"words\": 4") !=
           std::string::npos);

    std::filesystem::remove_all(directory);

    std::cout
        << "All report generator tests passed!\n";
}