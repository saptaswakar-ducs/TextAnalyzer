
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

class Tokenizer {
public:
    static std::string normalize(
        const std::string& text
    );

    static std::vector<std::string> tokenizeWords(
        const std::string& text
    );

    static std::vector<std::string> tokenizeSentences(
        const std::string& text
    );
};

#endif