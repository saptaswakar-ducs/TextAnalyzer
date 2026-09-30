
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

class FileHandler {
public:
    static std::string readFile(
        const std::string& filePath
    );

    static bool fileExists(
        const std::string& filePath
    );
};

#endif