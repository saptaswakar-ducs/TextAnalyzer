
#include "FileHandler.h"
#include "TextAnalyzer.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

std::string readManualInput() {
    std::cout << "\nEnter text.\n";
    std::cout << "Type END on a new line to finish.\n\n";

    std::string text;
    std::string line;
    bool firstLine = true;

    while (std::getline(std::cin, line)) {
        if (line == "END") {
            break;
        }

        if (!firstLine) {
            text += '\n';
        }

        text += line;
        firstLine = false;
    }

    return text;
}

int main() {
    std::unique_ptr<TextAnalyzer> analyzer;

    while (true) {
        std::cout
            << "\n===== TEXT ANALYZER =====\n"
            << "1. Enter text manually\n"
            << "2. Load a text file\n"
            << "3. Display text statistics\n"
            << "4. Display top 10 words\n"
            << "5. Display top 10 without stop words\n"
            << "6. Standard text search\n"
            << "7. KMP text search\n"
            << "8. Display readability analysis\n"
            << "9. Export analysis report\n"
            << "10. Exit\n"
            << "Choose an option: ";

        std::string choice;

        if (!std::getline(std::cin, choice)) {
            break;
        }

        try {
            if (choice == "1") {
                analyzer = std::make_unique<TextAnalyzer>(
                    readManualInput()
                );

                std::cout << "Text loaded successfully.\n";
            }
            else if (choice == "2") {
                std::cout << "Enter file path: ";

                std::string path;

                if (!std::getline(std::cin, path)) {
                    break;
                }

                std::string text =
                    FileHandler::readFile(path);

                analyzer = std::make_unique<TextAnalyzer>(
                    std::move(text)
                );

                std::cout << "File loaded successfully.\n";
            }
            else if (choice == "3") {
                if (!analyzer) {
                    std::cout << "Load text first.\n";
                    continue;
                }

                analyzer->displayStatistics();
            }
            else if (choice == "4") {
                if (!analyzer) {
                    std::cout << "Load text first.\n";
                    continue;
                }

                analyzer->displayWordFrequency(10, false);
            }
            else if (choice == "5") {
                if (!analyzer) {
                    std::cout << "Load text first.\n";
                    continue;
                }

                analyzer->displayWordFrequency(10, true);
            }
            else if (choice == "6" || choice == "7") {
                if (!analyzer) {
                    std::cout << "Load text first.\n";
                    continue;
                }

                std::string pattern;
                std::string answer;

                std::cout << "Enter search pattern: ";

                if (!std::getline(std::cin, pattern)) {
                    break;
                }

                if (pattern.empty()) {
                    std::cout
                        << "Search pattern cannot be empty.\n";
                    continue;
                }

                std::cout << "Case-sensitive? (y/n): ";

                if (!std::getline(std::cin, answer)) {
                    break;
                }

                const bool caseSensitive =
                    answer == "y" || answer == "Y";

                std::cout << "Whole-word matching? (y/n): ";

                if (!std::getline(std::cin, answer)) {
                    break;
                }

                const bool wholeWord =
                    answer == "y" || answer == "Y";

                const bool useKMP = choice == "7";

                analyzer->displaySearchResults(
                    pattern,
                    caseSensitive,
                    wholeWord,
                    useKMP
                );
            }
            
            else if (choice == "8") {
                if (!analyzer) {
                    std::cout << "Please load text first.\n";
                    continue;
                }

                analyzer->displayReadability();
            }
            else if (choice == "9") {
                if (!analyzer) {
                    std::cout << "Please load text first.\n";
                    continue;
                }

                std::cout
                    << "\nChoose report format:\n"
                    << "1. TXT\n"
                    << "2. CSV\n"
                    << "3. JSON\n"
                    << "Option: ";

                std::string formatChoice;

                if (!std::getline(std::cin, formatChoice)) {
                    break;
                }

                std::string format;
                std::string extension;

                if (formatChoice == "1") {
                    format = "txt";
                    extension = ".txt";
                }
                else if (formatChoice == "2") {
                    format = "csv";
                    extension = ".csv";
                }
                else if (formatChoice == "3") {
                    format = "json";
                    extension = ".json";
                }
                else {
                    std::cout << "Invalid format.\n";
                    continue;
                }

                std::cout << "Enter report name "
                            "(without extension): ";

                std::string name;

                if (!std::getline(std::cin, name)) {
                    break;
                }

                if (name.empty() ||
                    name == "." ||
                    name == ".." ||
                    name.find_first_of("/\\") != std::string::npos) {
                    std::cout << "Invalid report name.\n";
                    continue;
                }

                const std::string path =
                    "reports/" + name + extension;

                analyzer->exportReport(format, path);

                std::cout << "Report exported: "
                        << path << '\n';
            }
            else if (choice == "10") {
                std::cout << "Thanks for using me!!\n";
                break;
            }
            else {
                std::cout
                    << "Invalid option. Please try again.\n";
            }
        }
        catch (const std::exception& error) {
            std::cerr
                << "Error: " << error.what()
                << '\n';
        }
    }

    return 0;
}