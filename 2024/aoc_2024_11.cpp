// https://adventofcode.com/2024/day/11

#include "shared.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <ranges>
#include <stack>
#include <utility>
#include <vector>

static std::optional<std::pair<std::int64_t, std::int64_t>>
split_even_digit_count_numbers(const int64_t number) {
  const auto digit_count = static_cast<int>(std::log10(number)) + 1;
  const auto is_odd_digit_count = digit_count % 2;
  if (is_odd_digit_count) {
    return {};
  }
  const auto divisor = static_cast<std::int64_t>(std::pow(10, digit_count / 2));
  return {{number / divisor, number % divisor}};
}

static std::vector<std::int64_t> blink(const std::int64_t value) {
  if (value == 0) {
    return {1};
  } else if (const auto number_pair = split_even_digit_count_numbers(value)) {
    return {number_pair->first, number_pair->second};
  } else {
    return {2024 * value};
  }
}

struct Number {
  std::int64_t value;
  std::size_t blink_count = 0;
  auto operator<=>(const Number&) const = default;
};

static std::size_t count_numbers(
    const std::vector<Number>& numbers, const std::size_t blink_count,
    const std::map<Number, std::size_t>& cached_counts) {
  std::size_t number_count = 0;
  std::stack<Number> number_stack{numbers.cbegin(), numbers.cend()};
  while (!number_stack.empty()) {
    const auto number = number_stack.top();
    number_stack.pop();

    const auto cached_count_it =
        cached_counts.find({number.value, blink_count - number.blink_count});
    if (cached_count_it != cached_counts.cend()) {
      number_count += cached_count_it->second;
      continue;
    }

    const auto new_values = blink(number.value);
    const auto new_blink_count = number.blink_count + 1;
    if (new_blink_count >= blink_count) {
      number_count += new_values.size();
    } else {
      for (const auto new_value : new_values) {
        number_stack.push({new_value, new_blink_count});
      }
    }
  }
  return number_count;
}

static std::map<Number, std::size_t> build_cached_counts(
    const std::ranges::iota_view<int, int>& cache_value_range,
    const std::ranges::iota_view<std::size_t, std::size_t>&
        cache_blink_count_range) {
  std::map<Number, std::size_t> cached_counts;
  for (const auto [value, blink_count] : std::views::cartesian_product(
           cache_value_range, cache_blink_count_range)) {
    cached_counts[{value, blink_count}] =
        count_numbers({{value}}, blink_count, cached_counts);
  }
  return cached_counts;
}

int main() {
  const auto cached_counts =
      build_cached_counts(std::views::iota(0, 10), std::views::iota(0uz, 42uz));
  std::vector<Number> numbers{std::istream_iterator<std::int64_t>{std::cin},
                              {}};
  std::cout << count_numbers(numbers, 25, cached_counts) << "\n";
  std::cout << count_numbers(numbers, 75, cached_counts) << "\n";
}
