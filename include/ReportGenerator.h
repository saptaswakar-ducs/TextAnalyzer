
#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "ReadabilityAnalyzer.h"
#include "Statistics.h"
#include "WordFrequency.h"

#include <cstddef>
#include <string>
#include <vector>

struct AnalysisReport {
    TextStatistics statistics;
    ReadabilityResult readability;

    std::vector<WordFrequency::FrequencyEntry>
        topWords;
};

class ReportGenerator {
public:
    static void exportTXT(
        const AnalysisReport& report,
        const std::string& path
    );

    static void exportCSV(
        const AnalysisReport& report,
        const std::string& path
    );

    static void exportJSON(
        const AnalysisReport& report,
        const std::string& path
    );

private:
    static std::string escapeJSON(
        const std::string& value
    );

    static std::string escapeCSV(
        const std::string& value
    );
};

#endif