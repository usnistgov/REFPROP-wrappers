#include <string>
#include <fstream>
#include "utils.h"

bool file_exists(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

std::string parent_path(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");          // find last occurrence of a path separator
    if (pos != std::string::npos) {
        return filepath.substr(0, pos);
    }
    return "";                                          // return empty string if there's no parent path
}

std::string StripTrailingWhiteSpace(std::string str) {
    size_t pos = str.find_last_not_of(" \t\n\r\f\v");   // find last non-whitespace character
    if (pos != std::string::npos) {
        str.erase(pos + 1);                             // erase trailing whitespace
    }
    else {
        str.clear();                                    // clear string if all whitespace
    }
    return str;
}

std::string StripTrailingWhiteSpaceAndPipes(std::string str) {
    size_t pos = str.find_last_not_of(" |\t\n\r\f\v");   // find last non-whitespace/pipe character
    if (pos != std::string::npos) {
        str.erase(pos + 1);                             // erase trailing whitespace
    }
    else {
        str.clear();                                    // clear string if all whitespace
    }
    return str;
}
