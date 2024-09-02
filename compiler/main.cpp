#include <cstddef>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "frontend.h"
#include "token.h"
#include "build.h"

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
