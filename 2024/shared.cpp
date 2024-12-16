#include "shared.hpp"

#include <cctype>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> extract_values(std::istream& istream) {
  const auto stripped_input = std::views::istream<char>(istream) |
                              std::views::transform([](char c) {
                                return std::isdigit(c) || c == '-' ? c : ' ';
                              }) |
                              std::ranges::to<std::string>();
  auto stripped_input_ss = std::istringstream{stripped_input};
  return std::views::istream<int>(stripped_input_ss) |
         std::ranges::to<std::vector<int>>();
}

Vector2i operator*(const int lhs, const Vector2i& rhs) {
  return {lhs * rhs.x, lhs * rhs.y};
}

std::ostream& operator<<(std::ostream& ostream, const Vector2i& v) {
  return ostream << "(" << v.x << ", " << v.y << ")";
}

std::ostream& operator<<(std::ostream& ostream, const Grid& grid) {
  for (const auto& s : grid.locations) {
    ostream << s << "\n";
  }
  return ostream;
}
