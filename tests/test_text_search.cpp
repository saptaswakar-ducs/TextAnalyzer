
#include "TextSearch.h"

#include <cassert>
#include <iostream>
#include <vector>

void testCaseInsensitive() {
    TextSearch search("Hello hello HELLO");

    const auto matches = search.findAll("hello");

    assert(matches.size() == 3);
    assert(matches[0].position == 0);
    assert(matches[1].position == 6);
    assert(matches[2].position == 12);
}

void testCaseSensitive() {
    TextSearch search("Hello hello HELLO");

    const auto matches =
        search.findAll("Hello", true);

    assert(matches.size() == 1);
    assert(matches[0].position == 0);
}

void testWholeWord() {
    TextSearch search("cat catalog cat.");

    const auto matches =
        search.findAll("cat", false, true);

    assert(matches.size() == 2);
    assert(matches[0].position == 0);
    assert(matches[1].position == 12);
}

void testOverlappingMatches() {
    TextSearch search("banana");

    const auto matches = search.findAll("ana");
    const auto kmp = search.findAllKMP("ana");

    assert(matches.size() == 2);
    assert(kmp.size() == 2);

    assert(matches[0].position == 1);
    assert(matches[1].position == 3);

    assert(kmp[0].position == 1);
    assert(kmp[1].position == 3);
}

void testLineAndColumn() {
    TextSearch search("Hello\nworld\nHello");

    const auto matches = search.findAll("Hello");

    assert(matches.size() == 2);

    assert(matches[0].line == 1);
    assert(matches[0].column == 1);

    assert(matches[1].line == 3);
    assert(matches[1].column == 1);
}

void testAlgorithmsAgree() {
    const std::vector<std::string> texts = {
        "",
        "aaaaaa",
        "ABABCABAB",
        "Hello world. HELLO again!",
        "cat catalog cat",
        "banana banana"
    };

    const std::vector<std::string> patterns = {
        "",
        "a",
        "ana",
        "hello",
        "cat",
        "ABAB",
        "missing"
    };

    for (const auto& text : texts) {
        TextSearch search(text);

        for (const auto& pattern : patterns) {
            for (bool caseSensitive : {false, true}) {
                for (bool wholeWord : {false, true}) {
                    const auto standard = search.findAll(
                        pattern, caseSensitive, wholeWord
                    );

                    const auto kmp = search.findAllKMP(
                        pattern, caseSensitive, wholeWord
                    );

                    assert(standard.size() == kmp.size());

                    for (std::size_t i = 0;
                         i < standard.size(); ++i) {
                        assert(
                            standard[i].position ==
                            kmp[i].position
                        );
                    }
                }
            }
        }
    }
}

int main() {
    testCaseInsensitive();
    testCaseSensitive();
    testWholeWord();
    testOverlappingMatches();
    testLineAndColumn();
    testAlgorithmsAgree();

    std::cout << "All text search tests passed!\n";
}