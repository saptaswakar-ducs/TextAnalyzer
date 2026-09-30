# Text Analyzer

A modular command-line text analysis application built with **C++17** and **CMake**. The project progressively implements text statistics, word-frequency analysis, text searching with KMP, readability analysis, and report generation.

> **Scope:** This README documents **Phases 1–6 only**. Phase 7 performance/large-file optimization features are intentionally excluded.

---

## Features

### Core Text Analysis
- Character, word, line, and sentence counting
- Paragraph counting
- Unique-word counting
- Longest and shortest word detection
- Average word length
- Average sentence length
- Estimated reading time
- Lexical diversity

### Word Frequency Analysis
- Frequency of every word
- Top-K most frequent words
- Case-insensitive word queries
- Stop-word filtering
- Hapax legomena detection
- Relative word frequency
- Deterministic alphabetical ordering for frequency ties

### Text Search
- Case-sensitive and case-insensitive search
- Whole-word matching
- Overlapping occurrence detection
- Character/byte offset reporting
- Line and column reporting
- Standard string search
- Knuth-Morris-Pratt (KMP) pattern matching

### Readability Analysis
- Syllable-count estimation
- Flesch Reading Ease
- Flesch-Kincaid Grade Level
- Readability difficulty classification

### Report Generation
- TXT reports
- CSV reports
- JSON reports
- Statistics, readability results, and top-word frequencies in reports

### Testing
- CMake/CTest-based unit tests
- Statistics tests
- Word-frequency tests
- Text-search tests
- Readability tests
- Report-generation tests

---

## Project Architecture

```text
                         +----------------------+
                         |       main.cpp       |
                         |   CLI / Menu System  |
                         +----------+-----------+
                                    |
                                    v
                         +----------------------+
                         |    TextAnalyzer      |
                         |   Main Facade Class  |
                         +----------+-----------+
                                    |
              +---------------------+----------------------+
              |                     |                      |
              v                     v                      v
      +---------------+     +---------------+     +----------------+
      |   Tokenizer   |     |  Statistics   |     | WordFrequency  |
      +---------------+     +---------------+     +----------------+
              |                     |                      |
              +---------------------+----------------------+
                                    |
              +---------------------+----------------------+
              |                                            |
              v                                            v
      +---------------+                            +-------------------+
      |  TextSearch   |                            | Readability       |
      | Standard/KMP  |                            | Analyzer          |
      +---------------+                            +-------------------+
              |                                            |
              +---------------------+----------------------+
                                    |
                                    v
                         +----------------------+
                         |  ReportGenerator     |
                         | TXT / CSV / JSON     |
                         +----------------------+
```

---

## Directory Structure

```text
TextAnalyzer/
│
├── CMakeLists.txt
│
├── include/
│   ├── TextAnalyzer.h
│   ├── FileHandler.h
│   ├── Tokenizer.h
│   ├── Statistics.h
│   ├── WordFrequency.h
│   ├── TextSearch.h
│   ├── ReadabilityAnalyzer.h
│   └── ReportGenerator.h
│
├── src/
│   ├── main.cpp
│   ├── TextAnalyzer.cpp
│   ├── FileHandler.cpp
│   ├── Tokenizer.cpp
│   ├── Statistics.cpp
│   ├── WordFrequency.cpp
│   ├── TextSearch.cpp
│   ├── ReadabilityAnalyzer.cpp
│   └── ReportGenerator.cpp
│
├── tests/
│   ├── test_statistics.cpp
│   ├── test_word_frequency.cpp
│   ├── test_text_search.cpp
│   ├── test_readability.cpp
│   └── test_report_generator.cpp
│
├── data/
│   ├── sample.txt
│   ├── statistics_test.txt
│   └── stopwords.txt
│
├── reports/
│   └── generated reports
│
└── build/
    └── CMake build files
```

---

# Requirements

- C++17-compatible compiler
- CMake 3.16 or newer
- macOS, Linux, or another platform with standard C++17/CMake support

The project uses only the C++ standard library and does not require external C++ libraries.

---

# Building the Project

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

Run the application:

```bash
./build/text_analyzer
```

---

# Running Tests

The project uses CTest.

```bash
ctest --test-dir build --output-on-failure
```

Expected test suites:

```text
StatisticsTests
WordFrequencyTests
TextSearchTests
ReadabilityTests
ReportGeneratorTests
```

A successful build should report:

```text
100% tests passed, 0 tests failed
```

---

# Command-Line Interface

When the program starts, the main menu provides:

