#pragma once

#include <string>
#include <iostream>

void panic(std::string err) {
    std::cerr << "\033[1;31m" << err << "\033[0m" << std::endl;
    std::exit(EXIT_FAILURE);
}