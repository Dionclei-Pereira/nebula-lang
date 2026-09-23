#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "lexer.hpp"
#include "parser.hpp"
#include "preprocessor.hpp"

#define NEBULA_VERSION "0.0.1"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Usage: nebula [options] <file>\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --build       Generate preprocessed output\n";
    std::cout << "  --version     Show Nebula version\n";
    return 1;
  }

  std::string fileName;
  bool build = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--version") {
      std::cout << "Nebula Version: " << NEBULA_VERSION << '\n';
      return 0;
    }

    if (arg == "--build") {
      build = true;
      continue;
    }

    if (arg.rfind("--", 0) != 0) {
      if (!fileName.empty()) {
        std::cerr << "Error: multiple files specified\n";
        return 1;
      }

      fileName = arg;
      continue;
    }

    std::cerr << "Error: unknown option: " << arg << '\n';
    return 1;
  }

  if (fileName.empty()) {
    std::cerr << "Error: no input file specified\n";
    return 1;
  }

  std::filesystem::path filePath(fileName);
  std::string extension = filePath.extension().string();

  if (extension != ".nb" && extension != ".ns") {
    std::cerr << "Error: invalid file extension: " << extension << '\n';
    return 1;
  }

  std::ifstream file(filePath);

  if (!file) {
    std::cerr << "Error: could not open the file: " << filePath << '\n';
    return 1;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string code = buffer.str();

  if (extension == ".ns") {
    nbl::Preprocessor pre(code);
    code = pre.process();

    if (build) {
      std::filesystem::path outputPath = filePath;
      outputPath.replace_extension(".nb");

      std::ofstream output(outputPath);

      if (!output) {
        std::cerr << "Error: could not create output file: " << outputPath << '\n';
        return 1;
      }

      output << code;

      std::cout << "Built: " << outputPath << '\n';

      return 0;
    }
  }

  std::cout << code;

  nbl::Lexer lexer(code);
  std::vector<nbl::Token> tokens = lexer.tokenize();

  std::cout << "\n\nTokens:\n";

  for (const nbl::Token& token : tokens) {
    std::cout << static_cast<int>(token.type) << ": " << token.value << '\n';
  }

  nbl::Parser parser(tokens);
  std::vector<std::unique_ptr<nbl::Statement>> statements = parser.parse();

  std::cout << "\nStatements:\n";

  for (const auto& st : statements) {
    if (auto* letSt = dynamic_cast<nbl::LetStatement*>(st.get())) {
      std::cout << "LET: " << letSt->name << '\n';

    } else if (auto* printSt = dynamic_cast<nbl::PrintStatement*>(st.get())) {
      std::cout << "PRINT\n";

    } else {
      std::cout << "UNKNOWN\n";
    }
  }

  return 0;
}