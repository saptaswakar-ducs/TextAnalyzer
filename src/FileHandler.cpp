
#include "FileHandler.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

bool FileHandler::fileExists(
    const std::string& filePath
) {
    std::error_code error;

    return std::filesystem::is_regular_file(
        filePath, error
    );
}

std::string FileHandler::readFile(
    const std::string& filePath
) {
    std::ifstream file(
        filePath,
        std::ios::in | std::ios::binary
    );

    if (!file.is_open()) {
        throw std::runtime_error(
            "Unable to open file: " + filePath
        );
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    if (file.bad()) {
        throw std::runtime_error(
            "Error while reading file: " + filePath
        );
    }

    return buffer.str();
}