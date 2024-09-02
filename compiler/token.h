#pragma once

#include <vector>
#include <string>

class Token : public std::string {
public:
    std::string type;
    
    Token(const std::string& s, const std::string& t) 
        : std::string(s), type(t) {}
};

std::vector<Token> split(const std::string& s, char delimiter) {
    std::vector<Token> tokens;
    Token token("", "");
    bool insideQuotes = false;

    for (char ch : s) {
        if (ch == '"') {
            insideQuotes = !insideQuotes;
            if (insideQuotes) {
                token.type = "string";
            }
        } else if (ch == delimiter && !insideQuotes) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
                token.type = "value";
            }
        } else {
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}