
#include "TextSearch.h"

#include <cctype>
#include <utility>

TextSearch::TextSearch(std::string input)
    : text(std::move(input)) {}

std::string TextSearch::toLower(
    const std::string& input
) {
    std::string result = input;

    for (char& ch : result) {
        ch = static_cast<char>(
            std::tolower(
                static_cast<unsigned char>(ch)
            )
        );
    }

    return result;
}

bool TextSearch::isWordCharacter(
    unsigned char ch
) {
    return std::isalnum(ch) || ch == '_';
}

bool TextSearch::isWholeWordMatch(
    std::size_t position,
    std::size_t length
) const {
    const bool leftBoundary =
        position == 0 ||
        !isWordCharacter(
            static_cast<unsigned char>(
                text[position - 1]
            )
        );

    const std::size_t end = position + length;

    const bool rightBoundary =
        end == text.size() ||
        !isWordCharacter(
            static_cast<unsigned char>(
                text[end]
            )
        );

    return leftBoundary && rightBoundary;
}

SearchMatch TextSearch::makeMatch(
    std::size_t position
) const {
    SearchMatch match{position, 1, 1};

    for (std::size_t i = 0; i < position; ++i) {
        if (text[i] == '\n') {
            ++match.line;
            match.column = 1;
        } else {
            ++match.column;
        }
    }

    return match;
}


std::vector<SearchMatch> TextSearch::findAll(
    const std::string& pattern,
    bool caseSensitive,
    bool wholeWord
) const {
    std::vector<SearchMatch> matches;

    if (pattern.empty()) {
        return matches;
    }

    const std::string source =
        caseSensitive ? text : toLower(text);

    const std::string target =
        caseSensitive ? pattern : toLower(pattern);

    std::size_t position = 0;

    while (
        (position = source.find(target, position))
        != std::string::npos
    ) {
        if (!wholeWord ||
            isWholeWordMatch(position, target.size())) {
            matches.push_back(makeMatch(position));
        }

        // Advance by one to allow overlapping matches.
        ++position;
    }

    return matches;
}


std::vector<std::size_t> TextSearch::buildLPS(
    const std::string& pattern
) {
    std::vector<std::size_t> lps(
        pattern.size(), 0
    );

    std::size_t length = 0;
    std::size_t i = 1;

    while (i < pattern.size()) {
        if (pattern[i] == pattern[length]) {
            ++length;
            lps[i] = length;
            ++i;
        }
        else if (length != 0) {
            length = lps[length - 1];
        }
        else {
            lps[i] = 0;
            ++i;
        }
    }

    return lps;
}


std::vector<SearchMatch> TextSearch::findAllKMP(
    const std::string& pattern,
    bool caseSensitive,
    bool wholeWord
) const {
    std::vector<SearchMatch> matches;

    if (pattern.empty()) {
        return matches;
    }

    const std::string source =
        caseSensitive ? text : toLower(text);

    const std::string target =
        caseSensitive ? pattern : toLower(pattern);

    const auto lps = buildLPS(target);

    std::size_t i = 0;
    std::size_t j = 0;

    while (i < source.size()) {
        if (source[i] == target[j]) {
            ++i;
            ++j;
        }

        if (j == target.size()) {
            const std::size_t position = i - j;

            if (!wholeWord ||
                isWholeWordMatch(position, target.size())) {
                matches.push_back(makeMatch(position));
            }

            j = lps[j - 1];
        }
        else if (
            i < source.size() &&
            source[i] != target[j]
        ) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
    }

    return matches;
}