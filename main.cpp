#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <cstdlib>
#include <vector>

class Token : public std::string {
public:
    std::string type;

    Token(const std::string& s = "", const std::string& t = "") : std::string(s), type(t) {}
};

void panic(std::string err) {
    std::cerr << "\033[1;31m" << err << "\033[0m" << std::endl;
    std::exit(EXIT_FAILURE);
}

std::vector<Token> split(const std::string& s, char delimiter) {
    std::vector<Token> tokens;
    Token token("");
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
                token.type = "char";
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


void build(std::string input_path, std::string output_path) {
    std::cout << "Compiling " << input_path << " to " << output_path << std::endl;

    std::ifstream input_file(input_path);
    if (!input_file) {
        panic("Error opening file: " + input_path);
    }

    std::ofstream output_file(output_path, std::ofstream::trunc);
    output_file << "section .text \n" <<
        "global _start \n" <<
        "_start: \n";

    std::string line;
    int string_literal_count = 0;
    int line_number = 1;
    std::vector<std::string> data_section = {};
    while(getline(input_file, line)) {
        std::vector tokens = split(line, ' ');

        if (tokens[0] == "exit") {
            output_file << "mov rax, 60 \n" <<
                "mov rdi, " << tokens[1] << " \n" <<
                "syscall \n" <<
                "section .data \n";
            for (std::string data_item : data_section) {
                output_file << data_item << std::endl;
            }
        } else if (tokens[0] == "#") {
        } else if (tokens[0] == "_") {
            tokens.erase(tokens.begin());
            for (Token token : tokens) {
                output_file << token << " ";
            }
            output_file << std::endl;
        } else if(tokens[0] == "print") {
            string_literal_count += 1;

            tokens.erase(tokens.begin());
            std::string constructed_msg = "lit" + std::to_string(string_literal_count) + " db ";

            int msg_size = 0;
            for (int i = 0; i < tokens.size(); i++) {
                Token token = tokens[i];

                if (token.type == "string") {
                    constructed_msg += "\"" + token + "\"";
                    msg_size += token.size();
                } else if (token.type == "char") {
                    constructed_msg += token;
                    msg_size += 1;
                } else {
                    panic("Incorrect data type (line " + std::to_string(line_number) + "): `" + token.type + "` can not be fed into print.");
                }

                if (i != tokens.size() - 1) {
                    constructed_msg += ", ";
                }
            }
            output_file << "mov rax, 1 \n" <<
                "mov rdi, 1 \n" <<
                "lea rsi, [rel lit" + std::to_string(string_literal_count) << "] \n" <<
                "mov rdx, " << msg_size << " \n" <<
                "syscall \n";
            data_section.push_back(constructed_msg);
        }
        else {
            panic("Invalid syntax (line " + std::to_string(line_number) + "): `" + tokens[0] + "` is not a recognised command.");
        }
        line_number++;
    }

    input_file.close();
    output_file.close();
}

void help() {
    panic("You did something wrong.");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];

        if (arg == "build") {
            if (argc == 3) {
                std::string input_path = argv[2];
                std::size_t pos = input_path.rfind('.');

                std::string output_path = input_path.substr(0, pos) + ".asm";

                build(argv[2], output_path);
            } else if (argc == 4) {
                build(argv[2], argv[3]);
            } else {
                help();
            }
        } else {
            help();
        }
    } else {
        help();
    }

    return 0;
}
