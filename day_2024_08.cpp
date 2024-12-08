// https://adventofcode.com/2024/day/8

#include "shared.hpp"

#include <cctype>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <vector>

int main() {
  const Grid grid{std::cin};
  std::map<char, std::vector<Vector2i>> antennae;
  for (auto grid_it = grid.cbegin(); grid_it != grid.cend(); ++grid_it) {
    if (std::isalnum(*grid_it)) {
      antennae[*grid_it].push_back(grid_it);
    }
  }

  std::set<Vector2i> antinodes;
  std::set<Vector2i> multi_antinodes;
  for (const auto& antenna : antennae) {
    for (const auto& [antenna_a, antenna_b] :
         std::ranges::cartesian_product_view(antenna.second, antenna.second)) {
      if (antenna_a == antenna_b) {
        continue;
      }
      auto antinode = antenna_a;
      const auto delta = antenna_b - antenna_a;

      if (grid.is_inside(antinode - delta)) {
        antinodes.insert(antinode - delta);
      }

      while (grid.is_inside(antinode)) {
        multi_antinodes.insert(antinode);
        antinode -= delta;
      }
    }
  }
  std::cout << antinodes.size() << "\n";
  std::cout << multi_antinodes.size() << "\n";
}
