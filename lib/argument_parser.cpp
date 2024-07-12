#include "argument_parser.h"

MissingArgException::MissingArgException(const std::string& short_arg,
                                         const std::string& long_arg) {
  error_message = "missing argument! set it with " + short_arg + " or " + long_arg;
}

const char* MissingArgException::what() const noexcept {
  return error_message.c_str();
}

int64_t Parser::ProcessNumericArgument(const std::string& input,
                                       const std::string& parameter,
                                       bool long_arg) {
  int64_t result;
  try {
    if (long_arg) {
      auto value = input.substr(strlen(parameter.c_str()));
      result = std::stoll(value);
    }
    else {
      result = std::stoll(input);
    }
    if (result < 0) {
      throw std::invalid_argument("the value must be non-negative!");
    }
    return result;
  }
  catch (const std::invalid_argument& e) {
    std::cerr << "invalid argument for " + parameter + " option: " << e.what()
        << std::endl;
    exit(1);
  }
  catch (const std::out_of_range& e) {
    std::cerr << "argument value out of range for " + parameter + " option: " << e.what() << std::endl;
    exit(1);
  }
}

void Parser::CheckArguments(Sandpile& sandpile) {
  if (sandpile.freq() == -1) {
    throw MissingArgException("-f", "--freq");
  }
  if (sandpile.max_iteration() == -1) {
    throw MissingArgException("-m", "--max_iter");
  }
  if (sandpile.input_dir().empty()) {
    throw MissingArgException("-o", "--output");
  }
  if (sandpile.output_dir().empty()) {
    throw MissingArgException("-i", "--input");
  }
}

void Parser::Parse(int argc, char** argv, Sandpile& sandpile) {
  std::string input;
  for (int i = 1; i < argc; ++i) {
    input = argv[i];
    if (input.starts_with("--input=")) {
      sandpile.set_input_dir(input.substr(strlen("--input=")));
    }
    else if (input.starts_with("--output=")) {
      sandpile.set_output_dir(input.substr(strlen("--output=")));
    }
    else if (input.starts_with("--max-iter=")) {
      sandpile.set_max_iteration(
        ProcessNumericArgument(input, "--max-iter=", true));
    }
    else if (input.starts_with("--freq=")) {
      sandpile.set_freq(ProcessNumericArgument(input, "--freq=", true));
    }

    else {
      if (i + 1 < argc) {
        if (input == "-i") {
          sandpile.set_input_dir(argv[++i]);
        }
        else if (input == "-o") {
          sandpile.set_output_dir(argv[++i]);
        }
        else if (input == "-m") {
          sandpile.set_max_iteration(
            ProcessNumericArgument(argv[++i], "-m", false));
        }
        else if (input == "-f") {
          sandpile.set_freq(ProcessNumericArgument(argv[++i], "-f", false));
        }
      }
    }
  }
  try {
    CheckArguments(sandpile);
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
}
