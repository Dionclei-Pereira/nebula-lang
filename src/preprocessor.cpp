#include "preprocessor.hpp"

#include <iostream>

namespace nbl {
Preprocessor::Preprocessor(const std::string& code) : code(code) {}

std::string Preprocessor::getWord(size_t& current) {
  std::string word;
  while (current < code.size() && !std::isspace(code[current])) {
    word += code[current];
    current++;
  }
  current++;
  return word;
}

void Preprocessor::define(std::string& from, std::string& to) {
  if (from.empty()) return;

  size_t pos = code.find(from);

  while (pos != std::string::npos) {
    code.replace(pos, from.length(), to);

    pos = code.find(from, pos + to.length());
  }
}

std::string Preprocessor::process() {
  while (true) {
    size_t pos = code.find('@');

    if (pos == std::string::npos) {
      break;
    }

    size_t current = pos;

    std::string word = getWord(current);

    if (word == "@def") {
      std::string from = getWord(current);
      std::string to = getWord(current);

      code.replace(pos, current - pos, "");
      define(from, to);
    } else {
      code.replace(pos, 1, "");
    }
  }

  return code;
}

}  // namespace nbl