```text
===== TEXT ANALYZER =====
1. Enter text manually
2. Load a text file
3. Display text statistics
4. Display top 10 words
5. Display top 10 without stop words
6. Standard text search
7. KMP text search
8. Display readability analysis
9. Export analysis report
10. Exit
```

The exact menu numbering should match the final `main.cpp` implementation.

---

# Phase 1 — Basic Text Analyzer

The first phase established the project structure and basic CLI functionality.

## Main Goals

- Create a C++17 project
- Configure CMake
- Read text from the user
- Load text from files
- Count basic text properties
- Create the first `TextAnalyzer` class

The initial application could calculate:

- Characters
- Words
- Lines
- Sentences

This phase established the foundation for the later modular architecture.

---

# Phase 2 — File Handling and Tokenization

Phase 2 separated file operations and tokenization from the main analyzer.

## FileHandler

`FileHandler` provides:

```cpp
FileHandler::fileExists(path)
FileHandler::readFile(path)
```

It uses:

- `std::filesystem`
- `std::ifstream`
- `std::ostringstream`

File-reading errors are reported using C++ exceptions.

## Tokenizer

`Tokenizer` is responsible for converting raw text into useful tokens.

### Word normalization

The tokenizer:

1. Converts alphabetic characters to lowercase.
2. Keeps alphanumeric characters.
3. Converts punctuation and whitespace to separators.
4. Splits the normalized text into words.

For example:

```text
"Hello, WORLD!"
```

becomes:

```text
hello
world
```

The current tokenizer is intentionally simple and ASCII-oriented. It does not provide full Unicode linguistic processing.

### Sentence tokenization

Sentences are separated using:

```text
.
!
?
```

Empty sentence fragments are ignored.

---

# Phase 3 — Statistical Analysis

Phase 3 introduced the `Statistics` module.

The main result is represented by:

```cpp
struct TextStatistics {
    std::size_t characters;
    std::size_t words;
    std::size_t sentences;
    std::size_t lines;
    std::size_t paragraphs;
    std::size_t uniqueWords;

    std::string longestWord;
    std::string shortestWord;

    double averageWordLength;
    double averageSentenceLength;
    double readingTimeMinutes;
    double lexicalDiversity;
};
```

## Calculated Metrics

### Character count

The application uses the size of the original string:

```cpp
text.size()
```

This represents bytes rather than Unicode grapheme characters.

### Word count

The normalized word vector is used.

### Sentence count

The tokenizer's sentence vector is used.

### Line count

Lines are detected using newline characters.

### Paragraph count

Paragraphs are identified as blocks of non-blank text separated by blank lines.

### Unique words

An `std::unordered_set` is used to determine the number of unique words.

### Average word length

```text
total characters in words / number of words
```

### Average sentence length

```text
number of words / number of sentences
```

### Reading time

The project estimates reading time using approximately:

```text
words / 200
```

minutes.

### Lexical diversity

```text
unique words / total words
```

A value closer to 1 means a larger proportion of the vocabulary is unique within the analyzed text.

---

# Phase 4 — Word Frequency Analysis

Phase 4 introduced the `WordFrequency` class.

Internally, word counts are stored using:

```cpp
std::unordered_map<std::string, std::size_t>
```

For example:

```text
apple banana apple orange apple
```

produces:

```text
apple  -> 3
banana -> 1
orange -> 1
```

## Top-K Words

The application can return the most frequent K words.

When two words have the same frequency, alphabetical ordering is used as the tie-breaker.

For example:

```text
apple 3
banana 2
orange 2
```

## Stop Words

The project contains:

```text
data/stopwords.txt
```

Common words such as:

```text
the
a
is
and
of
to
in
```

can be excluded from frequency analysis.

## Hapax Legomena

A **hapax legomenon** is a word that occurs exactly once.

The application provides:

```cpp
getHapaxLegomena()
```

which returns all words whose frequency is exactly 1.

## Relative Frequency

For a word `w`:

```text
relative frequency =
frequency(w) / total number of words
```

This can also be displayed as a percentage.

---

# Phase 5 — Text Search

Phase 5 introduced the `TextSearch` module.

The search engine operates on the original text rather than normalized tokens. This is important because it allows the program to preserve actual character positions.

## Search Features

The user can specify:

- Search pattern
- Case sensitivity
- Whole-word matching
- Search algorithm

Example:

```text
Pattern: machine
Case-sensitive: No
Whole-word: Yes
```

## SearchMatch

Every match is represented by:

```cpp
struct SearchMatch {
    std::size_t position;
    std::size_t line;
    std::size_t column;
};
```

The position is a zero-based byte offset, while displayed line and column values are one-based.

