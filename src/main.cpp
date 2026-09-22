#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer.hpp"
#include "parser.hpp"

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
    std::cout << "\n\nTokens: \n";
    for (const Token& token : tokens) {
        std::cout << static_cast<int>(token.type) << ": " << token.value << "\n" ;
    }

    Parser parser(tokens);
    std::vector<std::unique_ptr<Statement>> statements = parser.parse(); 
    std::cout << "\nStatements: \n";
    for (const auto& st : statements) {
        if (auto* letSt = dynamic_cast<LetStatement*>(st.get())){
            std::cout << "LET: " << letSt->name << "\n";
        } else if (auto* printSt = dynamic_cast<PrintStatement*>(st.get())) {
            std::cout << "PRINT " << "\n";
        } else {
            std::cout << "Unknown \n";
        }
        
    }
    return 0;
}