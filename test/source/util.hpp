#ifndef UTIL_HPP
#define UTIL_HPP

#include <regex>
#include <string>

void minifyHtml(std::string& str) {
    //std::cout << "HTML template minified from " << str.size();
    const std::regex rx2("\\s+<");
    const std::regex rx3(">\\s+");
    //_htmlTemplate = std::regex_replace(_htmlTemplate, rx1, "");
    str = std::regex_replace(str, rx2, "<");
    str = std::regex_replace(str, rx3, ">");
    //std::cout << " to " << str.size() << " bytes" << std::endl;
    str.shrink_to_fit();
}

std::string readHtmlFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    minifyHtml(str);
    return str;
}

#endif // UTIL_HPP