## Standard Search

The standard implementation uses:

```cpp
std::string::find()
```

Multiple occurrences are detected by advancing the search position by one character, allowing overlapping matches.

For example:

```text
banana
```

Searching for:

```text
ana
```

finds two overlapping occurrences.

## Whole-Word Matching

Whole-word matching checks the characters immediately before and after the match.

Therefore:

```text
cat
```

matches:

```text
cat
cat.
(cat)
```

but does not match the substring in:

```text
catalog
```

## KMP Search

The project also implements the **Knuth-Morris-Pratt** algorithm.

KMP preprocesses the pattern using an:

```text
LPS
Longest Proper Prefix which is also a Suffix
```

array.

The LPS table allows KMP to avoid rechecking characters that have already been matched.

### Complexity

For a text of length `N` and pattern of length `M`:

```text
LPS preprocessing: O(M)
KMP matching:      O(N)
Total:             O(N + M)
```

The project includes tests verifying that standard search and KMP return the same match positions.

---

# Phase 6 — Readability Analysis

Phase 6 introduced readability metrics and report generation.

## Syllable Estimation

English syllables are difficult to determine perfectly from spelling alone.

The project therefore uses a heuristic based on:

- Vowel groups
- Silent final `e`
- Common exceptions

For example:

```text
hello → approximately 2 syllables
beautiful → approximately 3 syllables
```

The implementation should be considered an estimate rather than a linguistic pronunciation engine.

---

## Flesch Reading Ease

The project implements:

```text
206.835
- 1.015 × (words / sentences)
- 84.6 × (syllables / words)
```

Higher values generally correspond to easier-to-read English prose under this formula.

---

## Flesch-Kincaid Grade Level

The project also implements:

```text
0.39 × (words / sentences)
+ 11.8 × (syllables / words)
- 15.59
```

This provides an approximate US grade-level measure.

---

## Difficulty Classification

The Flesch Reading Ease score is classified approximately as:

| Score | Classification |
|---:|---|
| 90–100 | Very easy |
| 80–89 | Easy |
| 70–79 | Fairly easy |
| 60–69 | Standard |
| 50–59 | Fairly difficult |
| 30–49 | Difficult |
| 0–29 | Very difficult |

These classifications are intended for English prose and should not be treated as exact assessments of an individual reader's ability.

---

# Report Generation

The `ReportGenerator` module exports analysis results in three formats.

## TXT

Human-readable report containing:

- Text statistics
- Readability results
- Top words
- Notes about estimated readability

Example:

```text
TEXT ANALYSIS REPORT
====================

TEXT STATISTICS
Characters: 120
Words: 25
Sentences: 4
Lines: 4
Paragraphs: 2
Unique words: 18

READABILITY
Syllables: 32
Flesch Reading Ease: 72.50
Flesch-Kincaid Grade: 6.20
Difficulty: Fairly easy
```

## CSV

CSV reports use a structure similar to:

```text
section,metric,value
statistics,words,25
statistics,sentences,4
statistics,unique_words,18
readability,flesch_reading_ease,72.5000
```

This makes the report convenient for spreadsheet and data-analysis tools.

## JSON

JSON provides a structured representation:

```json
{
  "statistics": {
    "characters": 120,
    "words": 25,
    "sentences": 4
  },
  "readability": {
    "valid": true,
    "total_syllables": 32,
    "flesch_reading_ease": 72.5
  },
  "top_words": [
    {
      "word": "text",
      "count": 4
    }
  ]
}
```

The report generator also performs escaping for special characters in JSON and CSV values.

---

# Testing Strategy

The project uses separate executable test programs registered with CTest.

## Statistics Tests

`tests/test_statistics.cpp`

Tests include:

- Empty input
- Word counts
- Sentence counts
- Unique words
- Longest/shortest words
- Average word length
- Lexical diversity
- Paragraph counting
- Line counting

## Word Frequency Tests

`tests/test_word_frequency.cpp`

Tests include:

- Frequency calculation
- Case-insensitive queries
- Top-K extraction
- Relative frequency
- Hapax legomena
- Stop-word filtering
- Empty input

## Text Search Tests

`tests/test_text_search.cpp`

Tests include:

- Case-sensitive search
- Case-insensitive search
- Whole-word matching
- Overlapping matches
- Line/column calculation
- Standard search vs KMP

## Readability Tests

`tests/test_readability.cpp`

Tests include:

- Syllable estimation
- Readability calculation
- Empty text handling

## Report Tests

`tests/test_report_generator.cpp`

Tests verify:

