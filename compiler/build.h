#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "frontend.h"
#include "token.h"

void build(std::string input_path, std::string output_path) {

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
            std::string constructed_data = "lit_" + std::to_string(string_literal_count) + " db ";

            int data_size = 0;
            for (int i = 0; i < tokens.size(); i++) {
                Token token = tokens[i];

                if (token.type == "string") {
                    constructed_data += "\"" + token + "\"";
                    data_size += token.size();
                } else if (token.type == "value") {
                    constructed_data += token;
                    data_size += 1;
                } else {
                    panic("Incorrect data type (line " + std::to_string(line_number) + "): `" + token.type + "` can not be fed into print.");
                }

                if (i != tokens.size() - 1) {
                    constructed_data += ", ";
                }
            }
            output_file << "mov rax, 1 \n" <<
                "mov rdi, 1 \n" <<
                "lea rsi, [rel lit_" + std::to_string(string_literal_count) << "] \n" <<
                "mov rdx, " << data_size << " \n" <<
                "syscall \n";
            data_section.push_back(constructed_data);
        } else {
            panic("Invalid syntax (line " + std::to_string(line_number) + "): `" + tokens[0] + "` is not a recognised command.");
        }
        line_number++;
    }

    input_file.close();
    output_file.close();
}