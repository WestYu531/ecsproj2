#include "StringUtils.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>
#include <iostream>

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    ssize_t len = str.length();
    if (end == 0) end = len; 
    if (start < 0) start += len;
    if (end < 0) end += len;
    if (start < 0) start = 0;
    if (end > len) end = len;
    if (start >= end) return "";
    return str.substr(start, end - start);
}

std::string Capitalize(const std::string &str) noexcept{
    if (str.empty()) return str;  // empty str

    std::string result = str;
    result[0] = std::toupper(result[0]);  // capitalize the string
    std::transform(result.begin() + 1, result.end(), result.begin() + 1, ::tolower);// let rest of words not capitalized
    return result;
}

std::string Upper(const std::string &str) noexcept{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string Lower(const std::string &str) noexcept{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string LStrip(const std::string &str) noexcept{
    size_t start = 0;
    while (start < str.length() && std::isspace(str[start])) {
        start++;
    }
    return str.substr(start);
}

std::string RStrip(const std::string &str) noexcept{
    size_t end = str.length();
    while (end > 0 && std::isspace(str[end - 1])) {
        end--;
    }
    return str.substr(0, end); 
}

std::string Strip(const std::string &str) noexcept{
    return LStrip(RStrip(str));
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    if (width <= static_cast<int>(str.length())) return str;  // width <= string length return string

    int totalNum = width - str.length();  // count the total num of fill
    int leftNum = totalNum / 2;           // left fill
    int rightNum = totalNum - leftNum;    // right fill

    return std::string(leftNum, fill) + str + std::string(rightNum, fill);
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
if (width <= static_cast<int>(str.length())) return str;

    return str + std::string(width - str.length(), fill);
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    if (width <= static_cast<int>(str.length())) return str;

    return std::string(width - str.length(), fill) + str;
}


std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    if (old.empty()) return str;  // aviod infinte loop

    std::string result = str;
    size_t pos = 0;

    while ((pos = result.find(old, pos)) != std::string::npos) {
        result.replace(pos, old.length(), rep);
        pos += rep.length();  // moove position after rep, avoid infinite loop
    }

    return result;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    std::vector<std::string> result;

    // if splt is empty，splt is space
    if (splt.empty()) {
        std::istringstream iss(str);
        std::string word;
        while (iss >> word) {
            result.push_back(word);
        }
        return result;
    }

    // if splt is not empty
    size_t start = 0, end;
    while ((end = str.find(splt, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + splt.length();
    }
    result.push_back(str.substr(start));  // add the last part

    return result;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    if (vect.empty()) return "";  // empty vector

    std::string result = vect[0];  // add the fist element

    for (size_t i = 1; i < vect.size(); ++i) { 
        result += str + vect[i];
    }

    return result;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    std::string result;

    for (char ch : str) {
        if (ch == '\t') {
            result.append(tabsize - result.length() % tabsize, ' '); // replace \t with tabsize's tab
        } else {
            result.push_back(ch);
        }
    }

    return result;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    size_t m = left.length();
    size_t n = right.length();
    
    // preprocess left and right
    std::string l = left, r = right;
    if (ignorecase) {
        std::transform(l.begin(), l.end(), l.begin(), ::tolower);
        std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    }

    // initilize the dp table
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    // initilize margin
    for (size_t i = 0; i <= m; ++i) dp[i][0] = i;  // delete all
    for (size_t j = 0; j <= n; ++j) dp[0][j] = j;  // insert a;;

    // fill up the dp table
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (l[i - 1] == r[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // same char
            } else {
                dp[i][j] = std::min({
                    dp[i - 1][j] + 1,  // delete
                    dp[i][j - 1] + 1,  // insert
                    dp[i - 1][j - 1] + 1  // replace
                });
            }
        }
    }

    return dp[m][n];
}

};