- TXT file creation
- CSV file creation
- JSON file creation
- Expected report fields
- Generated content

---

# CMake Configuration

The project uses CMake to manage the application and tests.

The main executable includes:

```cmake
add_executable(text_analyzer
    src/main.cpp
    src/TextAnalyzer.cpp
    src/FileHandler.cpp
    src/Tokenizer.cpp
    src/Statistics.cpp
    src/WordFrequency.cpp
    src/TextSearch.cpp
    src/ReadabilityAnalyzer.cpp
    src/ReportGenerator.cpp
)
```

The include directory is configured with:

```cmake
target_include_directories(
    text_analyzer
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)
```

Testing is enabled with:

```cmake
enable_testing()
```

Each test executable is registered using:

```cmake
add_test(...)
```

The project uses warning flags such as:

```text
-Wall
-Wextra
-Wpedantic
```

when compiled with GCC/Clang.

---

# Example Workflow

A typical session looks like:

```text
1. Start application
2. Load a text file
3. Display text statistics
4. Display top 10 words
5. Display top 10 without stop words
6. Search for a word using standard search
7. Search for a pattern using KMP
8. Display readability analysis
9. Export a TXT/CSV/JSON report
10. Exit
```

For example:

```text
Choose an option: 2
Enter file path: data/sample.txt

File loaded successfully.

Choose an option: 3

===== TEXT STATISTICS =====
Characters: ...
Words: ...
Sentences: ...
Lines: ...
Paragraphs: ...
Unique words: ...
```

Then:

```text
Choose an option: 9

Choose report format:
1. TXT
2. CSV
3. JSON
Option: 3

Enter report name: analysis

Report exported: reports/analysis.json
```

---

# Design Principles

The project follows a modular design rather than putting all functionality inside `main.cpp`.

### Separation of concerns

| Module | Responsibility |
|---|---|
| `FileHandler` | File access |
| `Tokenizer` | Word/sentence tokenization |
| `Statistics` | Statistical calculations |
| `WordFrequency` | Frequency analysis |
| `TextSearch` | Pattern matching |
| `ReadabilityAnalyzer` | Readability calculations |
| `ReportGenerator` | Report export |
| `TextAnalyzer` | Coordinates analysis modules |
| `main.cpp` | User interaction |

This makes individual components easier to test, understand and extend.

---

# Technologies Used

- **C++17**
- **CMake**
- **STL**
  - `std::vector`
  - `std::string`
  - `std::unordered_map`
  - `std::unordered_set`
  - `std::filesystem`
  - `std::algorithm`
  - `std::fstream`
- **CTest**
- Object-oriented programming
- Exception handling
- KMP pattern matching
- File I/O
- Basic statistical analysis
- JSON/CSV/TXT serialization

---

# Limitations

The current implementation intentionally has several limitations:

1. **English-oriented readability analysis**  
   Syllable estimation and Flesch formulas are designed for English text.

2. **ASCII-oriented tokenization**  
   The tokenizer does not provide complete Unicode linguistic support.

3. **Byte-based character positions**  
   Search offsets refer to positions within the underlying `std::string`.

4. **Simple sentence detection**  
   Sentence boundaries are based primarily on `.`, `!`, and `?`.

5. **Heuristic syllable counting**  
   Pronunciation cannot be determined perfectly from spelling alone.

6. **Stop words are file-based**  
   The default stop-word list is loaded from `data/stopwords.txt`.

---

# Possible Future Extensions

The project can be extended with:

- Unicode-aware text processing
- Advanced NLP tokenization
- Stemming and lemmatization
- N-gram analysis
- TF-IDF analysis
- Sentiment analysis
- Named Entity Recognition
- Word-frequency charts
- Interactive GUI using Qt
- Web-based visualization
- More advanced readability formulas
- Configurable stop-word dictionaries
- PDF/DOCX input
- Additional export formats

---

# Learning Outcomes

This project provides practical experience with:

- Designing a modular C++ application
- Object-oriented programming
- C++17 STL containers and algorithms
- File handling
- String processing
- Hash-based frequency analysis
- Pattern matching algorithms
- KMP and LPS preprocessing
- Basic NLP concepts
- Statistical text analysis
- Readability formulas
- Serialization
- JSON and CSV generation
- CMake project management
- Automated unit testing with CTest
- Exception handling
- Separation of concerns

---

# Author

**Saptaswa Kar**

MSc Computer Science  
University of Delhi

---

## License

This project is intended primarily for educational and portfolio purposes. Add an explicit open-source license such as MIT if you plan to distribute the project publicly. 
2026, Saptaswa Kar. All rights reserved.
