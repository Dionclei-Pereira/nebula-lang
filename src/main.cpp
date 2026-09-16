#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: nebula <file.nb>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: could not open the file";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string code = buffer.str();
    std::cout << code;

    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();
    std::cout << "\n";
    for (const Token& token : tokens) {
        std::cout << static_cast<int>(token.type) << ": " << token.value << "\n" ;
    }
    return 0;
}