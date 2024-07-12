#pragma once
#include <iostream>
#include <string_view>
#include <sandpile.h>

class MissingArgException : public std::exception {
public:
  MissingArgException(const std::string& short_arg,
                      const std::string& long_arg);
  [[nodiscard]] const char* what() const noexcept override;

private:
  std::string error_message;
};

class Parser {
public:
  static int64_t ProcessNumericArgument(const std::string& input,
                                        const std::string& parameter,
                                        bool long_arg);
  static void CheckArguments(Sandpile& sandpile);
  static void Parse(int argc, char** argv, Sandpile& sandpile);
};
