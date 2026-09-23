#pragma once

#include <string>

namespace nbl {

class Preprocessor {
 private:
  std::string code;

  void define(std::string& from, std::string& to);
  std::string getWord(size_t& current);
 public:
  Preprocessor(const std::string& code);

  std::string process();
};

}  // namespace nbl