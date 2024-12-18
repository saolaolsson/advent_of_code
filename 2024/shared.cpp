#include "shared.hpp"

#include <cctype>
#include <ios>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> extract_values(std::istream& istream) {
  istream >> std::noskipws;
  const auto stripped_input = std::views::istream<char>(istream) |
                              std::views::transform([](char c) {
                                return std::isdigit(c) || c == '-' ? c : ' ';
                              }) |
                              std::ranges::to<std::string>();
  auto stripped_input_ss = std::istringstream{stripped_input};
  return std::views::istream<int>(stripped_input_ss) |
         std::ranges::to<std::vector<int>>();
}

void print_grid(const Grid& grid, std::vector<Vector2i>& locations) {
  auto new_grid = grid;
  for (const auto& location : locations) {
    new_grid.location(location) = 'x';
  }
  std::cout << "\033c";
  std::cout << "\033[H";
  std::cout << new_grid << "\n";
}

Vector2i operator*(const int lhs, const Vector2i& rhs) {
  return {lhs * rhs.x, lhs * rhs.y};
}

std::ostream& operator<<(std::ostream& ostream, const Vector2i& v) {
  return ostream << "(" << v.x << ", " << v.y << ")";
}

std::ostream& operator<<(std::ostream& ostream, const Matrix<char>& matrix) {
  for (const auto& v : matrix.locations) {
    ostream << std::string{v.begin(), v.end()} << "\n";
  }
  return ostream;
}
