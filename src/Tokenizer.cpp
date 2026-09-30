
#include "Tokenizer.h"

#include <cctype>
#include <sstream>

std::string Tokenizer::normalize(
    const std::string& text
) {
    std::string result;
    result.reserve(text.size());

    for (unsigned char ch : text) {
        if (std::isalnum(ch)) {
            result += static_cast<char>(
                std::tolower(ch)
            );
        } else if (std::isspace(ch) ||
                   std::ispunct(ch)) {
            result += ' ';
        }
    }

    return result;
}

std::vector<std::string> Tokenizer::tokenizeWords(
    const std::string& text
) {
    std::vector<std::string> words;

    std::string normalized = normalize(text);

    std::istringstream stream(normalized);

    std::string word;

    while (stream >> word) {
        words.push_back(word);
    }

    return words;
}

std::vector<std::string> Tokenizer::tokenizeSentences(
    const std::string& text
) {
    std::vector<std::string> sentences;

    std::string current;

    for (char ch : text) {
        if (ch == '.' || ch == '!' || ch == '?') {
            std::size_t start =
                current.find_first_not_of(
                    " \t\n\r"
                );

            if (start != std::string::npos) {
                std::size_t end =
                    current.find_last_not_of(
                        " \t\n\r"
                    );

                sentences.push_back(
                    current.substr(
                        start,
                        end - start + 1
                    )
                );
            }

            current.clear();
        } else {
            current += ch;
        }
    }

    std::size_t start =
        current.find_first_not_of(" \t\n\r");

    if (start != std::string::npos) {
        std::size_t end =
            current.find_last_not_of(" \t\n\r");

        sentences.push_back(
            current.substr(start, end - start + 1)
        );
    }

    return sentences;
}