// https://adventofcode.com/2024/day/2

#include "shared.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

static bool is_monotonic(const std::vector<int>& values,
                         const int min_value_distance,
                         const int max_value_distance) {
  if (values.size() < 2) {
    return true;
  }
  auto previous_value = values[0];
  const auto is_increasing = values[1] > previous_value;
  for (auto it = values.cbegin() + 1; it != values.cend(); ++it) {
    const auto value = *it;
    const auto value_distance = std::abs(previous_value - value);
    const auto is_bad_value = value_distance < min_value_distance ||
                              value_distance > max_value_distance ||
                              (value > previous_value) != is_increasing;
    if (is_bad_value) {
      return false;
    }
    previous_value = value;
  }
  return true;
}

static std::vector<int> remove_value(const std::vector<int>& values,
                                     const std::size_t index) {
  std::vector<int> new_values;
  std::copy(
      values.cbegin(),
      values.cbegin() + static_cast<std::vector<int>::difference_type>(index),
      std::back_inserter(new_values));
  std::copy(values.cbegin() +
                static_cast<std::vector<int>::difference_type>(index) + 1,
            values.cend(), std::back_inserter(new_values));
  return new_values;
}

int main() {
  std::vector<std::string> lines;
  std::copy(std::istream_iterator<Line>{std::cin}, {},
            std::back_inserter(lines));
  std::size_t n_safe = 0;
  std::size_t n_safe_dampened = 0;
  for (const auto& line : lines) {
    std::stringstream ss{line};
    std::vector<int> values;
    std::copy(std::istream_iterator<int>{ss}, {}, std::back_inserter(values));

    if (is_monotonic(values, 1, 3)) {
      n_safe++;
    }

    for (auto i = 0uz; i < values.size(); i++) {
      if (is_monotonic(remove_value(values, i), 1, 3)) {
        n_safe_dampened++;
        break;
      }
    }
  }
  std::cout << n_safe << "\n";
  std::cout << n_safe_dampened << "\n";
}
