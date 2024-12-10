// https://adventofcode.com/2024/day/4

#include "shared.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

static bool find_xmas_in_direction(const Grid& grid, const Vector2i& location,
                                   const Vector2i& direction) {
  static constexpr std::string word = "XMAS";
  auto word_location = location;
  for (auto word_index = 0uz; word_index < word.size(); word_index++) {
    if (!grid.is_inside(word_location)) {
      return false;
    }
    if (grid.location(word_location) != word[word_index]) {
      return false;
    }
    word_location += direction;
  }
  return true;
}

static int count_xmas(const Grid& grid, const Vector2i& location) {
  return std::ranges::count_if(
      Grid::DIRECTIONS, [&](const Vector2i& direction) {
        return find_xmas_in_direction(grid, location, direction);
      });
}

static std::pair<std::string, std::string> get_diagonals(
    const Grid& grid, const Vector2i& location) {
  return {{grid.location({location.x - 1, location.y - 1}),
           grid.location({location.x, location.y}),
           grid.location({location.x + 1, location.y + 1})},
          {grid.location({location.x + 1, location.y - 1}),
           grid.location({location.x, location.y}),
           grid.location({location.x - 1, location.y + 1})}};
}

static int count_x_mas(const Grid& grid, const Vector2i& location) {
  const auto diagonals = get_diagonals(grid, location);
  const auto is_valid_x_mas =
      (diagonals.first == "MAS" || diagonals.first == "SAM") &&
      (diagonals.second == "MAS" || diagonals.second == "SAM");
  return is_valid_x_mas ? 1 : 0;
}

int main() {
  const Grid grid{std::cin};

  auto n_xmas = 0;
  for (auto y = 0; y < grid.size().y; y++) {
    for (auto x = 0; x < grid.size().x; x++) {
      n_xmas += count_xmas(grid, {x, y});
    }
  }
  std::cout << n_xmas << "\n";

  auto n_x_mas = 0;
  for (auto y = 1; y < grid.size().y - 1; y++) {
    for (auto x = 1; x < grid.size().x - 1; x++) {
      n_x_mas += count_x_mas(grid, {x, y});
    }
  }
  std::cout << n_x_mas << "\n";
}
