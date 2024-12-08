// https://adventofcode.com/2024/day/3

#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>

static int sum_of_products(const std::string& input) {
  const std::regex regex{"mul\\((\\d{1,3}),(\\d{1,3})\\)"};
  const auto begin = std::sregex_iterator{input.cbegin(), input.cend(), regex};
  return std::accumulate(begin, {}, 0,
                         [](const int sum, const std::smatch& match) {
                           const auto multiplicator = std::stoi(match.str(1));
                           const auto multiplicand = std::stoi(match.str(2));
                           return sum + multiplicator * multiplicand;
                         });
}

int main() {
  std::string input;  // regex needs bidirectional iterator
  std::copy(std::istream_iterator<char>{std::cin}, {},
            std::back_inserter(input));

  std::cout << sum_of_products(input) << "\n";

  std::regex regex_trim_donts{"don't\\(\\).*?(do\\(\\)|$)"};
  input = std::regex_replace(input, regex_trim_donts, "");
  std::cout << sum_of_products(input) << "\n";
}
