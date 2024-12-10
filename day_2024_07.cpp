// https://adventofcode.com/2024/day/7

#include "shared.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

struct Calibration {
  std::int64_t result;
  std::vector<std::int64_t> operands;
};

std::vector<Calibration> extract_calibrations(std::istream& istream) {
  std::vector<Calibration> calibrations;
  for (auto line_it = std::istream_iterator<Line>{istream};
       line_it != std::istream_iterator<Line>{}; ++line_it) {
    std::string line;
    std::ranges::copy_if(
        std::string{*line_it}, std::back_inserter(line),
        [](const char c) { return std::isdigit(c) || std::isspace(c); });
    std::stringstream line_ss{line};
    Calibration calibration;
    auto value_it = std::istream_iterator<std::int64_t>{line_ss};
    calibration.result = *value_it++;
    std::copy(value_it, {}, std::back_inserter(calibration.operands));
    calibrations.push_back(calibration);
  }
  return calibrations;
}

struct Operator {
  std::string available_operators;
  std::size_t operator_index = 0;

  Operator(const std::string& available_operators)
      : available_operators{available_operators} {}

  std::int64_t evaluate(const std::int64_t left,
                        const std::int64_t right) const {
    switch (available_operators[operator_index]) {
      case '+':
        return left + right;
      case '*':
        return left * right;
      case '|':
        return left * std::pow(10, std::floor(std::log10(right)) + 1) + right;
      default:
        return 0;
    }
  }

  bool next() {
    operator_index = (operator_index + 1) % available_operators.size();
    return operator_index != 0;  // false if overflow
  }
};

struct Operators {
  Operators(const std::size_t n_operators,
            const std::string available_operators) {
    for (auto i = 0uz; i < n_operators; i++) {
      operators.push_back(available_operators);
    }
  }

  bool next() {
    for (auto& op : operators) {
      if (op.next()) {
        return true;
      }
    }
    return false;  // false if overflow
  }

  std::int64_t evaluate(const std::vector<std::int64_t>& operands) const {
    auto result = operands[0];
    for (auto operand_index = 1uz; operand_index < operands.size();
         operand_index++) {
      result = operators[operand_index - 1].evaluate(result,
                                                     operands[operand_index]);
    }
    return result;
  }

  std::vector<Operator> operators;
};

bool test_calibration(const Calibration& calibration,
                      const std::string& available_operators) {
  Operators operators{calibration.operands.size() - 1, available_operators};
  while (true) {
    if (operators.evaluate(calibration.operands) == calibration.result) {
      return true;
    }
    if (!operators.next()) {
      return false;
    }
  }
}

int main() {
  const auto calibrations = extract_calibrations(std::cin);

  std::int64_t n_calibration_sum = 0;
  for (const auto& calibration : calibrations) {
    if (test_calibration(calibration, "+*")) {
      n_calibration_sum += calibration.result;
    }
  }
  std::cout << n_calibration_sum << "\n";

  std::int64_t n_calibration_sum_concat = 0;
  for (const auto& calibration : calibrations) {
    if (test_calibration(calibration, "+*|")) {
      n_calibration_sum_concat += calibration.result;
    }
  }
  std::cout << n_calibration_sum_concat << "\n";
}
