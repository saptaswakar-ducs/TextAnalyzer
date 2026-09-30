
cmake_minimum_required(VERSION 3.16)

project(TextAnalyzer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

target_include_directories(text_analyzer
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

enable_testing()

add_executable(test_statistics
    tests/test_statistics.cpp
    src/Statistics.cpp
    src/Tokenizer.cpp
)

target_include_directories(test_statistics
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME StatisticsTests
    COMMAND test_statistics
)

add_executable(test_word_frequency
    tests/test_word_frequency.cpp
    src/WordFrequency.cpp
    src/Tokenizer.cpp
)

target_include_directories(test_word_frequency
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME WordFrequencyTests
    COMMAND test_word_frequency
)

add_executable(test_text_search
    tests/test_text_search.cpp
    src/TextSearch.cpp
)

target_include_directories(test_text_search
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME TextSearchTests
    COMMAND test_text_search
)

if(MSVC)
    target_compile_options(test_text_search PRIVATE /W4)
else()
    target_compile_options(test_text_search
        PRIVATE -Wall -Wextra -Wpedantic
    )
endif()


add_executable(test_readability
    tests/test_readability.cpp
    src/ReadabilityAnalyzer.cpp
)

target_include_directories(test_readability
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME ReadabilityTests
    COMMAND test_readability
)

add_executable(test_report_generator
    tests/test_report_generator.cpp
    src/ReportGenerator.cpp
)

target_include_directories(test_report_generator
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME ReportGeneratorTests
    COMMAND test_report_generator
)

foreach(target
    test_readability
    test_report_generator
)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target}
            PRIVATE -Wall -Wextra -Wpedantic
        )
    endif()
endforeach()

set_tests_properties(
    WordFrequencyTests
    PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

foreach(target
    text_analyzer
    test_statistics
    test_word_frequency
)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(
            ${target}
            PRIVATE -Wall -Wextra -Wpedantic
        )
    endif()
endforeach()