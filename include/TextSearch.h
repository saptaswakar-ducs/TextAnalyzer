
#ifndef TEXT_SEARCH_H
#define TEXT_SEARCH_H

#include <cstddef>
#include <string>
#include <vector>

struct SearchMatch {
    std::size_t position;
    std::size_t line;
    std::size_t column;
};

class TextSearch {
public:
    explicit TextSearch(std::string text);

    std::vector<SearchMatch> findAll(
        const std::string& pattern,
        bool caseSensitive = false,
        bool wholeWord = false
    ) const;

    std::vector<SearchMatch> findAllKMP(
        const std::string& pattern,
        bool caseSensitive = false,
        bool wholeWord = false
    ) const;

private:
    std::string text;

    static std::string toLower(
        const std::string& input
    );

    static bool isWordCharacter(
        unsigned char ch
    );

    bool isWholeWordMatch(
        std::size_t position,
        std::size_t length
    ) const;

    SearchMatch makeMatch(
        std::size_t position
    ) const;

    static std::vector<std::size_t> buildLPS(
        const std::string& pattern
    );
};

#endif